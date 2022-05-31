#ifndef ACTIVATIONLAYER_H
#define ACTIVATIONLAYER_H

#include ".\src\activationfunction.hpp"

class ActivationLayer: public Layer {
private:
  ActivationFunction *f;
  Matrix<float> input;
  Matrix<float> output;
public:

  ActivationLayer(ActivationFunction *_f):
    input(0, 0),
    output(0 ,0)
  {
    f = _f;
  };
  
  Matrix<float> forward_propagation(Matrix<float> _input) {
    input = _input;
    output = (*f).activate(_input);
    return output;
  };

  Matrix<float> backward_propagation(Matrix<float> output_error, float learning_rate) {
    return (*f).activate_prime(input).mult(output_error);
  };
};

#endif //ACTIVATIONLAYER_H
