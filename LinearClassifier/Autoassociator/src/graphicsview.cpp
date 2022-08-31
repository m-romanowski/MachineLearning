#include "graphicsview.h"

GraphicsView::GraphicsView(QWidget* _parent)
    : QLabel(_parent)
{
    init();
}

GraphicsView::GraphicsView(const QSize& _size, QWidget* _parent)
    : QLabel(_parent),
      mImage(QImage(_size, QImage::Format_RGB32))
{
    init();
}

GraphicsView::GraphicsView(const QImage& _image, QWidget* _parent)
    : QLabel (_parent),
      mImage(_image)
{
    init();
}

GraphicsView::GraphicsView(int _width, int _height, QWidget* _parent)
    : QLabel(_parent),
      mImage(QImage(_width, _height, QImage::Format_RGB32))
{
    init();
}

void GraphicsView::init()
{
    this->setMouseTracking(true);
    this->setBackgroundRole(QPalette::Base);
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    this->setScaledContents(true);

    mPenColor = QColor(0, 0, 0);
    mPenWeight = 10;
    mDrawing = false;
    mBlockResize = true;
}

void GraphicsView::paintEvent(QPaintEvent* _event)
{
    QLabel::paintEvent(_event);

    QPainter painter(this);
    QRect dirtyRect = _event->rect();
    painter.drawImage(dirtyRect, mImage, dirtyRect);
}

void GraphicsView::draw(QPoint _pos)
{
    QPainter painter(&mImage);
    painter.setPen(QPen(mPenColor, mPenWeight, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(mMousePos, _pos);

    int rad = (mPenWeight / 2) + 2;
    update(QRect(mMousePos, _pos).normalized().adjusted(-rad, -rad, +rad, +rad));

    mMousePos = _pos;
}

void GraphicsView::clearImage(QImage* _img)
{
    _img->fill(Qt::white);
}

void GraphicsView::mousePressEvent(QMouseEvent* _event)
{
    if(_event->button() == Qt::LeftButton)
    {
        // We need to draw one pixel per click (main painter function drawing a line between
        // current and previous mouse position) - e.g. if we need paint pixel(s) per click.
        QPainter painter(&mImage);
        painter.setPen(QPen(mPenColor, mPenWeight, Qt::SolidLine, Qt::RoundCap));
        painter.drawPoint(_event->pos());

        mMousePos = _event->pos();
        mDrawing = true;
    }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent* _event)
{
    if(_event->button() == Qt::LeftButton)
    {
        draw(_event->pos());
        mDrawing = false;
    }
}

void GraphicsView::mouseMoveEvent(QMouseEvent* _event)
{
    if((_event->buttons() == Qt::LeftButton) && mDrawing)
        draw(_event->pos());

    if(_event->pos().x() >= 0 && _event->pos().x() <= mImage.width() &&
        _event->pos().y() >= 0 && _event->pos().y() <= mImage.height())
    {
        mMousePos = _event->pos();
        emit mouseMoved(_event->pos());
    }
}

void GraphicsView::wheelEvent(QWheelEvent* _event)
{
    if(_event->angleDelta().y() > 0)
        scaleImage(1.25);
    else
        scaleImage(0.8);
}

void GraphicsView::zoomIn()
{
    scaleImage(1.25);
}

void GraphicsView::zoomOut()
{
    scaleImage(0.8);
}

void GraphicsView::normalSize()
{
    this->adjustSize();
    mScaleFactor = 1.0;
}

void GraphicsView::scaleImage(double _factor)
{
    Q_ASSERT(this->pixmap());
    mScaleFactor *= _factor;
    this->resize(_factor * this->pixmap()->size());

    QImage scaledImage = mImage.scaled(this->size(), Qt::IgnoreAspectRatio);
    mImage = scaledImage;
    this->setPixmap(QPixmap::fromImage(mImage));
}

void GraphicsView::loadImage(const QImage& _newImage)
{
    mImage = _newImage;
    this->setPixmap(QPixmap::fromImage(mImage));
    mScaleFactor = 1.0;
    scaleImage();
}

void GraphicsView::loadImage(const QSize& _size)
{
    QImage newImage = QImage(_size, QImage::Format_RGB32);
    clearImage(&newImage);

    mImage = newImage;
    this->setPixmap(QPixmap::fromImage(mImage));
    mScaleFactor = 1.0;
    scaleImage();
}

void GraphicsView::updateImage(bool* _bitmap)
{
    GraphicsView::toBitmapImage(&mImage, _bitmap);
    update();
}

bool* GraphicsView::bitmap()
{
    return GraphicsView::bitmap(&mImage);
}

void GraphicsView::updateView()
{
    update();
}

bool* GraphicsView::bitmap(QImage* _image)
{
    unsigned char *ptr = _image->bits();
    int width = _image->width();
    int height = _image->height();

    int size = width * height;
    bool* data = new bool[size];

    for(int i = 0; i < size; i++)
        data[i] = false;

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            float r = ptr[width * 4 * y + 4 * x];
            float g = ptr[width * 4 * y + 4 * x + 1];
            float b = ptr[width * 4 * y + 4 * x + 2];

            if(r != 255.0f && g != 255.0f && b != 255.0f)
                data[y * width + x] = true;
        }
    }

    return data;
}

void GraphicsView::toBitmapImage(QImage* _image, const bool* _bitmap)
{
    unsigned char* ptr = _image->bits();
    int width = _image->width();
    int height = _image->height();

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            if(_bitmap[y * width + x])
            {
                ptr[width * 4 * y + 4 * x] = 1;
                ptr[width * 4 * y + 4 * x + 1] = 1;
                ptr[width * 4 * y + 4 * x + 2] = 1;
            }
            else
            {
                ptr[width * 4 * y + 4 * x] = 255;
                ptr[width * 4 * y + 4 * x + 1] = 255;
                ptr[width * 4 * y + 4 * x + 2] = 255;
            }
        }
    }
}

void GraphicsView::randomNoise(GraphicsView *_view, int _probability)
{
    srand(static_cast<unsigned>(time(nullptr)));
    bool* result = _view->bitmap();

    for(unsigned int i = 0; i < _view->image1DSize(); i++)
    {
        // Probability to change to opposite color (e.g. white to black).
        int probability = (rand() % 100) < _probability;

        if(probability)
            result[i] = !result[i];
    }

    _view->updateImage(result);
}

void GraphicsView::scaleImage(const QImage& _image, int _width, int _height, bool _keepRatio)
{
    if(_keepRatio)
        _image.scaled(_width, _height, Qt::KeepAspectRatio);

    _image.scaled(_width, _height, Qt::IgnoreAspectRatio);
}

void GraphicsView::scaleImage(const QImage& _image, QSize _size, bool _keepRatio)
{
    if(_keepRatio)
        _image.scaled(_size, Qt::KeepAspectRatio);

    _image.scaled(_size, Qt::IgnoreAspectRatio);
}
