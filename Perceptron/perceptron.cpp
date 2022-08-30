#include "perceptron.h"

Perceptron::Perceptron(float _learningConst)
    : learningConst(_learningConst),
      done(false),
      lifeOfTime(0)
{
    weights.resize(MAX_WIDTH * MAX_HEIGHT);

    // Starting (random) weights values.
    for(auto& weight : weights)
        weight = fRand(0, 0.5);

    // Starting (random) theta.
    theta = fRand(0, 0.5);
}

/* Returns random value <a; b>
 */
float Perceptron::fRand(float _fMin, float _fMax)
{
    srand(static_cast<unsigned>(time(nullptr)));

    return _fMin + static_cast<float>(rand()) /
        (static_cast<float>(RAND_MAX/(_fMax - _fMin)));
}

int Perceptron::activate(float _sum, float _theta)
{
    if(_sum >= _theta)
        return 1;

    return -1;
}

/* Sum all weights.
 */
float Perceptron::sum(bool* _input, std::vector<float> _weights)
{
    float result = 0.0;

    for(unsigned int i = 0; i < MAX_WIDTH * MAX_HEIGHT; ++i)
        result += static_cast<int>(_input[i]) * _weights[i];

    return result;
}

/* Update weights.
 */
void Perceptron::update(bool* _input, float _err)
{
    for(unsigned int i = 0; i < MAX_WIDTH * MAX_HEIGHT; ++i)
        weights[i] += learningConst * _err * static_cast<int>(_input[i]);
}

/* Simple Perceptron Learning Algorithm
 */
void Perceptron::trainSPLA(bool* _input, int _expected)
{
    int it = 0;

    while(it < MAX_LOOP_IT || !done)
    {
        float O = activate(sum(_input, weights), theta);
        float err = _expected - O;

        if(err == 0.0f)
        {
            done = true;
            return;
        }

        // Update weights.
        update(_input, err);

        // Update theta.
        theta -= learningConst * err;

        it++;
    }
}

/* Pocket Learning Algorithm
 */
void Perceptron::trainPLA(bool* _input, int _expected)
{
    while(!done)
    {
        float O = activate(sum(_input, weights), theta);
        float err = _expected - O;

        if(err == 0.0f)
        {
            lifeOfTime++;

            // Update pocket
            if(lifeOfTime > pocket.lifeOfTime)
            {
                pocket.lifeOfTime = lifeOfTime;
                pocket.weights = weights;
            }

            done = true;
            return;
        }

        // Update data
        update(_input, err);
        theta -= learningConst * err;
        lifeOfTime = 0;
    }
}

int Perceptron::check(bool* _input)
{
    return activate(sum(_input, weights), theta);
}

std::string Perceptron::toString() const
{
    std::string result;

    for(auto& weight : weights)
        result.append(std::to_string(static_cast<double>(weight)) + ",");

    return result;
}
