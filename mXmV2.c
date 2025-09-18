// Version 2 - Matrix multiplication with threads
// No use of stdio.h (only pthreads and stdlib/time)

#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>

typedef struct {
    int *A;
    int *B;
    int *C;
    int n;
    int start_row;
    int end_row;
} ThreadData;

void fill_random(int *matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i * n + j] = rand() % 100;
        }
    }
}

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void *thread_multiply(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int n = data->n;
    int *A = data->A;
    int *B = data->B;
    int *C = data->C;

    for (int i = data->start_row; i < data->end_row; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        return 1;
    }

    int n = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    int *A = (int *)malloc(n * n * sizeof(int));
    int *B = (int *)malloc(n * n * sizeof(int));
    int *C = (int *)malloc(n * n * sizeof(int));

    if (A == NULL || B == NULL || C == NULL) {
        return 1;
    }

    srand(time(NULL));
    fill_random(A, n);
    fill_random(B, n);

    double start_time = get_time();

    pthread_t *threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    ThreadData *thread_data = (ThreadData *)malloc(num_threads * sizeof(ThreadData));

    int rows_per_thread = n / num_threads;
    int extra = n % num_threads;

    int current_row = 0;
    for (int t = 0; t < num_threads; t++) {
        int start = current_row;
        int end = start + rows_per_thread + (t < extra ? 1 : 0);
        current_row = end;

        thread_data[t].A = A;
        thread_data[t].B = B;
        thread_data[t].C = C;
        thread_data[t].n = n;
        thread_data[t].start_row = start;
        thread_data[t].end_row = end;

        pthread_create(&threads[t], NULL, thread_multiply, &thread_data[t]);
    }

    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
    }

    double end_time = get_time();
    double execution_time = end_time - start_time;
    execution_time = fabs(execution_time);
    printf("%.3f s\n", execution_time);

    free(A);
    free(B);
    free(C);
    free(threads);
    free(thread_data);

    return 0;
}
