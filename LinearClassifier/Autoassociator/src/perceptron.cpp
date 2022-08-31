#include "perceptron.h"

Perceptron::Perceptron()
{

}

Perceptron::Perceptron(int _width, int _height)
    : mWeights(static_cast<unsigned>(_width * _height), 0.0f)
{
    srand(static_cast<unsigned>(time(nullptr)));

    // Starting (random) weights values.
    for(auto& weight : mWeights)
        weight = fRand(0, 0.5);
}

/* Returns random value from <a; b>.
 */
float Perceptron::fRand(float _fMin, float _fMax)
{
    float f = static_cast<float>(rand()) / RAND_MAX;
    return _fMin + f * (_fMax - _fMin);
}

std::string Perceptron::toString() const
{
    std::string result;

    for(auto& weight : mWeights)
        result.append(std::to_string(static_cast<double>(weight)) + " ");

    return result;
}
