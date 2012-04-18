#ifndef GRAPHICWIDGETDECLARATIVE_H
#define GRAPHICWIDGETDECLARATIVE_H

#include <QDeclarativeItem>
#include <QGraphicsProxyWidget>
#include "graphicwidget.h"

class GraphicWidgetDeclarative : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit GraphicWidgetDeclarative(QDeclarativeItem *parent = 0);
    ~GraphicWidgetDeclarative ();
    GraphicWidget* getGraphicWidget ();
private:
    GraphicWidget *widget;
    QGraphicsProxyWidget *proxy;

signals:
    
public slots:
    
};

#endif // GRAPHICWIDGETDECLARATIVE_H
