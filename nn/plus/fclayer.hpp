#include <iostream>
#include "layer.h"

class FcLayer: public Layer {
private:
  Matrix<float> weights;
  Matrix<float> bias;
  Matrix<float> input;
  Matrix<float> output;
public:

  FcLayer(int input_size, int output_size):
    weights(input_size, output_size),
    bias(1, output_size),
    input(0, 0),
    output(0,0)
  {
    
  };
  
  Matrix<float> forward_propagation(Matrix<float> _input) {
    input = _input;
    output = _input.mult(weights).add(bias);

    return output;
  };
};











