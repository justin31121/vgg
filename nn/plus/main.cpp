#include <iostream>

#include ".\src\fclayer.hpp"
#include ".\src\activationlayer.hpp"
#include ".\src\activations.hpp"
#include ".\src\losses.hpp"
#include ".\src\network.hpp"

float x_min = -50.0;
float x_max = 60.0;
float y_min = 0;
float y_max = x_max*x_max;

float normalize(float x) {
    return (x - x_min) / (x_max - x_min);
}

float denormalize(float x) {
    return (x * (x_max - x_min)) + x_min;
}

std::tuple<std::vector<Matrix<float>>, std::vector<Matrix<float>>>get_data(int range) {

    std::vector<Matrix<float>> x_train;
    std::vector<Matrix<float>> y_train;

    for(int i=0;i<range;i++) {
        float x = randf(x_min, x_max);
        x = normalize(x);
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

void test_data(std::vector<Matrix<float>> x_train,
               std::vector<Matrix<float>> y_train,
               Network *net) {
    for(int i=0;i<10;i++) {
        auto result = (*net).predict(x_train[i]);
        std::cout << x_train[i] << " => " << y_train[i] << " != " << result << std::endl;
        auto err = mse_prime(y_train[i], result);
        std::cout << "\t" << err << "\n";
    }
    std::cout << std::endl << std::endl;
    std::cout << (*net) << "\n";
}

float square(float n,float m) {
    return n*m;
}

int main() {
    float a = randf(x_min, x_max);
    float b = a*a;
    float ac = normalize(a);

    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << denormalize(ac*ac) << std::endl;


    return 0;
}

int main11() {
    auto list = make_data({4.0, -1.0, 11.0, 20.0, 8.0, -5.0});

    for(int i=0;i<list.size();i++) {
        std::cout << list[i] << std::endl;
        std::cout << list[i].for_each(list[i], square).to_string() << std::endl;

        auto curr = list[i].for_each(normalize);
        
        std::cout << curr << std::endl;
        std::cout << curr.for_each(curr, square) << std::endl;
        std::cout << curr.for_each(curr, square).for_each(denormalize) << std::endl;
        std::cout << std::endl;
    }

    return 0;
}

int main1() {
    
    std::srand(std::time(0));
    
    auto [x_train, y_train] = get_data(5000);

    FcLayer dense1(1, 10);
    ActivationLayer act1(tanhh, tanhh_prime);
    FcLayer dense2(10, 1);

    Network net;
    net.add_layer(&dense1);
    net.add_layer(&act1);
    net.add_layer(&dense2);

    net.use(mse, mse_prime);
  
    //LOG
    test_data(x_train, y_train, &net);
    net.train(x_train, y_train, 100, 0.001, true);
    test_data(x_train, y_train, &net);

    //TEST
    auto list = make_data({4.0, -1.0, 11.0, 20.0, 8.0, -5.0});

    for(int i=0;i<list.size();i++) {
        auto current = list[i].for_each(normalize);
        //auto current = list[i];
        auto r = net.predict(current);

        std::cout << current << " -> " << r << std::endl;
        std::cout << list[i] << " -> " << r.for_each(denormalize) << std::endl;
    }
 
    return 0;
}
