#include "trainingitem.h"

TrainingItem::TrainingItem()
    : mData(nullptr)
{

}

TrainingItem::~TrainingItem()
{
    delete mData;
}

std::vector<double> TrainingItem::dataVector()
{
    std::vector<double> output;

    for(int i = 0; i < mSize; i++)
        output.push_back(mData[i]);

    return output;
}

void TrainingItem::setData(std::vector<double>& _data)
{
    mData = new double[_data.size()];

    for(size_t i = 0; i < _data.size(); i++)
        mData[i] = _data[i];
}

void TrainingItem::toBits(QImage* _image)
{
    unsigned char *ptr = _image->bits();
    int width = _image->width();
    int height = _image->height();

    mSize = width * height;
    mData = new double[static_cast<unsigned>(mSize)];

    for(int i = 0; i < mSize; i++)
        mData[i] = 0;

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            float r = ptr[width * 4 * y + 4 * x];
            float g = ptr[width * 4 * y + 4 * x + 1];
            float b = ptr[width * 4 * y + 4 * x + 2];

            if(r != 255.0f && g != 255.0f && b != 255.0f)
                mData[y * width + x] = 1;
        }
    }
}
