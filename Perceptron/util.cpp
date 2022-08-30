#include "util.h"

Util::Util()
{

}

std::size_t Util::findStr(const std::string& _input, const std::string& _toFind)
{
    std::size_t found = _input.find(_toFind);

    /*if(found == std::string::npos)
        throw std::invalid_argument("Cannot find given string");*/

    return found;
}

// See: ImageGen.
void Util::genImageBySeed(const QString& _seed, int _width, int _height,
    const QString& _format)
{
    Q_UNUSED(_seed);
    Q_UNUSED(_width);
    Q_UNUSED(_height);
    Q_UNUSED(_format);
}

std::vector<bool*> Util::randomNoise(bool* _input, size_t _size, size_t _count,
    unsigned int _probability)
{
    srand(static_cast<unsigned>(time(nullptr)));

    std::vector<bool*> result;

    for(size_t i = 0; i < _count; ++i)
    {
        bool* tmp = new bool[_size];
        memcpy(tmp, _input, _size * sizeof(bool));

        for(size_t j = 0; j < _size; ++j)
        {
            // % to change pixel to opposite color
            bool probability = (rand() % 100) < _probability;

            if(probability)
                tmp[j] = !tmp[j];
        }

        // If arrays are not same add a new item.
        if(!Util::isEqual(tmp, _input, _size))
            result.push_back(tmp);
    }

    return result;
}

bool Util::isEqual(bool* _input1, bool* _input2, size_t _size)
{
    for(size_t i = 0; i < _size; ++i)
    {
        if(_input1[i] != _input2[i])
            return false;
    }

    return true;
}
