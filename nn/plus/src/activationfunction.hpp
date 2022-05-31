#ifndef ACTIVATIONFUNCTION_H
#define ACTIVATIONFUNCTION_H

#include ".\src\matrix.hpp"

class ActivationFunction {
public:
  virtual ~ActivationFunction() {};
  virtual Matrix<float> activate(Matrix<float> x) = 0;
  virtual Matrix<float> activate_prime(Matrix<float> x) = 0;
};

#endif //ACTIVATIONFUNCTION_H
