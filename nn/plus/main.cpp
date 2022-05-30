#include <iostream>
#include "Matrix.hpp"

using namespace std;


int main() {

    Matrix t({
            {1, 2, 3},
        });

    cout << t.to_string() << "\n";
    
    return 0;
}
