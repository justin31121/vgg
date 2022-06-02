#include <iostream>

#include ".\src\matrix3.hpp"

using namespace std;

int main() {

  int height = 2;
  int width = 2;
  int depth = 2;

  std::vector<int> temp1{
    {1, 2, 3}
  };

  std::vector<std::vector<int>> temp2{
    {1, 2, 3},
    {4, 5, 6}
  };

  std::vector< std::vector< std::vector<int>>> temp3{
    {{1, 3}, {2, 4}, {3, 5}},
  };

  Matrix<int> t(temp3);

  cout << t.to_string() << "\n";
  
  return 0;
}
