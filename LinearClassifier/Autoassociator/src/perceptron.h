#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include "trainingitem.h"

#include <utility>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <string>

class Perceptron
{
public:
    Perceptron();
    Perceptron(int _width, int _height);

    // Getters
    std::vector<float>& weights() { return mWeights; }

    // Setters
    void setWeights(std::vector<float> _weights) { mWeights = std::move(_weights); }

    [[nodiscard]] std::string toString() const;

private:
    float fRand(float _fMin, float _fMax); // Random number between _fMin and _fMax.

    std::vector<float> mWeights; // Training weights.
    float bias;
};

#endif // PERCEPTRON_H
