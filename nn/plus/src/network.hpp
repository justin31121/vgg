#ifndef NETWORK_H
#define NETWORK_H

#include ".\src\layer.hpp"

#define LAYER_CAP 12

class Network {
private:
  Layer *layers[LAYER_CAP];
  int layer_size;

  std::function<float(Matrix<float>, Matrix<float>)> loss;
  std::function<Matrix<float>(Matrix<float>, Matrix<float>)> loss_prime;
public:
  Network() {
    layer_size = 0;
  };

  void add_layer(Layer *layer) {
    if(layer_size==LAYER_CAP) {
      std::cout << "ERROR: Network, layer_overflow\n";
      exit(1);
    }

    layers[layer_size++] = layer;
  }

  void use(std::function<float(Matrix<float>, Matrix<float>)> _loss,
	   std::function<Matrix<float>(Matrix<float>, Matrix<float>)> _loss_prime)
  {
    loss = _loss;
    loss_prime = _loss_prime;
  }

  Matrix<float> predict(Matrix<float> _input) {

    Matrix temp = _input;

       
    for(int i=0;i<layer_size;i++) {
      temp = (*layers[i]).forward_propagation(temp);
    }

    return temp;
  }

  void train(std::vector<Matrix<float>> x_train, std::vector<Matrix<float>> y_train,
	     int epochs, float learning_rate)
  {
    int sample_len = x_train.size();
    
    for(int i=0;i<epochs;i++) {
      float err = 0;
      for(int j=0;j<sample_len;j++) {
	Matrix<float> output = x_train[j];

	for(int k=0;k<layer_size;k++) {
	  output = (*layers[k]).forward_propagation(output);
	}

	err = err + loss(y_train[j], output);

	Matrix<float> error = loss_prime(y_train[j], output);
	for(int k=layer_size-1;k>=0;k--) {
	  error = (*layers[k]).backward_propagation(error, learning_rate);
	}

      }
      err /= sample_len;
      std::cout << "epoch " << i << "/" << epochs << " error=" << err << "\n";
    }
    
  }
};

#endif //NETWORK_H
