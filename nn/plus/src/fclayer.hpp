#ifndef FCLAYER_H
#define FCLAYER_H

#include ".\src\layer.hpp"

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
    std::vector< std::vector<float> > w(input_size, std::vector<float>(output_size, 0));
    std::vector< std::vector<float> > b(1, std::vector<float>(output_size, 0));
    
    for(int i=0;i<input_size;i++) {
      for(int j=0;j<output_size;j++) {
	w[i][j] = randf(-1.0, 1.0);
	b[0][j] = randf(-1.0, 1.0);
      }
    }

    Matrix<float> temp_weights(w);
    Matrix<float> temp_bias(b);
    weights = temp_weights;
    bias = temp_bias;
  };
  
  Matrix<float> forward_propagation(Matrix<float> _input) {
    input = _input;
    output = _input.mult(weights).add(bias);

    return output;
  };

  Matrix<float> backward_propagation(Matrix<float> output_error, float learning_rate) {
    Matrix<float> input_error = output_error.mult(weights.transpose());
    Matrix<float> weights_error = input.transpose().mult(output_error);

    weights = weights.sub(weights_error.scale(learning_rate));
    bias = bias.sub(output_error.scale(learning_rate));

    return input_error;
  };
};

#endif //FCLAYER_H

