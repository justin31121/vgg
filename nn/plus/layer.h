#ifndef LAYER_H
#define LAYER_H

#include "matrix.hpp"

class Layer {
  virtual Matrix<float> forward_propagation(Matrix<float> input);
};

#endif // LAYER_H
