#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include "trainingitem.h"

#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <string>

/* SPLA (Simple Perceptron Learning Algorithm)
 *
 * Input E[2]: (1, -1) -> -1 (<-- T - is input correct?)
 *             (1, 1) -> 1 (<-- T, 1 - yes, -1 - no)
 *
 * (1) Random input E
 * (2) Pre-computing
 *      weights = random(0, 1)
 *      THETA = 0 (or random?)
 *      learningConst = <0; 1>
 *
 *      Activation function f: f(x) = -1, x < THETA
 *                             f(x) = 1, x >= THETA
 * (2) Learn(E, T)
 *          while(currentIt < maxIt)
 *              O = f(SUM(wi * Ei))
 *              ERR = T - O
 *
 *              if ERR == 0 then
 *                  break and go to step (3)
 *              else
 *                  wi = wi + learningConst * ERR * Ei
 *                  THETA = THETA - learningConst * ERR
 *
 *              currentIt++
 *
 * (3) Next random input and back to step (2)
 */

/* PLA (Pocket Leraning Algorithm)
 *
 * Input E[2]: (1, -1) -> -1 (<-- T - is input correct?)
 *             (1, 1) -> 1 (<-- T, 1 - yes, -1 - no)
 *
 * (1) Random input E
 * (2) Pre-computing
 *      Pocket { weights, lifeOfTime } pocket
 *      weights = random(0, 1)
 *      THETA = 0 (or random?)
 *      learningConst = <0; 1>
 *      lifeOfTime = 0
 *
 *      Activation function f: f(x) = -1, x < THETA
 *                             f(x) = 1, x >= THETA
 * (2) Learn(E, T)
 *          while(1)
 *              O = f(SUM(wi * Ei))
 *              ERR = T - O
 *
 *              if ERR == 0 then
 *                  lifeOfTime++
 *
 *                  if(pocket.lifeOfTime < lifeOfTime)
 *                      pocket.weights = weights
 *                      break and go to step (3)
 *              else
 *                  wi = wi + learningConst * ERR * Ei
 *                  THETA = THETA - learningConst * ERR
 *                  lifeOfTime = 0
 *
 * (3) Next random input and back to step (2)
 */

#define MAX_WIDTH 5
#define MAX_HEIGHT 7
#define MAX_LOOP_IT 1000

struct Pocket
{
    Pocket()
    {
        lifeOfTime = 0;
        weights.resize(MAX_WIDTH * MAX_HEIGHT);
    }

    int lifeOfTime;
    std::vector<float> weights;
};

class Perceptron
{
public:
    Perceptron(float _learningConst = 0.5);

    // Getters
    std::vector<float> getWeights() const { return weights; }
    float getTheta() const { return theta; }
    bool currentStatus() const { return done; }

    // Setters
    void setWeights(std::vector<float> _weights) { weights = _weights; }
    void setLearningConst(float _learningConst) { learningConst = _learningConst; }
    void setStatus(bool _status) { done = _status; }

    void trainSPLA(bool* _input, int _expected);
    void trainPLA(bool* _input, int _expected);

    int check(bool* _input);
    std::string toString() const;
private:
    float fRand(float _fMin, float _fMax); // Random number between _fMin and _fMax.
    int activate(float _sum, float _theta); // Activate function.
    float sum(bool* _input, std::vector<float> _weights); // Sum all weights.
    void update(bool* _input, float _err); // Update weights for each loop step.

    std::vector<float> weights; // Training weights.
    float learningConst; // Learning const.
    float theta;
    bool done;

    Pocket pocket;
    int lifeOfTime;
};

#endif // PERCEPTRON_H
