#include "tradelistdeclarative.h"

TradeListDeclarative::TradeListDeclarative(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
//    this->setHeight( 854 );
//    this->setWidth( 480 );

    widget = new QTableWidget ();
    proxy = new QGraphicsProxyWidget(this);
    this->setFlag(QGraphicsItem::ItemHasNoContents, false);
    widget->setMinimumSize(480,840);
    proxy->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding,QSizePolicy::TabWidget );
    widget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    widget->setRowCount( 21 );
    widget->setColumnCount( 2 );
    widget->setColumnWidth( 0, 200 );
    widget->setColumnWidth( 1, 200 );
    for ( int i=0;i<=2;i++ )
    {
        for ( int j=0;j<=20;j++)
        {
            QTableWidgetItem* item = new QTableWidgetItem ();
            item->setText ( QString::number ( 0 ) );
            widget->setItem ( j, i, item );
            widget->setRowHeight( j, 32 );
        }
    }
    proxy->setWidget(widget);
}

QTableWidget* TradeListDeclarative::getTableWidget ()
{
    return widget;
}
