# Parallel Computation in C using POSIX Threads

This project demonstrates efficient workload distribution and performance comparison using multithreading in C.

## ✅ Features

### 1. Square Root Computation
- Utilizes 10 POSIX threads to compute square roots of integers from 0 to 99.
- Each thread handles 10 consecutive integers and writes to a shared result array.

### 2. Matrix Summation (Serial vs Parallel)
- Calculates the sum of a large matrix both serially and in parallel using user-defined number of threads.
- Automatically distributes rows across threads, handling uneven distribution.
- Measures and compares execution time using microsecond-precision timestamps.

## 🚀 Technologies Used
- C Programming
- POSIX Threads (`pthread`)
- Time Benchmarking with `gettimeofday`

## 📁 How to Compile & Run
```bash
gcc sqrt_threads.c -lpthread -o sqrt_threads
./sqrt_threads > sqrt_threads_output.txt

gcc matrix_sum.c -lpthread -o matrix_sum
./matrix_sum > matrix_sum_output.txt
