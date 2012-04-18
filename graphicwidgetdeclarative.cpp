#include "graphicwidgetdeclarative.h"

GraphicWidgetDeclarative::GraphicWidgetDeclarative(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    widget = new GraphicWidget ();
    proxy = new QGraphicsProxyWidget(this);
    proxy->setWidget(widget);
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
