# High Performance Matrix Operations: MPI Implementation

## Overview

This project is a continuation of the previous Project [LINK] and implements and optimizes matrix operations, such as matrix transposition and symmetry verification by using Message Passing Interface (MPI). It evaluates and compares the efficiency of MPI-based parallelization with OpenMP and sequential methods. 


---

## **Contents**

1. **Introduction**
2. **Methodology**
3. **Algorithms**
4. **How to run the code**
5. **Performance Analysis**

---

### 1. Introduction

Matrix transposition and symmetry verification are critical operations. This project explores explicit parallelization techniques using MPI to accelerate these operations and provides a detailed performance analysis to identify the best approaches for different matrix sizes and system configurations.

### 2. Methodology

#### Computational Environment:

- **CPU**: Intel Xeon Gold 6252N with 96 logical CPUs.
- **Technologies:**
  - **MPI:** For distributed memory parallelization.
  - **OpenMP:** For shared memory parallelization (comparison).
- **Compiler:** GNU Compiler Collection (g++) and MPICH with performance-focused flags



### 3. Algorithms:

1. **Sequential Implementation**:
   - Baseline for comparison.
   - Functions: `matTranspose` and `checkSym`.
2. **MPI-based Implementation**:
   - Parallel matrix transposition (`matTransposeMPI`).
   - Symmetry verification (`checkSymMPI`).
   - Optional block-based matrix transposition (`matTransposeBlockMPI`).

### 4. How to Run the Code

#### **Dependencies**

- **MPI Library:** OpenMPI or MPICH
- **GCC:** Version supporting C++11 or later
- **PBS/Torque:** For HPC job scheduling (optional)

#### Compilation and Execution with PBS file:

Ensure that the system supports MPI and that the necessary libraries are installed.

```bash
# Clone the repository
git clone https://github.com/marcoMiglio02/IntroPARCO-2024-H2_Marco_Miglioranza.git

# Navigate to the code directory
cd IntroPARCO-2024-H2_Marco_Miglioranza/report/

# Excute the PBS file
qsub test.pbs
```
#### Compilation without PBS file:
```bash
# Clone the repository
git clone https://github.com/marcoMiglio02/IntroPARCO-2024-H2_Marco_Miglioranza.git

# Navigate to the code directory
cd IntroPARCO-2024-H2_Marco_Miglioranza/report/code/H2

# Compile the MPI implementation
mpicxx -O3 -march=native -mtune=native -funroll-loops -o code main.cpp MPI_operation.cpp file_operation.cpp serial_operation.cpp

# Compile the old OpenMP implementation
cd ../H1/
g++ -O2 -march=native -ffast-math -fopenmp -funroll-loops -o es es.cpp
```
#### Execution:

Run the program with varying numbers of processes (MPI Implementation):

```bash
# From the directory H2 where there is the MPI implementation
mpirun -np <num_processes> ./code <size_matrix> <tries>
```

Run the program with varying numbers of threads (OpenMP Implementation):

```bash
# From the directory H1 where there is the OpenMP implementation
./es <size_matrix> <num_threads> <tries>
```

### 5. Performance Analysis

#### Metrics Evaluated:

- **Execution Time**: Measured for sequential, MPI, and OpenMP implementations.
- **Speedup and Efficiency**: Calculated relative to the sequential baseline.
- **Scalability**: Analysis of weak and strong scaling.

#### Key Findings:

1. **MPI**:
   - Superior scalability for large matrices due to distributed memory architecture.
   - Effective in reducing computation time with an increasing number of processors if not calculated the required time on Message passing between processes.
2. **OpenMP**:
   - Optimal for medium-sized matrices due to shared memory advantages.
   - The best solution if code is inside in only one node
3. **Sequential**:
   - Inefficient for large matrices, but useful as a baseline for comparisons.

### 5. Results

Performance results for matrix sizes ranging from  to  are presented in the full report.

### 6. Conclusion

This project demonstrated the effectiveness of MPI for parallelizing matrix operations, particularly for large matrices. It highlights the complementary strengths of MPI and OpenMP, with MPI excelling in distributed environments and OpenMP providing efficient solutions for shared memory systems.

## GIT Repository

[GitHub Repository](https://github.com/marcoMiglio02/IntroPARCO-2024-H2_Marco_Miglioranza)

### Reproducibility

1. Clone the repository.
2. Follow the compilation and execution instructions above.
3. Ensure that the system meets the computational environment requirements detailed in the methodology.

---

For more details on the implementation and analysis, please refer to the [full report](https://github.com/marcoMiglio02/IntroPARCO-2025-H2_Marco_Miglioranza/blob/32a56a07b59da3a7ddfcd87cc72f71b29e29550f/Marco_Miglioranza_Report_H2.pdf).

