#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

#define NUM_THREADS 10
#define NUM_INTEGERS 100

double sroot[NUM_INTEGERS];

void *sqr_root(void *threadid) {
    long tid = (long)threadid;
    int start = tid * 10;
    int end = start + 10;

    for (int i = start; i < end; i++) {
        sroot[i] = sqrt(i);
        // Simulate some processing time
        usleep(100000); // Sleep for 100 milliseconds
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    // Create threads
    for (t = 0; t < NUM_THREADS; t++) {
        printf("Creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, sqr_root, (void *)t);
        if (rc) {
            fprintf(stderr, "Error: Unable to create thread %ld, %d\n", t, rc);
            exit(EXIT_FAILURE);
        }
    }

    // Main thread will wait for all threads to complete
    printf("Main thread is waiting for all threads to finish...\n");

    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    // Display results
    printf("Square roots calculated:\n");
    for (int i = 0; i < NUM_INTEGERS; i++) {
        printf("sqrt(%d) = %.2f\n", i, sroot[i]);
    }

    pthread_exit(NULL);
}

