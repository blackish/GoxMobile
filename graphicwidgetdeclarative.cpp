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
    delete widget;
    delete proxy;
}

GraphicWidget* GraphicWidgetDeclarative::getGraphicWidget ()
{
    return widget;
}
