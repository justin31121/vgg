#include <iostream>

#include ".\src\fclayer.hpp"
#include ".\src\activationlayer.hpp"
#include ".\src\activations.hpp"

using namespace std;

int main() {

  Matrix<float> input({
      {0.0f, 0.0f},
    });

  FcLayer dense1(2, 3);
  
  ActivationLayer act1(relu, relu_prime);

  //========

  Matrix<float> result =
    act1.forward_propagation(dense1.forward_propagation(input));

  cout << result.to_string() << "\n";
    
  return 0;
}
