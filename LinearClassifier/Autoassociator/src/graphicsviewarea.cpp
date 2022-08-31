#include "graphicsviewarea.h"

GraphicsViewArea::GraphicsViewArea()
    : mView(new GraphicsView)
{
    this->setBackgroundRole(QPalette::Dark);
    this->setWidget(mView);
}

GraphicsViewArea::~GraphicsViewArea()
{
    delete mView;
}

void GraphicsViewArea::adjustScrollBar(QScrollBar* _scrollBar, double _factor)
{
    _scrollBar->setValue(int(_factor * _scrollBar->value()
        + ((_factor - 1) * _scrollBar->pageStep()/2)));
}

void GraphicsViewArea::setView(const QImage& _img)
{
    mView->loadImage(_img);
}

void GraphicsViewArea::setView(const QSize& _size)
{
    mView->loadImage(_size);
}

void GraphicsViewArea::setView(int _width, int _height)
{
    mView->loadImage(QSize(_width, _height));
}

void GraphicsViewArea::updateScrollArea()
{
    adjustScrollBar(this->horizontalScrollBar(), mView->scaleFactor());
    adjustScrollBar(this->verticalScrollBar(), mView->scaleFactor());
}
