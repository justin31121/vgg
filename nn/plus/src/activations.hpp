#ifndef RELU_H
#define RELU_H

#include ".\src\matrix.hpp"

Matrix<float> relu(Matrix<float> x) {
  std::vector< std::vector<float >> temp = x.get_matrix();
  int row = x.get_row();
  int col = x.get_col();
    
  for(int i=0;i<row;i++) {
    for(int j=0;j<col;j++) {
      temp[i][j] = temp[i][j] > 0.0 ? temp[i][j] : 0.0;
    }
  }

  Matrix result(temp);
  return result;
}

Matrix<float> relu_prime(Matrix<float> x) {
  std::vector< std::vector<float >> temp = x.get_matrix();
  int row = x.get_row();
  int col = x.get_col();
    
  for(int i=0;i<row;i++) {
    for(int j=0;j<col;j++) {
      temp[i][j] = temp[i][j] > 0.0 ? 1 : 0.0;
    }
  }

  Matrix result(temp);
  return result;
}


#endif // RELU_H
