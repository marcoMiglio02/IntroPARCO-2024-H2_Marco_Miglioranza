#include "matrix_operation.h"
#include <mpi.h>
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

void matTranspose(Matrix & T, Matrix & M) {
size_t N = M.get_rows();
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < N; j++) {
      T(j,i) = M(i,j);
    }
  }
}


bool checkSym(Matrix & M) {
size_t N = M.get_rows();
  for (size_t i = 0; i < N; i++)
    for (size_t j = 0; j < N; j++)
      if (M(i,j) != M(j,i)) return false;
  return true;
}

void matTransposeBlock(Matrix & T, Matrix & M) {
  size_t N = M.get_rows();
  int BLOCK_SIZE = 32; // Cache size friendly

  // Block size trasposition
  for (size_t i = 0; i < N; i += BLOCK_SIZE) {
    for (size_t j = 0; j < N; j += BLOCK_SIZE) {
      // transpose the block beginning at [i,j]
      for (size_t ii = i; ii < i + BLOCK_SIZE && ii < N; ++ii) {
        for (size_t jj = j; jj < j + BLOCK_SIZE && jj < N; ++jj) {
          T(j,i) = M(i,j);
        }
      }
    }
  }
}
