//#include "btctrader.h"

/*BTCTrader::BTCTrader(QObject *parent) :
    QObject(parent)
{

}*/

#include "btctrader.h"
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QTableWidgetItem>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <openssl/engine.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>



//#include "passworddialog.h"
//#include "preferencedialog.h"
//#include "historydialog.h"
//#include "tradehistorywidget.h"
#include <QDateTime>
//#include <QtCrypto/QtCrypto>

BTCTrader::BTCTrader(QObject *parent) :
    QObject(parent)
{
    failedLogin = false;
    errorCount = 0;
    timerDepth = new QTimer ( this );
    timerTicker = new QTimer ( this );
    timerBalance = new QTimer ( this );
    timerOrders = new QTimer ( this );
/*    PasswordDialog* passwordDialog = new PasswordDialog ( this );
    passwordDialog->exec();
    QString openPassword = passwordDialog->getPassword ();
    delete passwordDialog;*/
    btctrader = new QSettings ( "blackish", "btctrader" );
//    openKeys ( openPassword );
    fee = btctrader->value( "fee" ).toFloat();
    request = new QNetworkAccessManager ( this );

    nonce = QDateTime::currentDateTime().toTime_t();// * 10000000;
    nonce = nonce * 10000000;

    connect ( request, SIGNAL ( finished ( QNetworkReply* ) ), this, SLOT ( gotReply ( QNetworkReply* ) ) );
//    graph = new GraphicWidget ( this );
/*    ordersGraph = new OrdersWidget ( this );
    myOrderTableWidget = new MyOrdersTableWidget ( this );
    myOrderTableWidget->verticalHeader()->setVisible( false );
    myOrderTableWidget->horizontalHeader()->setVisible( false );
    myOrderTableWidget->setSizePolicy( QSizePolicy ( QSizePolicy::Ignored, QSizePolicy::Ignored ) );*/

/*    ui->myOrdersWidget->setLayout( new QGridLayout () );
    ui->myOrdersWidget->layout()->addWidget( myOrderTableWidget );
    ui->widgetOrders->setLayout( new QGridLayout () );
    ui->widgetOrders->layout()->addWidget( ordersGraph );
    ui->widgetGraphics->setLayout( new QGridLayout () );
    ui->widgetGraphics->layout()->addWidget( graph );*/

    connect ( timerDepth, SIGNAL ( timeout () ), this, SLOT ( sendDepthRequest () ) );
    connect ( timerTicker, SIGNAL ( timeout () ), this, SLOT ( sendTickerRequest () ) );
    connect ( timerBalance, SIGNAL ( timeout () ), this, SLOT ( sendBalanceRequest () ) );
    connect ( timerOrders, SIGNAL ( timeout () ), this, SLOT ( sendOrdersRequest () ) );
//    connect ( myOrderTableWidget, SIGNAL ( cancelOrderSignal ( QString, int ) ), this, SLOT ( sendCancelOrderRequest ( QString , int ) ));
/*    connect ( ui->buyAmountLineEdit, SIGNAL ( textChanged ( QString  ) ), this, SLOT ( buyTextEdited ( QString ) ) );
    connect ( ui->buyRateLineEdit, SIGNAL ( textChanged ( QString ) ), this, SLOT ( buyTextEdited ( QString ) ) );
    connect ( ui->sellAmountLineEdit, SIGNAL ( textChanged ( QString  ) ), this, SLOT ( sellTextEdited ( QString ) ) );
    connect ( ui->sellRateLineEdit, SIGNAL ( textChanged ( QString ) ), this, SLOT ( sellTextEdited ( QString ) ) );
    connect ( ui->buyPushButton, SIGNAL ( clicked ( bool ) ), this, SLOT ( buyOrder ( bool ) ) );
    connect ( ui->sellPushButton, SIGNAL ( clicked ( bool ) ), this, SLOT ( sellOrder ( bool ) ) );
    connect ( ui->actionPreference, SIGNAL ( triggered () ), this, SLOT ( preferences () ) );
    connect ( ui->actionHistory, SIGNAL ( triggered () ), this, SLOT ( tradeHistory () ) );
    connect ( ui->actionClear_error, SIGNAL ( triggered () ), this, SLOT ( clearError() ) );*/
    balance = 0;
    balanceUSD = 0;
    poolInterval = 10000;

    timerDepth->start( poolInterval );
    timerTicker->start( poolInterval );
    timerOrders->start ( poolInterval );
}

BTCTrader::~BTCTrader()
{
//    delete ui;
    delete request;
//    delete graph;
//    delete ordersGraph;
    delete timerDepth;
    delete timerTicker;
    delete timerBalance;
    delete timerOrders;
//    delete myOrderTableWidget;
    btctrader->sync();
    delete btctrader;
}

int BTCTrader::gotReply ( QNetworkReply* reply )
{
 /*   if ( graph == NULL )
    {
//        timerOrders->start ( poolInterval );
        return 0;
    }*/
    QByteArray result;
    float bid = 0;
    float ask = 0;
    float trade = 0;
    result = reply->readAll ();

    QScriptValue sc;
    QScriptEngine engine;
    sc = engine.evaluate("(" + QString(result) + ")");
    if (sc.property("asks").isArray())
    {
            QStringList items;
            qScriptValueToSequence(sc.property("asks"), items);
            QMap<float,float> hash;
            for ( int i=0;i<items.count();i++ )
            {
                if ( hash[items.at ( i ).split ( ',') .at ( 0 ).toFloat()] > 0 )
                {
                    hash[items.at ( i ).split ( ',') .at ( 0 ).toFloat()] += items.at ( i ).split ( ',') .at ( 1 ).toFloat();
                } else {
                    hash.insert( items.at ( i ).split ( ',') .at ( 0 ).toFloat(), items.at ( i ).split ( ',') .at ( 1 ).toFloat() );
                }
            }
            QList<float> keys = hash.keys();
            qSort ( keys );
            ask = keys.at ( 0 );
            float balanceBs = 0;
            float count = balanceUSD;
            int counter = 0;
            while ( count > 0 )
            {
                if ( count >= hash[keys.at ( counter )]*keys.at ( counter ) )
                {
                    balanceBs += hash[keys.at ( counter )];
                    count -= hash[keys.at ( counter )]*keys.at ( counter );
                } else {
                    balanceBs += count/keys.at ( counter );
                    count = 0;
                }
                counter++;
            }
            balanceBs -= balanceBs/100*fee;
//            ui->USDLabel->setText( QString::number ( balanceUSD ) );
//            ui->USDtoBTCLabel->setText (QString::number ( balanceBs ) );


            if ( graph != NULL )
                graph->updateAsk( ask );
            if ( ordersGraph != NULL )
                ordersGraph->updateAsks( hash );
            if ( tradeList != NULL )
            {
                for ( int i=9; i>=0;i-- )
                {
                      QTableWidgetItem* item;
                      item = tradeList->takeItem ( i, 0 );
                      ask = (float)keys.at ( 9 - i );
                      item->setText( QString::number ( ask ) );
                      item->setForeground( Qt::red );
                      tradeList->setItem ( i, 0, item );

                      item = tradeList->takeItem ( i, 1 );
                      ask = (float)hash.value( keys.at ( 9 - i ) );
                      item->setText( QString::number ( ask ) );
                      tradeList->setItem ( i, 1, item );
                }
            }
    }
    if (sc.property("bids").isArray())
    {
            QStringList items;
            qScriptValueToSequence(sc.property("bids"), items);
            QMap<float,float> hash;
            for ( int i=0;i<items.count();i++ )
            {
                if ( hash[items.at ( i ).split ( ',') .at ( 0 ).toFloat()] > 0 )
                {
                    hash[items.at ( i ).split ( ',') .at ( 0 ).toFloat()] += items.at ( i ).split ( ',') .at ( 1 ).toFloat();
                } else {
                    hash.insert( items.at ( i ).split ( ',') .at ( 0 ).toFloat(), items.at ( i ).split ( ',') .at ( 1 ).toFloat() );
                }
            }
            QList<float> keys = hash.keys();
            qSort ( keys );
            bid = keys.at ( keys.count() - 1 );
            float balanceBs = 0;
            float count = balance;
            count -= count / 100 * fee;
            int counter = keys.count() - 1;
            while ( count > 0 )
            {
                if ( count >= hash[keys.at ( counter )] )
                {
                    balanceBs += hash[keys.at ( counter )] * keys.at ( counter );
                    count -= hash[keys.at ( counter ) ];
                } else {
                    balanceBs += count * keys.at ( counter );
                    count = 0;
                }
                counter--;
            }
//            ui->BTCLabel->setText( QString::number ( balance ) );
//            ui->BTCtoUSDLabel->setText (QString::number ( balanceBs ) );
            if ( graph != NULL )
                graph->updateBid( bid );
            if ( ordersGraph != NULL )
                ordersGraph->updateBids( hash );

            if ( tradeList != NULL )
            {
                for ( int i=11; i<=20;i++ )
                {
                    QTableWidgetItem* item;
                    item = tradeList->takeItem ( i, 0 );
                    bid = (float)keys.at ( keys.count() - i + 10 );
                    item->setText( QString::number ( bid ) );
                    item->setForeground( Qt::blue );
                    tradeList->setItem ( i, 0, item );

                    item = tradeList->takeItem ( i, 1 );
                    bid = (float)hash.value( keys.at ( keys.count() - i + 10 ) );
                    item->setText( QString::number ( bid ) );
                    tradeList->setItem ( i, 1, item );
                }

            }
            timerDepth->start ( poolInterval );
    }
    if ( sc.property ( "ticker" ).isObject() )
    {
        QString str = sc.property("ticker").property("last").toString();
        trade = str.toFloat ();
        graph->updateTrade( trade );
        if ( tradeList != NULL )
        {
            QTableWidgetItem* item;
            item = tradeList->takeItem ( 10, 0 );
            item->setText( str );
            item->setForeground( Qt::green );
            tradeList->setItem ( 10, 0, item );
        }
//        QTableWidgetItem* item;
//        item = ui->bidTableWidget->takeItem ( 0, 20 );
//        item->setText( str );
//        item->setForeground( Qt::green );
//        ui->bidTableWidget->setItem ( 0, 20, item );
        timerTicker->start ( poolInterval );

    }
    if ( sc.property ( "usds" ).isString() )
    {
        balanceUSD=sc.property ( "usds" ).toNumber();
    }
    if ( sc.property ( "btcs" ).isString() )
    {
        balance = sc.property ( "btcs" ).toNumber();
    }

    if ( sc.property ( "orders" ).isObject()/* && ! myOrderTableWidget->contextMenu->isVisible()*/ )
    {
        errorCount = 0;
//        myOrderTableWidget->clear();
//        myOrderTableWidget->setRowCount( 0 );
//        myOrderTableWidget->setColumnCount( 4 );
        QScriptValueIterator iterator ( sc.property( "orders" ) );
        while ( iterator.hasNext() )
        {
            iterator.next();
            QTableWidgetItem* item;
//            myOrderTableWidget->insertRow( 0 );
            item = new QTableWidgetItem ();
            item->setText( iterator.value().property( "oid" ).toString() );
            if ( iterator.value().property ( "status" ).toInteger() != 1 )
                item->setBackgroundColor( ( QColor ( Qt::gray ) ) );
//            myOrderTableWidget->setItem( 0, 0, item );
            item = new QTableWidgetItem ();
            if ( iterator.value().property( "type" ).toInteger() == 1 )
                item->setText( "S" );
            else
                item->setText( "B" );
            if ( iterator.value().property ( "status" ).toInteger() != 1 )
                item->setBackgroundColor( ( QColor ( Qt::gray ) ) );
//            myOrderTableWidget->setItem( 0, 1, item );
            item = new QTableWidgetItem ();
            item->setText( iterator.value().property( "amount" ).toString() );
            if ( iterator.value().property ( "status" ).toInteger() != 1 )
                item->setBackgroundColor( ( QColor ( Qt::gray ) ) );
//            myOrderTableWidget->setItem( 0, 2, item );
            item = new QTableWidgetItem ();
            item->setText( iterator.value().property( "price" ).toString() );
            if ( iterator.value().property ( "status" ).toInteger() != 1 )
                item->setBackgroundColor( ( QColor ( Qt::gray ) ) );
//            myOrderTableWidget->setItem( 0, 3, item );
//            myOrderTableWidget->setRowHeight(0, 15);
        }
/*        myOrderTableWidget->removeRow( 0 );*/

/*        myOrderTableWidget->setColumnWidth( 0, 0 );
        myOrderTableWidget->setColumnWidth( 1, 20 );
        myOrderTableWidget->setColumnWidth( 2, 110 );
        myOrderTableWidget->setColumnWidth( 3, 110 );*/
        timerOrders->start( poolInterval );
    }
    if ( sc.property( "error" ).toString().length() > 0 )
    {
        errorCount += 1;
        qDebug ( QString ( result ).toStdString().c_str() );
        if ( errorCount == 5 )
        {

            failedLogin = true;
            qDebug ( sc.property( "error" ).toString().toStdString().c_str() );
//            this->ui->statusBar->showMessage ( sc.property( "error" ).toString() );
//            this->setWindowTitle( "BTCTrade - failed login" );
        }
    }

    reply->deleteLater();

    return 0;
}

void BTCTrader::sendDepthRequest()
{
    QNetworkReply* rpl;
    rpl = request->get(QNetworkRequest(QUrl("https://mtgox.com/api/0/data/getDepth.php")));
    rpl->ignoreSslErrors();

}
void BTCTrader::sendTickerRequest()
{
    QNetworkReply* rpl;
    rpl = request->get(QNetworkRequest(QUrl("https://mtgox.com/api/0/data/ticker.php")));
    rpl->ignoreSslErrors();
}

void BTCTrader::sendBalanceRequest ()
{
    if ( failedLogin )
        return;
    return;
}

void BTCTrader::sendOrdersRequest ()
{
    if ( failedLogin )
        return;
    QString header;
    QNetworkReply* rpl;
    QNetworkRequest newRequest ( QUrl ( "https://mtgox.com/api/0/getOrders.php" ) );
    header ="";
    signRequest ( &header, &newRequest );
    rpl = request->post (newRequest, header.toStdString().c_str() );
    rpl->ignoreSslErrors();

}

void BTCTrader::sendCancelOrderRequest ( QString oid, int type )
{
    if ( failedLogin )
        return;
    QString header;
    QNetworkReply* rpl;
    QNetworkRequest newRequest ( QUrl ( "https://mtgox.com/api/0/cancelOrder.php" ) );
    header = "oid="+oid+"&type="+QString::number(type);
    signRequest ( &header, &newRequest );
    rpl = request->post (newRequest, header.toStdString().c_str() );
    rpl->ignoreSslErrors();
}

void BTCTrader::sendBuyRequest ( float amount, float price )
{
    if ( failedLogin )
        return;
    QString header;
    QNetworkReply* rpl;
    header = "amount="+QString::number ( amount )+"&price="+QString::number ( price );
    QNetworkRequest newRequest ( QUrl ( "https://mtgox.com/api/0/buyBTC.php" ) );
    signRequest ( &header, &newRequest );
    rpl = request->post (newRequest, header.toStdString().c_str() );
    rpl->ignoreSslErrors();

}

void BTCTrader::sendSellRequest ( float amount, float price )
{
    if ( failedLogin )
        return;
    QString header;
    QNetworkReply* rpl;
    header = "amount="+QString::number ( amount )+"&price="+QString::number ( price );
    QNetworkRequest newRequest ( QUrl ( "https://mtgox.com/api/0/sellBTC.php" ) );
    signRequest ( &header, &newRequest );
    rpl = request->post (newRequest, header.toStdString().c_str() );
    rpl->ignoreSslErrors();
}

void BTCTrader::signRequest ( QString* header, QNetworkRequest* newRequest )
{
    nonce += 1;
    *header = ( header->length() == 0 ) ? "nonce=" + QString::number( nonce ) : "nonce=" + QString::number( nonce ) + "&" + *header;

    QByteArray key = QByteArray::fromBase64( restSign.toStdString().c_str() );
    unsigned char* result;
    unsigned int result_len = 512;
    HMAC_CTX ctx;

    result = (unsigned char*) malloc(sizeof(unsigned char) * result_len);

    ENGINE_load_builtin_engines();
    ENGINE_register_all_complete();

    HMAC_CTX_init(&ctx);
    HMAC_Init_ex(&ctx, key.constData(), key.length(), EVP_sha512(), NULL);
    HMAC_Update(&ctx, (unsigned char*)header->toAscii().constData(), header->length());
    HMAC_Final(&ctx, result, &result_len);
    HMAC_CTX_cleanup(&ctx);

    newRequest->setRawHeader( "Rest-Key", restKey.toStdString().c_str() );
    newRequest->setRawHeader( "Rest-Sign", QByteArray::fromRawData( (char*)result, result_len ).toBase64() );
    newRequest->setRawHeader( "content-type","application/x-www-form-urlencoded" );
    free ( result );
}

void BTCTrader::buyTextEdited ( QString )
{
    if ( failedLogin )
        return;
    float amount;
    float rate;
//    amount = ui->buyAmountLineEdit->text().toFloat();
//    rate = ui->buyRateLineEdit->text().toFloat();
//    ui->buyCostLabel->setText( QString::number ( ( amount * rate ) + (amount * rate / 100 * fee ) ) );
}

void BTCTrader::sellTextEdited ( QString )
{
    if ( failedLogin )
        return;
    float amount;
    float rate;
//    amount = ui->sellAmountLineEdit->text().toFloat();
//    rate = ui->sellRateLineEdit->text().toFloat();
//    ui->sellCostLabel->setText( QString::number ( ( amount - amount / 100 * fee ) * rate  ) );
}

void BTCTrader::buyOrder ( bool )
{
    if ( failedLogin )
        return;
    float amount;
    float rate;
//    amount = ui->buyAmountLineEdit->text().toFloat();
//    rate = ui->buyRateLineEdit->text().toFloat();
    if ( amount > 0 && rate > 0 )
    {
        sendBuyRequest ( amount, rate );
//        ui->buyAmountLineEdit->setText( "0" );
    }
}

void BTCTrader::sellOrder ( bool )
{
    if ( failedLogin )
        return;
    float amount;
    float rate;
//    amount = ui->sellAmountLineEdit->text().toFloat();
//    rate = ui->sellRateLineEdit->text().toFloat();
    if ( amount > 0 && rate > 0 )
    {
        sendSellRequest ( amount, rate );
//        ui->sellAmountLineEdit->setText( "0" );
    }

}

void BTCTrader::preferences()
{
/*    PreferenceDialog pref ( this );
    pref.setPoolInterval( poolInterval);
    pref.setSecret( restSign );
    pref.setKey( restKey );
    pref.setFee( fee );
    if ( pref.exec () == QDialog::Accepted )
    {
        poolInterval = pref.getPoolInterval();
        fee = pref.getFee();
        btctrader->setValue( "fee", fee );
        restKey = pref.getKey ();
        restSign = pref.getSecret ();
        QString pass = pref.getSavePassword ();
        if ( pass.length() > 1 )
            saveKeys ( pass );
    }*/
}

void BTCTrader::tradeHistory ()
{
/*    HistoryDialog trade ( this );
    TradeHistoryWidget* tradeWidget = new TradeHistoryWidget ( this );
    trade.layout()->addWidget( tradeWidget );
    trade.setWindowTitle( "Trade history" );
    trade.exec();
    delete tradeWidget;*/
}
void BTCTrader::clearError ()
{
    failedLogin = true;
    errorCount = 0;
//    this->setWindowTitle( "BTCTrade" );
        timerOrders->start ( poolInterval );
}

void BTCTrader::openKeys ( QString openPassword )
{
    if ( ! btctrader->contains( "key" ) && ! btctrader->contains( "secret" ) )
    {
        restKey = "";
        restSign = "";
        failedLogin = true;
        errorCount = 5;
//        this->setWindowTitle( "BTCTrade: login credentials are empty" );

        return;
    }
/*    QByteArray result;
    QCA::Initializer init;
    QCA::SymmetricKey encryptionKey ( QByteArray ( openPassword.toStdString().c_str() ) );
    QCA::InitializationVector iv( QByteArray ( "mtgox" ) );
    QCA::Cipher alg ( "aes128",QCA::Cipher::CBC );
    alg.setup ( QCA::Decode, encryptionKey,iv );
    QCA::SecureArray key ( QByteArray::fromBase64( btctrader->value( "key" ).toByteArray() )  );
    QCA::SecureArray resultRegion;
    resultRegion = alg.update( key );
    resultRegion += alg.final();
    result=resultRegion.toByteArray();
    restKey = QString ( result );
    QCA::SecureArray secret ( QByteArray::fromBase64( btctrader->value( "secret" ).toByteArray() ) );
    alg.setup ( QCA::Decode, encryptionKey,iv );
    resultRegion = alg.update( secret );
    resultRegion += alg.final ();
    result = resultRegion.toByteArray();
    restSign = QString ( result );*/
}

void BTCTrader::saveKeys ( QString openPassword )
{
/*    QByteArray result;
    QCA::Initializer init;
    QCA::SymmetricKey encryptionKey ( QByteArray ( openPassword.toStdString().c_str() ) );
    QCA::InitializationVector iv( QByteArray ( "mtgox" ) );
    QCA::Cipher alg ( "aes128",QCA::Cipher::CBC );
    alg.setup ( QCA::Encode, encryptionKey,iv );
    QCA::SecureArray key ( QByteArray ( restKey.toStdString().c_str() ) );
    QCA::SecureArray resultRegion;
    resultRegion = alg.update( key );
    resultRegion += alg.final();
    result = resultRegion.toByteArray();
    btctrader->setValue( "key" , QString ( result.toBase64() ) );
    alg.setup ( QCA::Encode, encryptionKey,iv );
    QCA::SecureArray secret ( QByteArray ( restSign.toStdString().c_str() ) );
    resultRegion = alg.update( secret );
    resultRegion += alg.final ();
    result = resultRegion.toByteArray();
    btctrader->setValue ( "secret", QString ( result.toBase64() ) );
    btctrader->sync();*/

    failedLogin = true;
    errorCount = 0;
//    this->setWindowTitle( "BTCTrade" );
        timerOrders->start ( poolInterval );

}

void BTCTrader::setGraphDeclarative ( GraphicWidgetDeclarative* g )
{
    graphDeclarative = g;
    graph = g->getGraphicWidget();
    if ( graphDeclarative == NULL )
        qDebug ( "NULL" );
//    timerDepth->start( poolInterval );
//    timerTicker->start( poolInterval );
}

void BTCTrader::setOrdersDeclarative ( OrdersWidgetDeclarative* g )
{
    ordersDeclarative = g;
    ordersGraph = g->getOrdersWidget();
    if ( graphDeclarative == NULL )
        qDebug ( "NULL" );
//    timerDepth->start( poolInterval );
//    timerTicker->start( poolInterval );
}
void BTCTrader::setTradeListDeclarative ( TradeListDeclarative* g )
{
    tradeListDeclarative = g;
    tradeList = g->getTableWidget();
}
