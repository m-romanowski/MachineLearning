#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>

#include <QString>
#include <QDebug>

class Util
{
public:
    Util();

    static std::size_t findStr(const std::string& _input, const std::string& _toFind);
    static void genImageBySeed(const QString& _seed, int _width, int _height,
        const QString& _format = "png");
    static std::vector<bool*> randomNoise(bool* _input, size_t _size, size_t _count = 1,
        unsigned int _probability = 10);

    template <typename T>
    static T* to1D(T** _arr, int _width, int _height)
    {
        T* result = new T[_width * _height];

        for(int i = 0; i < _width; i++)
            for(int j = 0; j < _height; j++)
                result[_width * i + _height] = _arr[i][j];

        return result;
    }

private:
    static bool isEqual(bool* _input1, bool* _input2, size_t _size);
};

#endif // UTIL_H
