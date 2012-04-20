#include "graphicwidgetdeclarative.h"

GraphicWidgetDeclarative::GraphicWidgetDeclarative(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
//    this->setHeight( 854 );
//    this->setWidth( 480 );

    widget = new GraphicWidget ();
    proxy = new QGraphicsProxyWidget(this);
    proxy->setWidget(widget);
    widget->setMinimumSize(480,840);
}

GraphicWidgetDeclarative::~GraphicWidgetDeclarative ()
{
//    delete proxy;
//    delete widget;
}

GraphicWidget* GraphicWidgetDeclarative::getGraphicWidget ()
{
    return widget;
}
