#include "util.h"

Util::Util()
{

}

size_t Util::findStr(const std::string& _input, const std::string& _toFind)
{
    return _input.find(_toFind);
}

std::vector<std::complex<double>> Util::DFT(const std::vector<std::complex<double>>& _input)
{
    std::vector<std::complex<double>> output;

    for(size_t i = 0; i < _input.size(); i++)
    {
        double sumReal = 0, sumImag = 0;
        for(size_t j = 0; j < _input.size(); j++)
        {
            double angle = 2 * M_PI * j * i / _input.size();
            sumReal = _input[j].real() * cos(angle) + _input[j].imag() * sin(angle);
            sumImag = -_input[j].real() * sin(angle) + _input[j].imag() * cos(angle);
        }

        output.push_back(std::complex(sumReal, sumImag));
    }

    return output;
}

std::vector<std::complex<double>> Util::DFT(const std::vector<double>& _inReal, const std::vector<double>& _inImag)
{
    assertm(_inReal.size() == _inImag.size(), "Vectors must be equal");

    std::vector<std::complex<double>> _complexInput;
    for(size_t i = 0; i < _inReal.size(); i++)
        _complexInput.push_back(std::complex<double>(_inReal[i], _inImag[i]));

    return Util::DFT(_complexInput);
}

std::vector<double> Util::DFTRealValuesVector(const std::vector<std::complex<double>>& _input)
{
    std::vector<double> output;
    for(auto& item: _input)
        output.push_back(item.real());

    return output;
}

double* Util::DFTRealValues(const std::vector<std::complex<double>>& _input)
{
    auto* output = new double[_input.size()];
    for(size_t i = 0; i < _input.size(); i++)
        output[i] = _input[i].real();

    return output;
}

std::vector<double> Util::DFTImaginaryValuesVector(const std::vector<std::complex<double>>& _input)
{
    std::vector<double> output;
    for(auto& item: _input)
        output.push_back(item.imag());

    return output;
}

double* Util::DFTImaginaryValues(const std::vector<std::complex<double>>& _input)
{
    auto* output = new double[_input.size()];
    for(size_t i = 0; i < _input.size(); i++)
        output[i] = _input[i].real();

    return output;
}
