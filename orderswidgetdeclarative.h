#ifndef ORDERSWIDGETDECLARATIVE_H
#define ORDERSWIDGETDECLARATIVE_H

#include <QDeclarativeItem>
#include <QGraphicsProxyWidget>
#include "orderswidget.h"

class OrdersWidgetDeclarative : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit OrdersWidgetDeclarative(QDeclarativeItem *parent = 0);
    ~OrdersWidgetDeclarative ();
    OrdersWidget* getOrdersWidget ();
private:
    OrdersWidget* widget;
    QGraphicsProxyWidget *proxy;

signals:
    
public slots:
    
};

#endif // ORDERSWIDGETDECLARATIVE_H
