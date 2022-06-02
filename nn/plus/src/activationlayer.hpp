#ifndef ACTIVATIONLAYER_H
#define ACTIVATIONLAYER_H

class ActivationLayer: public Layer {
private:
  std::function<Matrix<float>(Matrix<float>)> activate;
  std::function<Matrix<float>(Matrix<float>)> activate_prime;
  Matrix<float> input;
  Matrix<float> output;
public:

  ActivationLayer(std::function<Matrix<float>(Matrix<float>)> _activate,
		  std::function<Matrix<float>(Matrix<float>)> _activate_prime):
    input(0, 0),
    output(0 ,0)
  {
    activate = _activate;
    activate_prime = _activate_prime;
  };
  
  Matrix<float> forward_propagation(Matrix<float> _input) {
    input = _input;
    output = activate(_input);
    return output;
  };

  Matrix<float> backward_propagation(Matrix<float> output_error, float learning_rate) {
    return activate_prime(input).mult(output_error);
  };
};

#endif //ACTIVATIONLAYER_H
