// Version 1
// Without any function of the library stdio.h

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>

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

void multiply(int *A, int *B, int *C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }
    int n = atoi(argv[1]);
    
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
    multiply(A, B, C, n);
    double end_time = get_time();
    double execution_time = end_time - start_time;
    execution_time = fabs(execution_time);
    printf("%.3f s\n", execution_time);

    free(A);
    free(B);
    free(C);

    return 0;
}
