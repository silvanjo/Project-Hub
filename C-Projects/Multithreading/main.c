#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Function that will be executed by each thread
void* threadFunction(void* arg) {
    int thread_id = *(int*)arg;
    printf("Thread %d is starting...\n", thread_id);

    // Simulate some work with sleep
    sleep(1);

    printf("Thread %d is finishing...\n", thread_id);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[5];  // Array to store thread identifiers
    int thread_ids[5];     // Array to store thread arguments
    int result;

    // Create 5 threads
    for (int i = 0; i < 5; i++) {
        thread_ids[i] = i;
        printf("Creating thread %d...\n", i);
        result = pthread_create(&threads[i], NULL, threadFunction, &thread_ids[i]);

        if (result != 0) {
            printf("Error creating thread %d: %d\n", i, result);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
        printf("Thread %d has finished.\n", i);
    }

    printf("All threads completed.\n");
    return 0;
}

