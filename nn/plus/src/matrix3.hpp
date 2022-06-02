#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <string>
#include <vector>
#include <functional>

template<typename T>
class Matrix {
private:
    std::vector< std::vector< std::vector<T> > > matrix;
    int length;
    int row;
    int col;

    bool one_d() {
        return row==1 && col==1;
    };

    bool two_d() {
        return row!=1 && col==1;
    };

    bool three_d() {
        return row!=1 && col!=1;
    };
public:

    template <typename F, typename std::enable_if< std::is_same<F, T>::value, void** >::type = nullptr > 
    Matrix(std::vector<F> vec):
        matrix(0, std::vector<std::vector<T>> (0, std::vector<T> (0, 0)))
    {
        length = 1;
        row = 1;
        col = vec.size();
        
        std::vector< std::vector< std::vector<T>>> temp(length, std::vector<std::vector<T>> (row, std::vector<T> (col, 0)));

        for(int z=0;z<length;z++) {
            for(int i=0;i<row;i++) {
	for(int j=0;j<col;j++) {
	    temp[z][i][j]= vec[j];
	}
            }
        }

        matrix = temp;
    };

    Matrix(int c):
        matrix(1, std::vector<std::vector<T>> (1, std::vector<T> (c, 0)))
    {
        length = 1;
        row = 1;
        col = c;
    };

    template <typename F, typename std::enable_if< std::is_same<F, std::vector<T>>::value, void** >::type = nullptr > 
    Matrix(std::vector<F> vec):
        matrix(0, std::vector<std::vector<T>> (0, std::vector<T> (0, 0)))
    {
        length = 1;
        row = vec.size();
        col = vec[0].size();
        
        std::vector< std::vector< std::vector<T>>> temp(length, std::vector<std::vector<T>> (row, std::vector<T> (col, 0)));

        for(int z=0;z<length;z++) {
            for(int i=0;i<row;i++) {
	for(int j=0;j<col;j++) {
	    temp[z][i][j]= vec[i][j];
	}
            }
        }

        matrix = temp;
    };

    Matrix(int r, int c):
        matrix(1, std::vector<std::vector<T>> (r, std::vector<T> (c, 0)))
    {
        length = 1;
        row = r;
        col = c;
    };

    template <typename F, typename std::enable_if< std::is_same<F, std::vector<std::vector<T>>>::value, void** >::type = nullptr > 
    Matrix(std::vector<F> vec):
        matrix(0, std::vector<std::vector<T>> (0, std::vector<T> (0, 0)))
    {
        length = vec.size();
        row = vec[0].size();
        col = vec[0][0].size();
        
        std::vector< std::vector< std::vector<T>>> temp(length, std::vector<std::vector<T>> (row, std::vector<T> (col, 0)));

        for(int i=0;i<length;i++) {
            for(int j=0;j<row;j++) {
	for(int k=0;k<col;k++) {
	    temp[i][j][k]= vec[i][j][k];
	}
            }
        }

        matrix = temp;
    };

    Matrix(int l, int r, int c):
        matrix(l, std::vector<std::vector<T>> (r, std::vector<T> (c, 0)))
    {
        length = l;
        row = r;
        col = c;
    };

    friend std::ostream& operator<<(std::ostream& os, Matrix& m)
    {
        os << "[\n";
        for(int i=0;i<m.length;i++) {
            os << "  [";
            for(int j=0;j<m.row;j++) {
	if(j!=0) {
	    os <<"   ";
	}
	os << "[";
	for(int k=0;k<m.col;k++) {
	    os << m.matrix[i][j][k];
	    if(k!=m.col-1) {
	        os << ", ";
	    }
	}
	os << "]";
	if(j!=m.row-1) {
	    os << ",\n";
	}
            }
            os << "]\n\n";
        }
        return os << "]";
    }

    std::string to_string() {
        std::string acc = "[\n";
        for(int i=0;i<length;i++) {
            acc += "  [";
            for(int j=0;j<row;j++) {
	if(j!=0) {
	    acc+="   ";
	}
	acc += "[";
	for(int k=0;k<col;k++) {
	    acc+=std::to_string(matrix[i][j][k]);
	    if(k!=col-1) {
	        acc+=", ";
	    }
	}
	acc+="]";
	if(j!=row-1) {
	    acc+=",\n";
	}
            }
            acc += "]\n\n";
        }
        return acc+"]";
    };

    bool equals(Matrix<T> a) {
        if(length!=a.length || row!=a.row || col!=a.col) {
            return false;
        }

        for(int i=0;i<length;i++) {
            for(int j=0;j<row;j++) {
	for(int k=0;k<col;k++) {
	    if(matrix[i][j][k]!=a.matrix[i][j][k]) {
	        return false;
	    }
	}
            }
        }

        return true;
    };

    void dimMiss() {
        std::cout << "ERROR: Dimension mismatch\n";
        std::exit(1);
    };

    void dimMiss(Matrix<T> a) {
        if(length!=a.length || row!=a.row || col!=a.col) {                  
            std::cout << "ERROR: Dimension mismatch\n";
            std::exit(1);
        }
    };

    void log() {
        std::cout << "(" << length << ", " << row << ", " << col << ")\n";
    }

    Matrix<T> mult(Matrix<T> a) {

        log();
        std::cout << (*this);
        a.log();
        std::cout << a;

        if(col!=a.row) {
            dimMiss();  
        }

        Matrix result(length, row, a.col);

        for(int z=0;z<length;z++) {	
            for(int i=0;i<row;i++) {
	for(int k=0;k<a.col;k++) {
	    T sum = 0;
	    for(int j=0;j<col;j++) {
	        sum += matrix[z][i][j] * a.matrix[z][j][k];
	    }
	    result.matrix[z][i][k] = sum;
	}
            }	
        }
            
        return result;
    };

    Matrix<T> for_each(std::function<T(T)> f) {
        Matrix<T> result(length, row, col);
        
        for(int i=0;i<length;i++) {
            for(int j=0;j<row;j++) {
	for(int k=0;k<col;k++) {
	    result.matrix[i][j][k] = f(matrix[i][j][k]);
	}
            }
        }

        return result;
    }
    
    Matrix<T> for_each(Matrix<T> a, std::function<T(T, T)> f) {
        dimMiss(a);
        
        Matrix<T> result(length, row, col); 
        
        for(int i=0;i<length;i++) {
            for(int j=0;j<row;j++) {
	for(int k=0;k<col;k++) {
	    result.matrix[i][j][k] = f(matrix[i][j][k], a.matrix[i][j][k]);
	}
            }
        }

        return result;
    };

    Matrix<T> add(Matrix<T> a) {

        log();
        std::cout << (*this);
        a.log();
        std::cout << a;

        if(a.length!=0 || row!=a.row || col!=a.col) {
            dimMiss();
        } 

        Matrix<T> result(length, row, col);
        
        for(int i=0;i<length;i++) {
            for(int j=0;j<row;j++) {
	for(int k=0;k<col;k++) {
	    result.matrix[i][j][k] = matrix[0][j][k] + a.matrix[0][j][k];
	}
            }
        }        
        
        return result;
    };

    Matrix<T> sub(Matrix<T> a) {

        std::function<T(T, T)> f = [](T n, T m) -> T {return n - m;}; 
        
        return for_each(a, f);
    };

    Matrix<T> scale(T b) {

        std::function<T(T)> f = [](T n) -> T {return n*1;};
        
        return for_each(f);
    };
    
    Matrix<T> transpose() {

        Matrix<T> result(length, col, row);
        
        for(int i=0;i<length;i++) {
            for(int j=0;j<row;j++) {
	for(int k=0;k<col;k++) {
	    result.matrix[i][k][j] = matrix[i][j][k];
	}
            }
        }

        return result;
    };
};


#endif //MATRIX_H
