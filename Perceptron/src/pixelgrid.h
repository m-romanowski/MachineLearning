#ifndef PIXELGRID_H
#define PIXELGRID_H

#include <QGridLayout>
#include <QPushButton>
#include <QDebug>
#include <vector>

class PixelGrid : public QGridLayout
{
    Q_OBJECT

public:
    PixelGrid(int _width, int _height, QWidget* _parent = nullptr);
    ~PixelGrid();

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getGridSize() const { return width * height; }
    bool** getGrid() { return grid; }
    QPushButton* getButtonById(int _x, int _y);

    void clearGrid();

signals:
    void gridUpdated();

protected:
    void onClick(int _x, int _y);

private:
    void createGUIGrid();
    void reloadGUI();

    std::vector<std::vector<QPushButton*>> buttons;
    bool** grid;
    int width;
    int height;
};

#endif // PIXELGRID_H
