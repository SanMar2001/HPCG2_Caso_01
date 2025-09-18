// Version 0
// Using Files to see the result of the product

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void fill_random(int *matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i * n + j] = rand() % 10;
        }
    }
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

void write_matrix(FILE *f, int *M, int n, const char *name) {
    fprintf(f, "%s:\n", name);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(f, "%3d ", M[i * n + j]);
        }
        fprintf(f, "\n");
    }
    fprintf(f, "\n");
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

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    multiply(A, B, C, n);
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time of execution: %.3f s\n", cpu_time_used);

    FILE *f = fopen("result.txt", "w");
    if (f == NULL) {
        return 1;
    }

    write_matrix(f, A, n, "Matriz A");
    write_matrix(f, B, n, "Matriz B");
    write_matrix(f, C, n, "Matriz C = A * B");

    fclose(f);
    free(A);
    free(B);
    free(C);

    return 0;
}
