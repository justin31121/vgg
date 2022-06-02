#include <iostream>

#include ".\src\fclayer.hpp"
#include ".\src\activationlayer.hpp"
#include ".\src\activations.hpp"
#include ".\src\network.hpp"

using namespace std;

int main() {

    Matrix<float> input({
            {0, 1},
        });

    FcLayer dense(2, 3);
    ActivationLayer act(relu, relu_prime);
    FcLayer dense2(3, 4);

    Network net;
    net.add_layer(&dense);
    net.add_layer(&act);
    net.add_layer(&dense2);
    net.add_layer(&act);

    Matrix<float> result = net.predict(input);

    cout << result << "\n";
    
    return 0;
}
