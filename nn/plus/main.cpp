#include <iostream>

#include ".\src\fclayer.hpp"
#include ".\src\activationlayer.hpp"
#include ".\src\activations.hpp"
#include ".\src\losses.hpp"
#include ".\src\network.hpp"

std::tuple<std::vector<Matrix<float>>, std::vector<Matrix<float>>>get_data(int range) {

    std::vector<Matrix<float>> x_train;
    std::vector<Matrix<float>> y_train;

    float x_min = -50.0;
    float x_max = 50.0;

    float y_min = x_min*x_min;
    float y_max = x_max*x_max;

    for(int i=0;i<range;i++) {
        float x = randf(-50.0, 50.0) / (x_max - x_min);
        float y = x*x ;

        x_train.push_back((Matrix<float>) ((std::vector<float>) {x}));
        y_train.push_back((Matrix<float>) ((std::vector<float>) {y}));
    }
  
    return std::make_tuple(x_train, y_train);
}

std::vector<Matrix<float>>make_data(std::vector<float> vec) {
    std::vector<Matrix<float>> result;

    for(int i=0;i<vec.size();i++) {
        Matrix<float> temp = ((std::vector<float>) {
	vec[i]});

        result.push_back(temp);
    }

    return result;
}

void test_data(std::vector<Matrix<float>> x_train,
               std::vector<Matrix<float>> y_train,
               Network *net) {
    for(int i=0;i<10;i++) {
        auto result = (*net).predict(x_train[i]);
        std::cout << x_train[i] << " => " << y_train[i] << " != " << result << std::endl;
        auto err = relu_prime(mse_prime(y_train[i], result));
        std::cout << "\t" << err << "\n";
    }
    std::cout << std::endl << std::endl;
    std::cout << (*net) << "\n";
}

int main() {
    auto [x_train, y_train] = get_data(10000);

    FcLayer dense1(1, 10);
    ActivationLayer act1(relu, relu_prime);
    FcLayer dense2(10, 1);

    Network net;
    net.add_layer(&dense1);
    net.add_layer(&act1);
    net.add_layer(&dense2);

    net.use(mse, mse_prime);
  
    //LOG
    test_data(x_train, y_train, &net);
    net.train(x_train, y_train, 300, 0.0001, true);
    test_data(x_train, y_train, &net);

    //TEST
    auto list = make_data({4, -1, 11, 20, 8, -5});

    for(int i=0;i<list.size();i++) {
        auto r = net.predict(list[i].scale(1.0/100.0)).scale(100.0);

        std::cout << list[i] << " -> " << r << std::endl;
    }
 
    return 0;
}
