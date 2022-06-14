#ifndef LOSSES_H
#define LOSSES_H

#include ".\matrix.hpp"

//return np.mean(np.power(y_true-y_pred, 2))
float mse(Matrix<float> y_true, Matrix<float> y_pred) {
    Matrix<float> diff = y_true.sub(y_pred);
    Matrix<float> squared =
        diff.for_each(diff, [](float n, float m) -> float { return n * m;});
    return squared.mean();
}

//return 2*(y_pred-y_true)/y_true.size
Matrix<float> mse_prime(Matrix<float> y_true, Matrix<float> y_pred) {
    float size = y_true.size();
    return y_pred.sub(y_true).for_each([](float n) -> float{ return 2*n;}).scale(1/size);
}

#endif // LOSSES_H
