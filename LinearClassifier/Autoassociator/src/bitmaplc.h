#ifndef BITMAPLC_H
#define BITMAPLC_H

#include "linearclassifier.h"
#include <exception>

class BitmapLC : public LinearClassifier
{
public:
    BitmapLC(unsigned int id, int _width, int _height, int _count);
    bool check(bool* _data, unsigned int _size);
};

#endif // BITMAPLC_H
