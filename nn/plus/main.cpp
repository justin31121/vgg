#include <iostream>

#include ".\src\fclayer.hpp"
#include ".\src\activationlayer.hpp"
#include ".\src\activations.hpp"
#include ".\src\losses.hpp"
#include ".\src\network.hpp"

std::tuple<std::vector<Matrix<float>>, std::vector<Matrix<float>>>get_data(int range) {

  std::vector<Matrix<float>> x_train;
  std::vector<Matrix<float>> y_train;

  for(int i=0;i<range;i++) {
    float x = randf(-50.0, 50.0);
    float y = x*x;

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

int main() {
  auto [x_train, y_train] = get_data(1000);

  FcLayer dense1(1, 8);
  ActivationLayer act1(relu, relu_prime);
  FcLayer dense2(8, 8);
  ActivationLayer act2(relu, relu_prime);
  FcLayer dense3(8, 1);

  Network net;
  net.add_layer(&dense1);
  net.add_layer(&act1);
  net.add_layer(&dense2);
  net.add_layer(&act2);
  net.add_layer(&dense3);

  net.use(mse, mse_prime);

  //TRAIN
  net.train(x_train, y_train, 1000, 0.1, true);

  //TEST
  auto list = make_data({4, -1, 11, 20, 8, -5});

  for(int i=0;i<list.size();i++) {
    auto r = net.predict(list[i]);

    std::cout << list[i] << " -> " << r << std::endl;
  }
 
  return 0;
}
