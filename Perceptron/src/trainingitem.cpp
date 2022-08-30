#include "trainingitem.h"

TrainingItem::TrainingItem()
    : data(nullptr)
{

}

TrainingItem::~TrainingItem()
{
    if(data != nullptr)
        delete data;
}

void TrainingItem::toBits(QImage* _image)
{
    unsigned char *ptr = _image->bits();
    int width = _image->width();
    int height = _image->height();

    size = width * height;
    data = new bool[size];

    for(int i = 0; i < size; i++)
        data[i] = 0;

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            float r = ptr[width * 4 * y + 4 * x];
            float g = ptr[width * 4 * y + 4 * x + 1];
            float b = ptr[width * 4 * y + 4 * x + 2];

            if(r == 0.0f && g == 0.0f && b == 0.0f)
                data[y * width + x] = 1;
        }
    }
}
