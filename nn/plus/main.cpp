#include <iostream>

#include ".\src\fclayer.hpp"
#include ".\src\activationlayer.hpp"
#include ".\src\activations.hpp"
#include ".\src\network.hpp"

using namespace std;

typedef float t;

typedef std::vector<t> one;
typedef std::vector<std::vector<t>> two;
typedef std::vector<std::vector<std::vector<t>>> three;

void n() {

    Matrix<float> input((three) {
            {
	{0, 0}
            },
            {
	{0, 1}
            },
            {
	{1, 0}
            },
            {
	{1, 1}
            }
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
    cout << "here\n";
    
}

int main() {

    n();
    
    return 0;
}
