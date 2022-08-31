#include "trainingitem.h"

TrainingItem::TrainingItem()
    : data(nullptr)
{

}

TrainingItem::TrainingItem(bool* _data, unsigned int _size)
    : data(_data),
      size(_size)
{

}

TrainingItem::~TrainingItem()
{
    delete data;
}

bool* TrainingItem::toBits(QImage* _image)
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

            if(r == 0.0f && g == 0.0f && b == 0.0f)
                data[y * width + x] = true;
        }
    }

    return data;
}
