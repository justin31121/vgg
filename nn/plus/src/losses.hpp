#ifndef LOSSES_H
#define LOSSES_H

#include ".\src\matrix.hpp"

//return np.mean(np.power(y_true-y_pred, 2))
Matrix<float> mse(Matrix<float> y_true, Matrix<float> y_pred) {
  return y_true;
}

//return 2*(y_pred-y_true)/y_true.size
Matrix<float> mse_prime(Matrix<float> y_true, Matrix<float> y_pred) {
  return y_true;
}

#endif // LOSSES_H
