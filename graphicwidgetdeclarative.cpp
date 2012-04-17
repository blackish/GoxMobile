#include "graphicwidgetdeclarative.h"

GraphicWidgetDeclarative::GraphicWidgetDeclarative(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    proxy = new QGraphicsProxyWidget(this);
    proxy->setWidget(widget);


}

GraphicWidgetDeclarative::~GraphicWidgetDeclarative ()
{
    delete widget;
    delete proxy;
}
