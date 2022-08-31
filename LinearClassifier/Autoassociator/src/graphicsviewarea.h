#ifndef GRAPGICSVIEWAREA_H
#define GRAPGICSVIEWAREA_H

#include "graphicsview.h"
#include <QScrollArea>
#include <QScrollBar>

class GraphicsViewArea : public QScrollArea
{
    Q_OBJECT

public:
    GraphicsViewArea();
    ~GraphicsViewArea();

    // Getters
    GraphicsView* view() { return mView; }

    // Setters
    void setView(const QImage& _img);
    void setView(const QSize& _size);
    void setView(int _width, int _height);

    void updateScrollArea();

private:
    void adjustScrollBar(QScrollBar *scrollBar, double _factor);

    GraphicsView* mView;
};

#endif // GRAPGICSVIEWAREA_H
