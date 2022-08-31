#include "pixelgrid.h"

PixelGrid::PixelGrid(int _width, int _height, QWidget* _parent)
    : QGridLayout(_parent),
      mWidth(_width),
      mHeight(_height)
{
    this->setSpacing(1);

    mGrid = new bool*[_width];
    mButtons.resize(static_cast<unsigned>(_width));

    for(unsigned int i = 0; i < static_cast<unsigned>(_width); i++)
    {
        mGrid[i] = new bool[_height];
        mButtons[i].resize(static_cast<unsigned>(_height));
    }

    for(int i = 0; i < _width; i++)
        for(int j = 0; j < _height; j++)
            mGrid[i][j] = 0;

    createGUIGrid();
}

PixelGrid::~PixelGrid()
{
    if(!mButtons.empty())
    {
        for(unsigned int i = 0; i < mButtons.size(); i++)
        {
            for(unsigned int j = 0; j < mButtons[i].size(); j++)
            {
                delete mButtons[i][j];
            }
        }
    }
}

void PixelGrid::createGUIGrid()
{
    for(int i = 0; i < mWidth; i++)
    {
        for(int j = 0; j < mHeight; j++)
        {
            auto* button = new QPushButton;
            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            button->setStyleSheet("QPushButton { background-color: white; }");
            connect(button, &QPushButton::clicked, [this, i, j]() {
                onClick(i, j);
            });
            this->addWidget(button, i, j);

            mButtons[static_cast<unsigned>(i)][static_cast<unsigned>(j)] = button;
        }
    }
}

QPushButton* PixelGrid::getButtonById(int _x, int _y)
{
    return mButtons[static_cast<unsigned long>(_x)][static_cast<unsigned long>(_y)];
}

void PixelGrid::clearGrid()
{
    for(unsigned int i = 0; i < static_cast<unsigned int>(mWidth); i++)
    {
        for(unsigned int j = 0; j < static_cast<unsigned int>(mHeight); j++)
        {
            mButtons[i][j]->setStyleSheet("QPushButton { background-color: white; }");
            mGrid[i][j] = false;
        }
    }
}

void PixelGrid::onClick(int _x, int _y)
{
    // Change value true / false in the bit map array
    mGrid[_x][_y] = !mGrid[_x][_y];

    // Change the button color.
    if(mGrid[_x][_y])
        getButtonById(_x, _y)->setStyleSheet("QPushButton { background-color: black; }");
    else
        getButtonById(_x, _y)->setStyleSheet("QPushButton { background-color: white; }");

    emit gridUpdated();
}
