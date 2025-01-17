#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "matrix_operation.h"
#include "file_operation.h"

using namespace std;
 
int main(int argc, char ** argv) {
  MPI_Init( & argc, & argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & size);

  if (argc < 3) {
    if (rank == 0) {
      cerr << "Usage: mpirun -np <num_processes> ./program <matrix_size> <number_of_tries>" << endl;
    }
    MPI_Finalize();
    return -1;
  }

  const int n = atoi(argv[1]);
  if (size > n) {
     if(rank == 0)
	 cerr << "Error: <matrix_size> gt <n_proc>" << endl;
    
    MPI_Finalize();
    return -1;
    }
  Matrix M(n, n);
  Matrix T(n, n);

  if (rank == 0) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
//        M(i,j) = i * n + j;
      }
    }
  }
  const int TRIES = atoi(argv[2]);
  vector < string > headers = {
    "MPI Transpose",
    "MPI CheckSym",
    "MPI Block Transpose",
    "Serial Transpose",
    "Serial CheckSym",
    "Block Transpose",
    "N", 
    "Process"
  };
  vector < double > results(headers.size(), 0.0);
  

  for (int iter = 0; iter < TRIES; ++iter) {
    double start_time, end_time;
    start_time = MPI_Wtime();
    matTransposeMPI(T, M, rank, size);
    end_time = MPI_Wtime();
    if (rank == 0) results[0] += (end_time - start_time);
    start_time = MPI_Wtime();
    bool result = checkSymMPI(M, rank, size);
    end_time = MPI_Wtime();
    if (rank == 0) results[1] += (end_time - start_time);
    
    start_time = MPI_Wtime();
    matTransposeBlockMPI(T, M, rank, size);
    end_time = MPI_Wtime();
    if (rank == 0) results[2] += (end_time - start_time);

    if (rank == 0) {
      start_time = MPI_Wtime();
      matTranspose(T, M);
      end_time = MPI_Wtime();
      results[3] += (end_time - start_time);

      start_time = MPI_Wtime();
      result = checkSym(M);
      end_time = MPI_Wtime();
      results[4] += (end_time - start_time);


      start_time = MPI_Wtime();
      matTransposeBlock(T, M);
      end_time = MPI_Wtime();
      results[5] += (end_time - start_time);
      
    }
  }

  if (rank == 0) {
      
      for (int i = 0; i < results.size(); i++) {
        results[i] = results[i] / TRIES;
      }
      
      results[6] = n;
      results[7] = size;
      saveResultsToCSV(results, headers);
  }

  MPI_Finalize();
  return 0;
}
