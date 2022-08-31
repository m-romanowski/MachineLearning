#ifndef PIXELGRID_H
#define PIXELGRID_H

#include <QGridLayout>
#include <QPushButton>
#include <vector>

class PixelGrid : public QGridLayout
{
    Q_OBJECT

public:
    PixelGrid(int _width, int _height, QWidget* _parent = nullptr);
    ~PixelGrid();

    int gridWidth() const { return mWidth; }
    int gridHeight() const { return mHeight; }
    int gridSize() const { return mWidth * mHeight; }
    bool** grid() { return mGrid; }
    QPushButton* getButtonById(int _x, int _y);

    void clearGrid();

signals:
    void gridUpdated();

protected:
    void onClick(int _x, int _y);

private:
    void createGUIGrid();

    std::vector<std::vector<QPushButton*>> mButtons;
    bool** mGrid;
    int mWidth;
    int mHeight;
};

#endif // PIXELGRID_H
