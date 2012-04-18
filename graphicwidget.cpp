#include "graphicwidget.h"
#include <cmath>
#include <QPainter>

GraphicWidget::GraphicWidget(QWidget *parent) :
    QWidget(parent)
{
    currentMin = 0;
    currentMax = 0;
    currentYAsk = -1;
    currentYBid = -1;
    currentYTrade = -1;
    this->setAttribute( Qt::WA_OpaquePaintEvent );
    setAttribute(Qt::WA_OpaquePaintEvent, true );
    this->setAttribute(Qt::WA_NoSystemBackground);
}

void GraphicWidget::updateAsk ( float ask )
{
//    currentMax = ask;
    asks.prepend( ask );
    if ( ask > currentMax )
        currentMax = ask;
    if ( ask < currentMin )
        currentMin = ask;
    repaint ();
}

void GraphicWidget::updateBid ( float bid )
{
//    currentMin = bid;
    bids.prepend( bid );
    if ( bid > currentMax )
        currentMax = bid;
    if ( bid < currentMin )
        currentMin = bid;
    repaint ();
}

void GraphicWidget::updateTrade ( float trade )
{
//    currentTrade = trade;
    trades.prepend( trade );
    if ( trade > currentMax )
        currentMax = trade;
    if ( trade < currentMin )
        currentMin = trade;
    repaint ();
}

void GraphicWidget::paintEvent( QPaintEvent* event )
{
    int geomX;
    int geomY;
    float scale;
    float min;
    float max;
    float value;
    QPen pen;
    QPainter painter ( this );
    min = 999999999;
    max = 0;
    geomY = this->height();
    geomX = this->width();

    for ( int i=asks.size();i>geomY/10;i--)
        asks.removeLast();

    for ( int i=bids.size();i>geomY/10;i--)
        bids.removeLast();

    for ( int i=trades.size();i>geomY/10;i--)
        trades.removeLast();

    QListIterator<float> asksIterator ( asks );
    while ( asksIterator.hasNext() )
    {
        value = asksIterator.next();
        if ( value > max )
            max = value;
        if ( value < min )
            min = value;
    }

    QListIterator<float> bidsIterator ( bids );
    while ( bidsIterator.hasNext() )
    {
        value = bidsIterator.next();
        if ( value > max )
            max = value;
        if ( value < min )
            min = value;
    }
    QListIterator<float> tradesIterator ( trades );
    while ( tradesIterator.hasNext() )
    {
        value = tradesIterator.next();
        if ( value > max )
            max = value;
        if ( value < min )
            min = value;
    }
    min = floor ( min );
    max = floor ( max ) + 1;
    scale = geomX/( max - min );

    if ( bids.size() >= 2 )
    {
        bidsIterator.toFront();
        geomY = 0;
        pen.setColor ( Qt::blue );
        painter.setPen ( pen );
        value = bidsIterator.next ();
        while ( bidsIterator.hasNext() )
        {
            value = bidsIterator.next ();
//            painter.drawEllipse((value - min) * scale, geomY, 2, 2 );
            painter.drawLine( (bidsIterator.peekPrevious() - min ) * scale, geomY, (value - min) * scale, geomY + 10 );
            geomY+=10;
        }
    }
    if ( trades.size() >= 2 )
    {
        tradesIterator.toFront();
        value = tradesIterator.next ();
        geomY = 0;
        pen.setColor ( Qt::green );
        painter.setPen ( pen );
        while ( tradesIterator.hasNext() )
        {
            value = tradesIterator.next ();
//            painter.drawEllipse((value - min) * scale, geomY, 2, 2 );
            painter.drawLine( (tradesIterator.peekPrevious() - min ) * scale , geomY, (value - min) * scale, geomY + 10 );
            geomY+=10;
        }
    }
    if ( asks.size() >= 2 )
    {
        asksIterator.toFront();
        value = tradesIterator.next ();
        geomY = 0;
        pen.setColor ( Qt::red );
        painter.setPen ( pen );
        while ( asksIterator.hasNext() )
        {
            value = asksIterator.next ();
//            painter.drawEllipse((value - min) * scale, geomY, 2, 2 );
            painter.drawLine( (asksIterator.peekPrevious() - min ) * scale, geomY, (value - min) * scale, geomY + 10 );
            geomY+=10;
        }
    }
/*

    if ( currentYAsk < 0 || currentYBid < 0 ||  currentYTrade < 0 )
     {
         painter.fillRect ( 0, 0, geomX, geomY, QBrush ( Qt::white ) );
         currentYAsk = 0;
         currentYBid = 0;
         currentYTrade = 0;
     }

    if ( currentYAsk >= geomX || currentYAsk < 0 || currentYBid >= geomX || currentYBid < 0 || currentYTrade >= geomX || currentYTrade < 0 )
    {
        currentYAsk = 0;
        currentYBid = 0;
        currentYTrade = 0;
        for ( int i=0;i<geomY;i++ )
        {
            pen.setColor ( Qt::white );
            painter.setPen ( pen );
            painter.drawEllipse(currentYTrade,i, 2, 2 );
            pen.setColor ( Qt::yellow );
            painter.setPen ( pen );
            painter.drawEllipse(currentYTrade+2,i, 2, 2 );
        }
    }
        min = floor ( currentMin );
        max = floor ( currentMax ) + 1;
    scale = geomY/( max - min );
    pen.setColor ( Qt::white );
    painter.setPen ( pen );
    for ( int i=0;i<geomY;i++ )
    {
        pen.setColor ( Qt::white );
        painter.setPen ( pen );
        painter.drawEllipse(currentYTrade+2,i, 2, 2 );
        pen.setColor ( Qt::yellow );
        painter.setPen ( pen );
        painter.drawEllipse(currentYTrade+4,i, 2, 2 );
    }
    pen.setColor ( Qt::blue );
    painter.setPen ( pen );
    painter.drawEllipse(currentYBid, geomY - floor ((currentMin-min)*scale ), 2, 2 );
    currentYBid+=2;
    pen.setColor ( Qt::red );
    painter.setPen ( pen );
    painter.drawEllipse(currentYAsk, geomY - floor ( (currentMax - min)*scale ), 2, 2 );
    currentYAsk+=2;
    pen.setColor ( Qt::green );
    painter.setPen ( pen );
    painter.drawEllipse(currentYAsk, geomY - floor ( (currentTrade - min)*scale ), 2, 2 );
    currentYTrade+=2;*/

}

void GraphicWidget::resizeEvent ( QResizeEvent * event )
{
    currentYAsk = -1;

}

