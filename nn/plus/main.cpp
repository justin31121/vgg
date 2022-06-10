#include <iostream>

#include ".\src\fclayer.hpp"
#include ".\src\activationlayer.hpp"
#include ".\src\activations.hpp"
#include ".\src\losses.hpp"
#include ".\src\network.hpp"

using namespace std;

int main() {

  vector<Matrix<float>> x_train({
      (Matrix<float> ({
	  {0, 0}
	})),
      (Matrix<float> ({
	  {0, 1}
	})),
      (Matrix<float> ({
	  {1, 0}
	})),
      (Matrix<float> ({
	  {1, 1}
	}))      
    });
  
  vector<Matrix<float>> y_train({
      (Matrix<float> (1,1)).for_each([](float n) -> float { return 0;}),
      (Matrix<float> (1,1)).for_each([](float n) -> float { return 1;}),
      (Matrix<float> (1,1)).for_each([](float n) -> float { return 1;}),
      (Matrix<float> (1,1)).for_each([](float n) -> float { return 1;})
    });
  
  //INIT
  FcLayer dense(2, 3);
  ActivationLayer act(relu, relu_prime);
  FcLayer dense2(3, 1);

  Network net;
  net.add_layer(&dense);
  net.add_layer(&act);
  net.add_layer(&dense2);
  net.add_layer(&act);
  net.use(mse, mse_prime);

  //TRAIN
  

  //PREDICT
  
  return 0;
}
