#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include "matrix_operation.h"

using namespace std;

void print_matrix(const Matrix& mat) {
    size_t rows = mat.get_rows();
    size_t cols = mat.get_cols();
    
    
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            std::cout << mat(i, j) << " ";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

void matTransposeMPI(Matrix& T,  Matrix& M, int rank, int size) {
    size_t N = M.get_rows();
    if (size > N) {
      if (rank == 0) {
        cerr << "Error: np > size" << endl;
        return;
      }
    }
    MPI_Bcast(M.data_ptr(), M.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    
    // Calculate block sizes
    int rowsPerProc = N / size;
    int remainder = N % size;
    
    // Calculate local block dimensions
    int startRow = (rank * rowsPerProc) + min(rank, remainder);
    int endRow = startRow + rowsPerProc + (rank < remainder ? 1 : 0);
    int localRows = endRow - startRow;
    
    // Create local matrix for this process's block
    Matrix localBlock(localRows, N);
    
    // local direct transposition input matrix
    for (int i = startRow; i < endRow; i++) {
        for (int j = 0; j < N; j++) {
            localBlock(i-startRow, j) = M(j, i);
        }
    }
    
    
    // Gather all transposed blocks to process 0
    MPI_Gather(localBlock.data_ptr(), N * localRows, MPI_DOUBLE,
           T.data_ptr(), N * localRows, MPI_DOUBLE,
           0, MPI_COMM_WORLD);

}


bool checkSymMPI(Matrix & M, int rank, int size) {
    size_t N = M.get_rows(); 
    MPI_Bcast(M.data_ptr(), M.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);    
    bool localSym = true;

    int rowsPerProcess = N / size;
    int remainder = N % size;
    int startRow = rank * rowsPerProcess + min(rank, remainder);
    int localRows = rowsPerProcess + (rank < remainder ? 1 : 0);


    if (size > N) {
        localSym = true;  
    }

    // Local symmetry check
    for (int i = startRow; i < startRow + localRows && localSym; ++i) {
        for (int j = i + 1; j < N && localSym; ++j) {
            if (M(i, j) != M(j, i)) {
                localSym = false;
            }
        }
    }

    // Gather results
    bool globalSym = true;
    MPI_Reduce(&localSym, &globalSym, 1, MPI_C_BOOL, MPI_LAND, 0, MPI_COMM_WORLD);


    return globalSym;
}


void matTransposeBlockMPI(Matrix& T, Matrix& M, int rank, int size) {
    size_t N = M.get_rows();
    
    int gridSize = (int)sqrt(size);

    // Check if the number of processes is a perfect square and a divisor of N
    if (gridSize * gridSize != size) {
        if (rank == 0) {
            cerr << "Error: the number of processes must be a perfect square." << endl;
        }
        return;
    }

    if (N % gridSize != 0) {
        if (rank == 0) {
            cerr << "Error: N must be perfectly divisible by the square of the number of processes." << endl;
        }
        return;
    }

    size_t localBlockSize = N / gridSize;
    Matrix localBlock(localBlockSize, localBlockSize);
    Matrix localTranspose(localBlockSize, localBlockSize);

    MPI_Datatype blockType;
    MPI_Type_vector(localBlockSize, localBlockSize, N, MPI_DOUBLE, &blockType);

    MPI_Datatype resizedBlockType;
    MPI_Type_create_resized(blockType, 0, sizeof(double), &resizedBlockType);
    MPI_Type_commit(&resizedBlockType);

    vector<int> disps(size);
    vector<int> counts(size, 1);

    for (int ii = 0; ii < gridSize; ii++) {
        for (int jj = 0; jj < gridSize; jj++) {
            disps[ii * gridSize + jj] = (ii * localBlockSize * N) + (jj * localBlockSize);
        }
    }

    MPI_Scatterv(M.data_ptr(), counts.data(), disps.data(), resizedBlockType, localBlock.data_ptr(),
                 localBlockSize * localBlockSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);


    // Local Transposition
    for (size_t i = 0; i < localBlockSize; ++i) {
        for (size_t j = 0; j < localBlockSize; ++j) {
            localTranspose(j, i) = localBlock(i, j);
        }
    }


    for (int ii = 0; ii < gridSize; ii++) {
        for (int jj = 0; jj < gridSize; jj++) {
            disps[ii * gridSize + jj] = (jj * localBlockSize * N) + (ii * localBlockSize);
        }
    }

    MPI_Gatherv(localTranspose.data_ptr(), localBlockSize * localBlockSize, MPI_DOUBLE,
                T.data_ptr(), counts.data(), disps.data(), resizedBlockType, 0, MPI_COMM_WORLD);

    MPI_Type_free(&blockType);
    MPI_Type_free(&resizedBlockType);
}

