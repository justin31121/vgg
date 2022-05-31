#ifndef LAYER_H
#define LAYER_H

#include ".\src\util.hpp"
#include ".\src\matrix.hpp"

class Layer {
public:
  virtual ~Layer() {};
  virtual Matrix<float> forward_propagation(Matrix<float> input) = 0;
  virtual Matrix<float> backward_propagation(Matrix<float> output_error, float learning_rate) = 0;
};

#endif //LAYER_H
