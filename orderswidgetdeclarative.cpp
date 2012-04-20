#include "orderswidgetdeclarative.h"
#include <QGraphicsScene>

OrdersWidgetDeclarative::OrdersWidgetDeclarative(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{

    setFlag(QGraphicsItem::ItemHasNoContents, false);
    proxy = new QGraphicsProxyWidget(this);
    widget = new OrdersWidget (  );
    widget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    widget->setMinimumSize(480,840);
//    proxy->scene()->addWidget( widget );
    proxy->setWidget( widget );


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

