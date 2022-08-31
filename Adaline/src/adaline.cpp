#include "adaline.h"

Adaline::Adaline(int _width, int _height, double _learningRate, double _errorTolerance)
    : mLearningRate(_learningRate),
      mBias(fRand(0, 0.5)),
      mErrorTolerance(_errorTolerance),
      mWeights(static_cast<size_t>(_width * _height), 0.0)
{
    srand(static_cast<unsigned>(time(nullptr)));

    // Starting (random) weights values.
    for(size_t i = 0; i < mWeights.size(); i++)
        mWeights[i] = fRand(0, 0.5);
}

/* Returns random value from <a; b>.
 */
double Adaline::fRand(double _fMin, double _fMax)
{
    double f = static_cast<double>(rand()) / RAND_MAX;
    return _fMin + f * (_fMax - _fMin);
}

/* Sum all weights.
 */
double Adaline::sum(double* _input, std::vector<double> _weights)
{
    double result = 0.0;

    for(unsigned int i = 0; i < _weights.size(); ++i)
        result += _input[i] * _weights[i];

    return result;
}

/* Update all neuron weights if was misclassification.
 */
void Adaline::updateWeights(double* _learningData, double _diff, std::vector<double>& _weights)
{
    for(size_t i = 0; i < _weights.size(); i++)
        // _diff = expected - netInput
        _weights[i] += mLearningRate * _diff * _learningData[i];
}

/* Train neuron on given data.
 */
void Adaline::learn(std::vector<TrainingItem*>& _trainingItems, size_t _perceptronId, int _epochs)
{
    for(int i = 0; i < _epochs; i++)
    {
        double err = 0;
        for(size_t j = 0; j < _trainingItems.size(); j++)
        {
            double netInput = sum(_trainingItems[j]->data(), mWeights);
            bool expected = j == _perceptronId ? 1 : 0;

            // Update
            updateWeights(_trainingItems[j]->data(), expected - activate(netInput), mWeights);

            // Compute error
            err += computeError(_trainingItems[j]->data(), expected, mWeights);
        }

        mErrors.push_back(err);
    }
}

/* Compute number of misclassification in very epoch.
 */
double Adaline::computeError(double* _learningData, double _expectedData, std::vector<double>& _weights)
{
    double netInput = sum(_learningData, _weights);
    return pow(_expectedData - netInput, 2);
}

int Adaline::activate(double _sum)
{
    if(_sum >= 0)
        return 1;

    return 0;
}

int Adaline::predict(double* _data)
{
    return activate(sum(_data, mWeights));
}

/* Weights in string reperesentation.
 */
std::string Adaline::toString(const std::vector<double> _input) const
{
    std::string result;

    for(auto& input : _input)
        result.append(std::to_string(input) + " ");

    return result;
}
