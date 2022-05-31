#ifndef TANH_H
#define TANH_H

#include ".\src\activationfunction.hpp"

class Tanh: public ActivationFunction {
public:
  Matrix<float> activate(Matrix<float> x) {
    return x;
  };
  Matrix<float> activate_prime(Matrix<float> x) {
    return x;
  };
};

#endif //TANH_H
