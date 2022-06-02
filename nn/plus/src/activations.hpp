#ifndef RELU_H
#define RELU_H

#include ".\matrix3.hpp"

float relu_max(float x) {
  return x > 0 ? x : 0;
}

float relu_prime_max(float x) {
  return x > 0 ? 1 : 0;
}

Matrix<float> relu(Matrix<float> x) {

  Matrix result = x.for_each(relu_max);
  
  return result;
}

Matrix<float> relu_prime(Matrix<float> x) {
  Matrix result = x.for_each(relu_prime_max);
  return result;
}


#endif // RELU_H
