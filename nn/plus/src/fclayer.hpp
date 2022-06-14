#ifndef FCLAYER_H
#define FCLAYER_H

#include ".\layer.hpp"

class FcLayer: public Layer {
private:
    Matrix<float> weights;
    Matrix<float> bias;
    Matrix<float> input;
    Matrix<float> output;
public:
    /*
[ [-1.70079e+06, -1.13071e+06, -1.77472e+06, -1.29577e+06, -1.56377e+06, -1.00939e+06, -1.42073e+06, -785381 ] ]
[ [0.813837, 0.0466323, 0.814203, 0.239998, 0.897885, 0.256508, 0.269356, 0.208014 ] ]
     */

    FcLayer(int input_size, int output_size):
        weights(input_size, output_size),
        bias(1, output_size),
        input(0, 0),
        output(0,0)
    {
        weights = weights.for_each([](float n) -> float { return randf(0.0, 1.0);});
        //weights = weights.scale(std::sqrt(2/output_size));
        bias = bias.for_each([](float n) -> float { return randf(-1.0, 1.0);});
    };
  
    Matrix<float> forward_propagation(Matrix<float> _input) {
        input = _input;
        output = _input.dot(weights);
        
        return output;
    };

    Matrix<float> backward_propagation(Matrix<float> output_error, float learning_rate) {
    
        Matrix<float> input_error = output_error.dot(weights.transpose());
        Matrix<float> weights_error = input.transpose().dot(output_error);

        weights = weights.sub(weights_error.scale(learning_rate));
        bias = bias.sub(output_error.scale(learning_rate));

        return input_error;
    };

    void log(std::ostream& os) {
        os << "==========Fully Connected Layer==========" << "\n";
        os << "Weights:\n";
        os << weights << "\n";
        os << "Biases:\n";
        os << bias << "\n";
        os << "Input:\n";
        os << input << "\n";
        os << "Output:\n";
        os << output << "\n\n";
    };

    Matrix<float> getWeights() {
        return weights;
    };

    Matrix<float> getBias() {
        return bias;
    }
};

#endif //FCLAYER_H

