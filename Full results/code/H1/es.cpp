#include <iostream>
#include <chrono>
#include <immintrin.h>  // for AVX-512 intrinsics
#include <cstddef>
#include<random>
#include<omp.h>
#include <fstream>
#define BLOCK_SIZE 32
#define PREFETCH_DISTANCE 16


using namespace std;



void matTransposeOMP(double** T, double** M, size_t N) {
    
    #pragma omp parallel for collapse(2)
    for (size_t i = 0; i < N; i += BLOCK_SIZE) {
        for (size_t j = 0; j < N; j += BLOCK_SIZE) {
            // transpose the block beginning at [i,j]
            for (size_t ii = i; ii < i + BLOCK_SIZE && ii < N; ++ii) {
                for (size_t jj = j; jj < j + BLOCK_SIZE && jj < N ; ++jj) {
                  
                    T[jj][ii] = M[ii][jj];
                }
            }
        }
    }
}

bool checkSymOMP(double** M, size_t N) {

    bool symmetric = true;

     #pragma omp parallel
    {
        bool local_symmetric = true;
        #pragma omp for schedule(guided) nowait
        for (size_t i = 0; i < N-1; ++i) {
            if (!symmetric) continue;  // Skip remaining work if asymmetry found
            for (size_t j = i + 1; j < N; ++j) {
                if (M[i][j] != M[j ][i]) {
                    local_symmetric = false;
                    symmetric = false;
                    break;  // Exit inner loop
                }
            }
        }
        #pragma omp critical
        symmetric = symmetric && local_symmetric;
    }
    
    return symmetric;
}

 




int main(int argc, char** argv) {
    
    
    if(argc < 4) {
       cerr << "Usage: ./program <size_matrix> <threads> <tries>" << endl;
	return -1;
    }
    
    size_t N = atoi(argv[1]);
    if (N < 16 || N > 4096) {
        cerr << "ERROR";
        return -1;
    }
    int thr = atoi(argv[2]);
    int tries = atoi(argv[3]);

    omp_set_num_threads(thr);
    auto** M = new double*[N];
    auto** T = new double*[N];    
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(1.0, 10.0);

    //Creating and initializing the Matrix
    for (size_t i = 0; i < N; i++) {
        M[i] = new double[N];
        T[i] = new double[N];

        for (size_t j = 0; j < N; j++)
            M[i][j] =  2;
    }

    double total_time_transpose = 0.0;
    double total_time_check = 0.0;
    vector<double> times_transpose, times_check;

    for (int i = 0; i < tries; i++) {
        auto start = chrono::high_resolution_clock::now();
        matTransposeOMP(T, M, N);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> time = end - start;
        total_time_transpose += time.count();
        times_transpose.push_back(time.count());

        

        start = chrono::high_resolution_clock::now();
        bool check = checkSymOMP(M, N);
        end = chrono::high_resolution_clock::now();
        time = end - start;
        total_time_check += time.count();
        times_check.push_back(time.count());

        }

    double avg_time_transpose = total_time_transpose / tries;
    double avg_time_check = total_time_check / tries;

    // Save results to a CSV file
    ofstream csv_file("../results/OpenMPresults.csv", ios::app);
    csv_file.seekp(0, ios::end);
    if (csv_file.tellp() == 0) {
        csv_file << "Threads,Matrix Size,Transpose Time,Check Symmetry Time\n";
        }
        
    
    
    csv_file << thr << "," << N << "," << avg_time_transpose << "," << avg_time_check << "\n";
    
    csv_file.close();

    return 0;
}




