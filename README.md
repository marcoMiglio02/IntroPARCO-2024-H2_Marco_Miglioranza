# High Performance Matrix Operations: MPI Implementation

## Overview
This project implements and optimizes matrix operations, specifically matrix transposition and symmetry verification, using Message Passing Interface (MPI). It evaluates and compares the efficiency of MPI-based parallelization with OpenMP and sequential methods. The focus is on leveraging parallel computing to achieve significant performance improvements in scientific and high-performance computing environments.

## Contents

### 1. Introduction
Matrix transposition and symmetry verification are critical operations in various computational fields, including robotics, physics, machine learning, and data processing. This project explores explicit parallelization techniques using MPI to accelerate these operations and provides a detailed performance analysis to identify the best approaches for different matrix sizes and system configurations.

### 2. Methodology
#### Computational Environment:
- **CPU**: Intel Xeon Gold 6252N with 96 logical CPUs.
- **Technologies**:
  - MPI for explicit parallelization.
  - OpenMP and sequential implementations for comparison.
- **Compiler**: GNU Compiler Collection (g++) with optimization flags.

#### Tools and Libraries:
- MPI (Message Passing Interface).
- Performance monitoring tools for benchmarking and analysis.

#### Algorithms:
1. **Sequential Implementation**:
   - Baseline for comparison.
   - Functions: `matTranspose` and `checkSym`.
2. **MPI-based Implementation**:
   - Parallel matrix transposition (`matTransposeMPI`).
   - Symmetry verification (`checkSymMPI`).
   - Optional block-based matrix transposition (`matTransposeBlockMPI`).

### 3. How to Run the Code
#### Compilation:
Ensure that the system supports MPI and that the necessary libraries are installed.

```bash
# Clone the repository
git clone https://github.com/marcoMiglio02/IntroPARCO-2024-H2_Marco_Miglioranza.git

# Navigate to the code directory
cd IntroPARCO-2024-H2_Marco_Miglioranza/report/code

# Compile the MPI implementation
mpic++ -O2 -march=native -ffast-math -o matrix_ops matrix_ops.cpp
```

#### Execution:
Run the program with varying numbers of processes:

```bash
# Execute with 4 processes
mpirun -np 4 ./matrix_ops
```

### 4. Performance Analysis
#### Metrics Evaluated:
- **Execution Time**: Measured for sequential, MPI, and OpenMP implementations.
- **Speedup and Efficiency**: Calculated relative to the sequential baseline.
- **Scalability**: Analysis of weak and strong scaling.

#### Key Findings:
1. **MPI**:
   - Superior scalability for large matrices due to distributed memory architecture.
   - Effective in reducing computation time with an increasing number of processors.
2. **OpenMP**:
   - Optimal for medium-sized matrices due to shared memory advantages.
3. **Sequential**:
   - Inefficient for large matrices, but useful as a baseline for comparisons.

### 5. Results
Performance results for matrix sizes ranging from \(2^4 \times 2^4\) to \(2^{12} \times 2^{12}\) are presented in the full report. Comparative graphs illustrate the speedup and efficiency of MPI, OpenMP, and sequential approaches.

### 6. Conclusion
This project demonstrated the effectiveness of MPI for parallelizing matrix operations, particularly for large matrices. It highlights the complementary strengths of MPI and OpenMP, with MPI excelling in distributed environments and OpenMP providing efficient solutions for shared memory systems. The findings contribute to advancements in high-performance computing for dense matrix operations.

## GIT Repository
[GitHub Repository](https://github.com/marcoMiglio02/IntroPARCO-2024-H2_Marco_Miglioranza)

### Reproducibility
1. Clone the repository.
2. Follow the compilation and execution instructions above.
3. Ensure that the system meets the computational environment requirements detailed in the methodology.

---

For more details on the implementation and analysis, please refer to the [full report](https://github.com/marcoMiglio02/IntroPARCO-2024-H2_Marco_Miglioranza/blob/main/report.pdf).

