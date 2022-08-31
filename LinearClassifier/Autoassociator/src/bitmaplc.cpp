#include "bitmaplc.h"

BitmapLC::BitmapLC(unsigned int _id, int _width, int _height, int _count)
    : LinearClassifier (_id, _width, _height, _count)
{

}

bool BitmapLC::check(bool* _data, unsigned int _size)
{
    float sum0 = sum(_data, _size, mPocket.weights[0]);
    float sum1 = sum(_data, _size, mPocket.weights[1]);

    if(sum1 > sum0)
        return true; // Return black color.

    return false; // Return white color.
}
