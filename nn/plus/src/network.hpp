#ifndef NETWORK_H
#define NETWORK_H

#include ".\layer.hpp"

#define LAYER_CAP 12

class Network {
private:
    Layer *layers[LAYER_CAP];
    int layer_size;

    std::function<Matrix<float>(Matrix<float>, Matrix<float>)> loss;
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

    void use(std::function<Matrix<float>(Matrix<float>, Matrix<float>)> _loss,
             std::function<Matrix<float>(Matrix<float>, Matrix<float>)> _loss_prime)
    {
        loss = _loss;
        loss_prime = _loss_prime;
    }

    Matrix<float> predict(Matrix<float> _input) {

        Matrix temp = _input;

       
        for(int i=0;i<layer_size;i++) {
            std::cout << i << "\n" ;
            temp = (*layers[i]).forward_propagation(temp);
        }

        return temp;
    }

    void train(Matrix<float> x_train, Matrix<float> y_train,
               int epochs, float learning_rate)
    {
        //TODO
        int sample_len = 0;;

        for(int i=0;i<epochs;i++) {
            float err = 0.0f;
            for(int j=0;j<sample_len;j++) {
	//Matrix<float> output = 
            }
        }
    
    }
};

#endif //NETWORK_H
