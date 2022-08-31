#ifndef ADALINE_H
#define ADALINE_H

#include "trainingitem.h"
#include "util.h"

#include <utility>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cassert>
#include <cmath>

#define DEFAULT_WIDTH 5
#define DEFAULT_HEIGHT 5

// Reference: https://en.wikipedia.org/wiki/ADALINE
class Adaline
{
public:
    explicit Adaline(int _width = DEFAULT_WIDTH, int _height = DEFAULT_HEIGHT, double _learningRate = 0.2,double _errorTolerance = 0.1);

    // Getters
    [[nodiscard]] std::vector<double> weights() const { return mWeights; }
    [[nodiscard]] std::vector<double> errors() const { return mErrors; }
    [[nodiscard]] double bias() const { return mBias; }
    [[nodiscard]] double errorTolerance() const { return mErrorTolerance; }

    // Setters
    void setWeights(std::vector<double> _weights) { mWeights = std::move(_weights); }
    void setLearningRate(double _learningRate) { mLearningRate = _learningRate; }
    void setBias(double _bias) { mBias = _bias; }
    void setErrorTolerance(double _error) { mErrorTolerance = _error; }

    void learn(std::vector<TrainingItem*>& _trainingItems, size_t _perceptronId, int _epochs);
    int predict(double* _data);
    [[nodiscard]] std::string toString(const std::vector<double> _input) const;

private:
    double fRand(double _fMin, double _fMax);
    double sum(double* _input, std::vector<double> _weights);
    void updateWeights(double* _learningData, double _diff, std::vector<double>& _weights);
    double computeError(double* _learningData, double _expectedData, std::vector<double>& _weights);
    int activate(double _sum);

    double mLearningRate, mBias, mErrorTolerance;
    std::vector<double> mWeights; // Training weights
    std::vector<double> mErrors; // Misclassifications
};

#endif // ADALINE_H
