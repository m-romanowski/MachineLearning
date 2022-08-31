#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <complex>
#include <cassert>
#include <variant>

#define assertm(exp, msg) assert(((void)msg, exp))

class Util
{
public:
    Util();

    static std::size_t findStr(const std::string& _input, const std::string& _toFind);
    static std::vector<std::complex<double>> DFT(const std::vector<std::complex<double>>& _input);
    static std::vector<std::complex<double>> DFT(const std::vector<double>& _inReal, const std::vector<double>& _inImag);
    static std::vector<double> DFTRealValuesVector(const std::vector<std::complex<double>>& _input);
    static std::vector<double> DFTImaginaryValuesVector(const std::vector<std::complex<double>>& _input);
    static double* DFTRealValues(const std::vector<std::complex<double>>& _input);
    static double* DFTImaginaryValues(const std::vector<std::complex<double>>& _input);

    template <typename T>
    static std::vector<T*> randomNoise(T* _input, size_t _size, size_t _count = 1,
        unsigned int _probability = 10)
    {
        srand(static_cast<unsigned>(time(nullptr)));

        std::vector<T*> result;

        for(size_t i = 0; i < _count; ++i)
        {
            T* tmp = new T[_size];
            memcpy(tmp, _input, _size * sizeof(T));

            for(size_t j = 0; j < _size; ++j)
            {
                // % to change pixel to opposite color
                bool probability = (rand() % 100) < _probability;

                if(std::is_same_v<T, bool>)
                {
                    if(probability)
                        tmp[j] = !tmp[j];
                }
                else
                {
                    if(probability)
                    {
                        if(tmp[j] == 1) tmp[j] = 0;
                        else if(tmp[j] == 0) tmp[j] = 1;
                    }
                }
            }

            // If arrays are not same add a new item.
            if(!Util::isEqual(tmp, _input, _size))
                result.push_back(tmp);
        }

        return result;
    }

    template <typename T>
    static std::vector<std::complex<double>> toComplex(T* _inReal, T* _inImag, size_t _size)
    {
        std::vector<std::complex<double>> output;
        for(size_t i = 0; i < _size; i++)
            output.push_back(std::complex<double>(static_cast<double>(_inReal[i]),
                static_cast<double>(_inImag[i])));

        return output;
    }

    template <typename T>
    static std::vector<std::complex<double>> toComplex(T* _inReal, size_t _size)
    {
        std::vector<std::complex<double>> output;
        for(size_t i = 0; i < _size; i++)
            output.push_back(std::complex<double>(static_cast<double>(_inReal[i]), 0.0));

        return output;
    }

    template <typename T>
    static std::vector<std::complex<double>> toComplex(const std::vector<T>& _inReal, const std::vector<T>& _inImag)
    {
        std::vector<std::complex<double>> output;
        for(size_t i = 0; i < _inReal.size(); i++)
            output.push_back(std::complex<double>(static_cast<double>(_inReal[i]),
                static_cast<double>(!_inImag.empty() ? _inImag[i] : 0.0)));

        return output;
    }

    template <typename T, typename U>
    T* arrToType(U* _data, size_t _size)
    {
        T* output = new T[_size];

        for(size_t i = 0; i < _size; i++)
            output[i] = static_cast<T>(_data[i]);

        return output;
    }

    template <typename T>
    static T* to1D(T** _arr, int _width, int _height)
    {
        T* result = new T[_width * _height];

        for(int i = 0; i < _width; i++)
            for(int j = 0; j < _height; j++)
                result[_width * i + _height] = _arr[i][j];

        return result;
    }

    template <typename T>
    static bool isEqual(T* _input1, T* _input2, size_t _size)
    {
        for(size_t i = 0; i < _size; ++i)
        {
            if(_input1[i] != _input2[i])
                return false;
        }

        return true;
    }
};

#endif // UTIL_H
