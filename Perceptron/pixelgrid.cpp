#include "pixelgrid.h"

PixelGrid::PixelGrid(int _width, int _height, QWidget* _parent)
    : QGridLayout(_parent),
      width(_width),
      height(_height)
{
    this->setSpacing(1);

    grid = new bool*[_width];
    buttons.resize(static_cast<unsigned long>(_width));

    for(int i = 0; i < _width; i++)
    {
        grid[i] = new bool[_height];
        buttons[static_cast<unsigned long>(i)].resize(static_cast<unsigned long>(_height));
    }

    for(int i = 0; i < _width; i++)
        for(int j = 0; j < _height; j++)
            grid[i][j] = 0;

    createGUIGrid();
}

PixelGrid::~PixelGrid()
{
    if(buttons.size() > 0)
    {
        for(unsigned int i = 0; i < buttons.size(); i++)
        {
            for(unsigned int j = 0; j < buttons[i].size(); j++)
            {
                delete buttons[i][j];
            }
        }
    }
}

void PixelGrid::createGUIGrid()
{
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            QPushButton* button = new QPushButton;
            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            button->setStyleSheet("QPushButton { background-color: white; }");
            connect(button, &QPushButton::clicked, [this, i, j]() {
                onClick(i, j);
            });
            this->addWidget(button, i, j);

            buttons[static_cast<unsigned long>(i)][static_cast<unsigned long>(j)] = button;
        }
    }
}

QPushButton* PixelGrid::getButtonById(int _x, int _y)
{
    return buttons[static_cast<unsigned long>(_x)][static_cast<unsigned long>(_y)];
}

void PixelGrid::clearGrid()
{
    for(unsigned int i = 0; i < static_cast<unsigned int>(width); i++)
    {
        for(unsigned int j = 0; j < static_cast<unsigned int>(height); j++)
        {
            buttons[i][j]->setStyleSheet("QPushButton { background-color: white; }");
            grid[i][j] = 0;
        }
    }
}

void PixelGrid::onClick(int _x, int _y)
{
    // Change value true / false in the bit map array
    grid[_x][_y] = !grid[_x][_y];

    // Change the button color.
    if(grid[_x][_y])
        getButtonById(_x, _y)->setStyleSheet("QPushButton { background-color: black; }");
    else
        getButtonById(_x, _y)->setStyleSheet("QPushButton { background-color: white; }");

    emit gridUpdated();
}

void PixelGrid::reloadGUI()
{

}
