#ifndef TRADELISTDECLARATIVE_H
#define TRADELISTDECLARATIVE_H

#include <QDeclarativeItem>
#include <QTableWidget>
#include <QGraphicsProxyWidget>

class TradeListDeclarative : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit TradeListDeclarative(QDeclarativeItem *parent = 0);
    QTableWidget* getTableWidget ();
private:
    QTableWidget* widget;
    QGraphicsProxyWidget* proxy;
signals:
    
public slots:
    
};

#endif // TRADELISTDECLARATIVE_H
