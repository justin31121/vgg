#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <vector>
#include <string>
#include <functional>

#define ROW_CAP 12
#define COL_CAP 12

template <typename T>
class Matrix {
private:
    std::vector< std::vector<T> > matrix;
    int row;
    int col;

    void check_dimensions(Matrix<T> a) {
        if(col!=a.col || row!=a.row) {
            std::cout << "ERROR: Can not operate on Dimensions (" << col << ", " << row << ")";
            std::cout << " and (" << a.col << ", " << a.row << ")\n"; 
            std::exit(1);
        }
    };
public:

    Matrix(std::vector< std:: vector<T>> vec) {

        T r = vec.size();
        T c = vec[0].size();
        
        if(r > ROW_CAP) {
            std::cout << "ERROR: Maximum row size is " << ROW_CAP << " (" << r << ")\n";
            exit(1);
        }	    
        row = r;
        if(c > COL_CAP) {
            std::cout << "ERROR: Maximum row size is " << COL_CAP << " (" << c  << ")\n";
            exit(1);
        }
        col = c;

        matrix = vec;
    };
    
    Matrix(int r, int c): matrix(r, std::vector<T> (c, 0)) {
        if(r > ROW_CAP) {
            std::cout << "ERROR: Maximum row size is " << ROW_CAP << " (" << r << ")\n";
            exit(1);
        }
        row = r;
        if(c > COL_CAP) {
            std::cout << "ERROR: Maximum row size is " << COL_CAP << " (" << c  << ")\n";
            exit(1);
        }
        col = c;
    };

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        os << "[ ";

        for(int i=0;i<m.row;i++) {
            if(i==0) {
	 os << "[";
            }
            else {
	os << "  [";
            }
            for(int j=0;j<m.col;j++) {
	os << m.matrix[i][j];
	if(j==m.col-1) {
	    os << " ]";
	}
	else {
	    os << ", ";
	}
            }
            if(i!=m.row-1) {
	os << "\n";
            }
        }
        os << " ]";
        return os;
    };
  
    std::string to_string() {
        
        std::string word ="[ ";

        for(int i=0;i<row;i++) {
            if(i==0) {
	word += "[";
            }
            else {
	word += "  [";
            }
            for(int j=0;j<col;j++) {
	word += std::to_string(matrix[i][j]);
	if(j==col-1) {
	    word += " ]";
	}
	else {
	    word += ", ";
	}
            }
            if(i!=row-1) {
	word += "\n";
            }
        }

        word += " ]";
        return word;
    };

    Matrix<T> transpose() {
        Matrix<T> result(col, row);

        for(int i=0;i<row;i++) {
            for(int j=0;j<col;j++) {
	result.matrix[j][i] = matrix[i][j];
            }
        }

        return result;
    };

    Matrix<T> mult(Matrix<T> a) {
        if(col!=a.row) {
            std::cout << "ERROR: Can not multiply Dimensions (" << col << ", " << row << ")";
            std::cout << " and (" << a.col << ", " << a.row << ")\n"; 
            std::exit(1);
        }

        Matrix result(row, a.col);

        for(int i=0;i<row;i++) {
            for(int k=0;k<a.col;k++) {
	T sum = 0;
	for(int j=0;j<col;j++) {
	    sum += matrix[i][j] * a.matrix[j][k];
	}
	result.matrix[i][k] = sum;
            }
        }

        return result;
    };

    Matrix<T> for_each(std::function<T(T)> f) {
        Matrix<T> result(row, col);

        for(int i=0;i<row;i++) {
            for(int j=0;j<col;j++) {
	result.matrix[i][j] = f(matrix[i][j]);
            }
        }
        
        return result;  
    };

    Matrix<T> for_each(Matrix<T> a, std::function<T(T, T)> f) {
        Matrix<T> result(row, col);

        for(int i=0;i<row;i++) {
            for(int j=0;j<col;j++) {
	result.matrix[i][j] = f(matrix[i][j], a.matrix[i][j]);
            }
        }
        
        return result;  
    };
    
    Matrix<T> add(Matrix a) {
        check_dimensions(a);
        
        Matrix<T> result(row, col);
        
        return result.for_each(a, [](T n, T m) -> T {return n + m;});
    }

    Matrix<T> sub(Matrix<T> a) {
        check_dimensions(a);
        
        Matrix<T> result(row, col);
        
        return result.for_each(a, [](T n, T m) -> T {return n - m;});    
    }

    Matrix<T> scale(T k) {
        Matrix<T> result(row, col);

        for(int i=0;i<row;i++) {
            for(int j=0;j<col;j++) {
	result.matrix[i][j] *= k;
            }
        }
        
        return result;  
    }

    Matrix<T> shift(T k) {
        Matrix<T> result(row, col);

        for(int i=0;i<row;i++) {
            for(int j=0;j<col;j++) {
	result.matrix[i][j] += k;
            }
        }
        
        return result;  
    }

    std::vector< std::vector<T> > get_matrix() {
        return matrix;
    }

    int get_row() {
        return row;
    }

    int get_col() {
        return col;
    }
};

#endif // MATRIX_H
