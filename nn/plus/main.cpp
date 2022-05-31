#include <iostream>

#include ".\src\fclayer.hpp"
#include ".\src\activationlayer.hpp"
#include ".\src\relu.hpp"

using namespace std;

int main() {

  Matrix<float> input({
      {0.0f, 0.0f},
    });

  FcLayer dense1(2, 3);

  ReLU t;
  
  ActivationLayer act(&t);

  //========

  Matrix<float> temp = dense1.forward_propagation(input);

  cout << act.forward_propagation(temp).to_string() << "\n";
    
  return 0;
}
