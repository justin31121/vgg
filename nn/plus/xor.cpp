#include <iostream>

#include ".\src\fclayer.hpp"
#include ".\src\activationlayer.hpp"
#include ".\src\activations.hpp"
#include ".\src\losses.hpp"
#include ".\src\network.hpp"

int main() {
    std::vector<Matrix<float>> x_train({
      (Matrix<float> ((std::vector<std::vector<float>>) {
	  {0, 0}
	})),
      (Matrix<float> ((std::vector<std::vector<float>>) {
	  {0, 1}
	})),
      (Matrix<float> ((std::vector<std::vector<float>>){
	  {1, 0}
	})),
      (Matrix<float> ((std::vector<std::vector<float>>){
	  {1, 1}
	}))      
    });

  std::vector<Matrix<float>> y_train2({
      (Matrix<float> ((std::vector<std::vector<float>>){
	  {1, 0}
	})),
      (Matrix<float> ((std::vector<std::vector<float>>){
	  {0, 1}
	})),
      (Matrix<float> ((std::vector<std::vector<float>>){
	  {0, 1}
	})),
      (Matrix<float> ((std::vector<std::vector<float>>){
	  {1, 0}
	}))
    });
  
  std::vector<Matrix<float>> y_train({
      (Matrix<float> (1,1)).for_each([](float n) -> float { return 0;}),
      (Matrix<float> (1,1)).for_each([](float n) -> float { return 1;}),
      (Matrix<float> (1,1)).for_each([](float n) -> float { return 1;}),
      (Matrix<float> (1,1)).for_each([](float n) -> float { return 0;})
    });

   std::vector<Matrix<float>> y_train3({
      (Matrix<float> ((std::vector<float>) {0})),
      (Matrix<float> ((std::vector<float>) {1})),
      (Matrix<float> ((std::vector<float>) {1})),
      (Matrix<float> ((std::vector<float>) {0})),      
    });
  
  //init
  FcLayer dense(2, 3);
  ActivationLayer act(tanhh, tanhh_prime);
  FcLayer dense1(3, 1);
  ActivationLayer act2(tanhh, tanhh_prime);
  
  Network net;
  net.add_layer(&dense);
  net.add_layer(&act);
  net.add_layer(&dense1);
  net.add_layer(&act2);

  net.use(mse, mse_prime);

  //train
  net.train(x_train, y_train3, 600, 0.1, true);

  for(int i=0;i<4;i++) {
    Matrix<float> result = net.predict(x_train[i]);

    std::cout << result << "\n\n";
  }

  return 0;
}
