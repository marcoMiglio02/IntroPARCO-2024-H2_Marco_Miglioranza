#ifndef MATRIX_OPERATION_H
#define MATRIX_OPERATION_H

#include <vector>
#include <cstddef>

using namespace std;
class Matrix {
private:
    vector<double> data;
    size_t rows;
    size_t cols;

public:
    Matrix(size_t r, size_t c) : data(r * c, 0.0), rows(r), cols(c) {}
    
    // Copy constructor
    Matrix(const Matrix& other) : data(other.data), rows(other.rows), cols(other.cols) {}
    
    // Access operators
    double& operator()(size_t i, size_t j) {
        return data[i * cols + j];
    }
    
    const double& operator()(size_t i, size_t j) const {
        return data[i * cols + j];
    }
    
    // Getters
    double* data_ptr() { return data.data(); }
    const double* data_ptr() const { return data.data(); }
    size_t get_rows() const { return rows; }
    size_t get_cols() const { return cols; }
    size_t size() const { return data.size(); }
    
    // Transpose helper for local blocks
    void transpose_block(Matrix& result, size_t start_row, size_t num_rows) const {
        for (size_t i = 0; i < num_rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                result(j, start_row + i) = (*this)(start_row + i, j);
            }
        }
    }
};

void print_matrix(const Matrix& mat);

// Function declarations for matrix operations
void matTranspose(Matrix & T, Matrix & M);
bool checkSym(Matrix & M);
void matTransposeBlock(Matrix & T, Matrix & M);

// MPI-based matrix operations
void matTransposeMPI(Matrix & T, Matrix & M, int rank, int size);
bool checkSymMPI(Matrix & M, int rank, int size);
void matTransposeBlockMPI(Matrix & T, Matrix & M, int rank, int size);



#endif // MATRIX_OPERATION_H
