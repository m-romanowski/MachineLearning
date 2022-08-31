#ifndef LC_H
#define LC_H

#include "perceptron.h"
#include "trainingitem.h"

#include <vector>

/* Multi-class (linear) classifier
 *
 * Input
 *  E[i]: Image bitmap (1, 0 values)
 *  P[m]: Perceptron per linear classifier
 *
 * Types
 *  id: linear classifier id
 *  pid: index of perceptron max sum
 *  ANSWER: perceptron value (sum)
 *  CORRECT: expected value
 *
 * Pre-computing
 *  weights = random(0, 1)
 *
 * (1) Random input E
 * (2) Learn(E, T)
 *         foreach perceptron in P
 *             sums += E[i] * weights[i];
 *
 *         foreach sum in sums
 *             calculate max(sums)
 *
 *         pid = perceptron index of max(sums)
 *
 *         if pid != E[id] then
 *             ANSWER: weights[i] -= E[i]
 *             CORRECT: weights[i] += E[i]
 *
 * (3) Next random input and back to step (1)
 */

enum DATA_TYPE
{
    ANSWER,
    CORRECT
};

struct Pocket
{
    Pocket()
    {
        lifeTime = 0;
    }

    int lifeTime;
    std::map<unsigned int, std::vector<float>> weights;
};

class LinearClassifier
{
public:
    LinearClassifier(unsigned int _id, int _width, int _height, int _count);
    ~LinearClassifier();

    [[nodiscard]] std::vector<Perceptron*> perceptrons() const { return mPerceptrons; }
    void addPerceptron(Perceptron* _perceptron) { mPerceptrons.push_back(_perceptron); }
    void learn(TrainingItem* _item);

protected:
    template <typename T>
    float sum(T* _data, unsigned int _size, std::vector<float> _weights)
    {
        float sum = 0.0f;

        for(unsigned int i = 0; i < _size; i++)
            sum += _data[i] * _weights[i];

        return sum;
    }

    Pocket mPocket;
    std::vector<Perceptron*> mPerceptrons;

private:
    template <typename T>
    void updateWeights(T* _data, unsigned int _size, std::vector<float>& _weights,
        DATA_TYPE _type)
    {
        for(unsigned int i = 0; i < _size; i++)
        {
            if(_type == DATA_TYPE::ANSWER)
                _weights[i] -= _data[i];
            else // DATA_TYPE::CORRECT
                _weights[i] += _data[i];
        }
    }

    unsigned int mId;
    int mLifeTime;
};

#endif // LC_H
