#ifndef RELU_H
#define RELU_H

#include <bits/stdc++.h>
#include ".\matrix.hpp"

Matrix<float> relu(Matrix<float> x) {
    return x.for_each([](float n) -> float{return n > 0 ? n : 0;});
}

Matrix<float> relu_prime(Matrix<float> x) {
    return x.for_each([](float n) -> float{return n > 0 ? 1 : 0;});
}

Matrix<float> softmax(Matrix<float> x) {  
    Matrix<float> exp_values = x.for_each([](float n) -> float{ return std::exp(n);});

    float sum = exp_values.sum();

    return exp_values.scale(1/sum);
}

Matrix<float> softmax_prime(Matrix<float> x) {
    Matrix<float> temp = softmax(x);
    Matrix<float> neg = temp.for_each([](float n) -> float {
        return 1.0 - n;
    });
  
    return temp.mult(neg);
}

Matrix<float> tanhh(Matrix<float> x) {
    return x.for_each([](float n) -> float{return std::tanh(n);});
}

Matrix<float> tanhh_prime(Matrix<float> x) {
    Matrix<float> temp = tanhh(x);
    temp = temp.mult(temp);

    return temp.for_each([](float n) -> float{return 1.0 - n;});
}
#endif // RELU_H
