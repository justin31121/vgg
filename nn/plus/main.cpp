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
  
  //init
  FcLayer dense(2, 3);
  ActivationLayer act(relu, relu_prime);
  FcLayer dense2(3, 1);

  Network net;
  net.add_layer(&dense);
  net.add_layer(&act);
  net.add_layer(&dense2);
  net.add_layer(&act);
  net.use(mse, mse_prime);

  //train;
  Matrix<float> result = net.predict(x_train[0]);
  Matrix<float> error = mse_prime(y_train[0], result);

  //cout << dense2.getWeights().to_string() << "\n";

  dense2.backward_propagation(error, 0.01).to_string();

  //cout << dense2.getWeights().to_string() << "\n";
  
  //predict
  
  return 0;
}
