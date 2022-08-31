#include "linearclassifier.h"

LinearClassifier::LinearClassifier(unsigned int _id, int _width, int _height, int _count)
    : mId(_id),
      mLifeTime(-1)
{
    for(unsigned int i = 0; i < static_cast<unsigned int>(_count); i++)
    {
        mPerceptrons.push_back(new Perceptron(_width, _height));
        mPocket.weights[i] = mPerceptrons[i]->weights();
    }
}

LinearClassifier::~LinearClassifier()
{
    for (auto perceptron : mPerceptrons)
        delete perceptron;

    mPerceptrons.clear();
}

void LinearClassifier::learn(TrainingItem* _item)
{
    size_t size = mPerceptrons.size();
    auto* sums = new float[size];

    float max = -1.0f;
    unsigned int perceptronNum = 0;
    for(unsigned int j = 0; j < size; j++)
    {
        sums[j] = sum(_item->getData(), _item->getSize(), mPerceptrons[j]->weights());

        if(sums[j] > max)
        {
            perceptronNum = j;
            max = sums[j];
        }
    }

    if(perceptronNum != _item->getData()[mId])
    {
        updateWeights(_item->getData(), _item->getSize(),
            mPerceptrons[perceptronNum]->weights(), DATA_TYPE::ANSWER);
        updateWeights(_item->getData(), _item->getSize(),
            mPerceptrons[_item->getData()[mId]]->weights(), DATA_TYPE::CORRECT);

        mLifeTime = 0;
    }
    else
    {
        mLifeTime++;

        if(mLifeTime > mPocket.lifeTime)
        {
            mPocket.lifeTime = mLifeTime;

            for(unsigned int i = 0; i < size; i++)
                mPocket.weights[i] = mPerceptrons[i]->weights();
        }
    }

    delete [] sums;
}
