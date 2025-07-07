#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/time.h>

#define ROWS 500
#define COLS 500

int mat[ROWS][COLS];

// Structure to pass data to threads
typedef struct {
    int thread_id;
    int num_threads;
    long long local_sum;
    int rows_per_thread;
    int extra_rows;
} thread_data;

uint64_t get_gtod_clock_time() {
    struct timeval tv;
    if (gettimeofday(&tv, NULL) == 0)
        return (uint64_t)(tv.tv_sec * 1000000 + tv.tv_usec);
    else
        return 0;
}

// Serial matrix sum function
long long serial_matrix_sum() {
    long long total_sum = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            total_sum += mat[i][j];
        }
    }
    return total_sum;
}

// Thread function for parallel summation
void *parallel_matrix_sum(void *arg) {
    thread_data *data = (thread_data *)arg;
    int start_row = data->thread_id * data->rows_per_thread;
    int end_row = start_row + data->rows_per_thread;
    
    // Handle extra rows
    if (data->thread_id < data->extra_rows) {
        end_row++;
    }

    long long local_sum = 0;
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < COLS; j++) {
            local_sum += mat[i][j];
        }
    }
    data->local_sum = local_sum;
    pthread_exit(NULL);
}

int main() {
    // Initialize the matrix with random values
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            mat[i][j] = rand() % 100; // Fill with random numbers
        }
    }

    int num_threads;
    printf("Enter the number of threads (less than %d): ", ROWS);
    scanf("%d", &num_threads);

    // Ensure valid number of threads
    if (num_threads <= 0 || num_threads > ROWS) {
        fprintf(stderr, "Invalid number of threads.\n");
        return EXIT_FAILURE;
    }

    // Measure time for serial summation
    uint64_t start_time, end_time, time_diff;

    start_time = get_gtod_clock_time();
    long long serial_sum = serial_matrix_sum();
    end_time = get_gtod_clock_time();
    time_diff = end_time - start_time;
    printf("Serial sum: %lld, Time taken: %" PRIu64 " microseconds\n", serial_sum, time_diff);

    // Prepare for parallel summation
    pthread_t threads[num_threads];
    thread_data thread_args[num_threads];
    long long total_sum = 0;

    int rows_per_thread = ROWS / num_threads;
    int extra_rows = ROWS % num_threads;

    // Measure time for parallel summation
    start_time = get_gtod_clock_time();

    for (int i = 0; i < num_threads; i++) {
        thread_args[i].thread_id = i;
        thread_args[i].num_threads = num_threads;
        thread_args[i].local_sum = 0;
        thread_args[i].rows_per_thread = rows_per_thread;
        thread_args[i].extra_rows = extra_rows;
        pthread_create(&threads[i], NULL, parallel_matrix_sum, (void *)&thread_args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        total_sum += thread_args[i].local_sum;
    }

    end_time = get_gtod_clock_time();
    time_diff = end_time - start_time;
    
    printf("Parallel sum: %lld, Time taken: %" PRIu64 " microseconds\n", total_sum, time_diff);

    return 0;
}

