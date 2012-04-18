#include "orderswidgetdeclarative.h"

OrdersWidgetDeclarative::OrdersWidgetDeclarative(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    widget = new OrdersWidget ();
    proxy = new QGraphicsProxyWidget(this);
    proxy->setWidget(widget);
}

OrdersWidgetDeclarative::~OrdersWidgetDeclarative ()
{
//    delete proxy;
//    delete widget;
}

OrdersWidget* OrdersWidgetDeclarative::getOrdersWidget()
{
    return widget;
}
