#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <string>
#include <vector>

template<typename T>
class Matrix {
private:
  std::vector< std::vector< std::vector<T> > > matrix;
  int height;
  int width;
  int depth;

  bool one_d() {
    return width==1 && depth==1;
  }

  bool two_d() {
    return width!=1 && depth==1;
  }

  bool three_d() {
    return width!=1 && depth!=1;
  }
public:

  Matrix(std::vector< std::vector< std::vector<T>>>& vec):
    matrix(0, std::vector<std::vector<T>> (0, std::vector<T> (0, 0)))
  {
    height = vec.size();
    width = vec[0].size();
    depth = vec[0][0].size();

    std::vector< std::vector< std::vector<T>>> temp(height, std::vector<std::vector<T>> (width, std::vector<T> (depth, 0)));

    for(int i=0;i<height;i++) {
      for(int j=0;j<width;j++) {
	for(int k=0;k<depth;k++) {
	  temp[i][j][k] = vec[i][j][k];
	}
      }
    }

    matrix = temp;
  };


  Matrix(std::vector< std::vector<T>>& vec):
    matrix(0, std::vector<std::vector<T>> (0, std::vector<T> (0, 0)))
  {
    height = vec.size();
    width = vec[0].size();
    depth = 1;

    std::vector< std::vector< std::vector<T>>> temp(height, std::vector<std::vector<T>> (width, std::vector<T> (1, 0)));

    for(int i=0;i<height;i++) {
      for(int j=0;j<width;j++) {
	temp[i][j][0] = vec[i][j];
      }
    }

    matrix = temp;
  };


  Matrix(std::vector<T>& vec):
    matrix(0, std::vector<std::vector<T>> (0, std::vector<T> (0, 0)))
  {
    height = vec.size();
    width = 1;
    depth = 1;

    std::vector< std::vector< std::vector<T>>> temp(height, std::vector<std::vector<T>> (1, std::vector<T> (1, 0)));

    for(int i=0;i<height;i++) {
      temp[i][0][0] = vec[i];
    }

    matrix = temp;
  };
  
  Matrix(int h):
    matrix(h, std::vector<std::vector<T>> (1, std::vector<T> (1, 0)))
  {

    std::cout << "here\n";
    
    height = h;
    width = 1;
    depth = 1;
  };


  Matrix(int h, int w):
    matrix(h, std::vector<std::vector<T>> (w, std::vector<T> (1, 0)))
  {
    height = h;
    width = w;
    depth = 1;
  };

  Matrix(int h, int w, int d):
    matrix(h, std::vector<std::vector<T>> (w, std::vector<T> (d, 0)))
  {

    std::cout << "ehmmm\n";
    
    height = h;
    width = w;
    depth = d;
  };

  std::string to_string() {
    
    std::string acc = "[ ";
    for(int i=0;i<height;i++) {
      if(width==1) {
	acc+=std::to_string(matrix[i][0][0]);
	if(i!=height-1) {
	  acc += ", ";
	}
      }
      else {
	if(i!=0) {
	  acc+="  ";
	}
	acc += "[";
	for(int j=0;j<width;j++) {
	  if(depth!=1) {	    
	    std::string acc_d = "[";
	    for(int k=0;k<depth;k++) {
	      acc_d += std::to_string(matrix[i][j][k]);
	      if(k!=depth-1) {
		acc_d +=", ";
	      }	    
	    }
	    acc+=acc_d+"]";
	  }
	  else {
	    acc+=std::to_string(matrix[i][j][0]);
	  }
	  if(j!=width-1) {
	    acc +=", ";
	  }
	}
	acc += "]";
	if(i!=height-1) {
	  acc += "\n";
	}

      }
    }
    return acc+" ]";
  }
};


#endif //MATRIX_H
