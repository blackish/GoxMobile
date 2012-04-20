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
//    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
private:
    OrdersWidget* widget;
    QGraphicsProxyWidget *proxy;

signals:
    
public slots:
    
};

#endif // ORDERSWIDGETDECLARATIVE_H
