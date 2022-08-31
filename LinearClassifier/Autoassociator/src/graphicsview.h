#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QLabel>

class GraphicsView : public QLabel
{
    Q_OBJECT

public:
    explicit GraphicsView(QWidget* _parent = nullptr);
    explicit GraphicsView(const QSize& _size, QWidget* _parent = nullptr);
    explicit GraphicsView(const QImage& _image, QWidget* _parent = nullptr);
    GraphicsView(int _width, int _height, QWidget* _parent = nullptr);

    // Getters
    [[nodiscard]] QImage image() const { return mImage; }
    QImage* imagePtr() { return &mImage; }
    [[nodiscard]] QPoint mousePos() const { return mMousePos; }
    [[nodiscard]] QColor penColor() const { return mPenColor; }
    [[nodiscard]] int penWeight() const { return mPenWeight; }
    [[nodiscard]] int imageWidth() const { return mImage.width(); }
    [[nodiscard]] int imageHeight() const { return mImage.height(); }
    [[nodiscard]] QSize imageSize() const { return {mImage.width(), mImage.height()}; }
    [[nodiscard]] unsigned int image1DSize() const { return static_cast<unsigned int>(mImage.width() * mImage.height()); }
    [[nodiscard]] bool resizeState() const { return mBlockResize; }
    [[nodiscard]] double scaleFactor() const { return mScaleFactor; }

    // Setters
    void loadImage(const QImage& _newImage);
    void loadImage(const QSize& _size);
    void setPenWeight(int _weight) { mPenWeight = _weight; }
    void setPenColor(const QColor& _color) { mPenColor = _color; }
    void setResizeState(bool _state) { mBlockResize = _state; }

    bool* bitmap();
    void updateImage(bool* _bitmap);
    void updateView();
    static void randomNoise(GraphicsView* _view, int _probability = 10);
    static bool* bitmap(QImage* _image);
    static void clearImage(QImage* _img);
    static void toBitmapImage(QImage* _image, const bool* _bitmap);
    static void scaleImage(const QImage& _image, int _width, int _height, bool _keepRatio = true);
    static void scaleImage(const QImage& _image, QSize _size, bool _keepRatio = true);

signals:
    void mouseMoved(QPoint _pos);

public slots:
    void zoomIn();
    void zoomOut();
    void normalSize();

private slots:
    void paintEvent(QPaintEvent* _event);
    void mousePressEvent(QMouseEvent* _event);
    void mouseReleaseEvent(QMouseEvent* _event);
    void mouseMoveEvent(QMouseEvent* _event);
    void wheelEvent(QWheelEvent* _event); // TODO: QImage zoom

private:
    void init();
    void draw(QPoint _pos);
    void scaleImage(double _factor = 1.0);

    QImage mImage;
    QPoint mMousePos;
    QColor mPenColor;

    int mPenWeight;
    bool mDrawing, mBlockResize;
    double mScaleFactor;
};

#endif // GRAPHICSVIEW_H
