// Version 3 - Matrix multiplication using forks

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
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

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    int num_procs = atoi(argv[2]);
    
    int shmA = shmget(IPC_PRIVATE, n * n * sizeof(int), IPC_CREAT | 0666);
    int shmB = shmget(IPC_PRIVATE, n * n * sizeof(int), IPC_CREAT | 0666);
    int shmC = shmget(IPC_PRIVATE, n * n * sizeof(int), IPC_CREAT | 0666);
    
    int *A = (int *)shmat(shmA, NULL, 0);
    int *B = (int *)shmat(shmB, NULL, 0);
    int *C = (int *)shmat(shmC, NULL, 0);
    
    srand(time(NULL));
    fill_random(A, n);
    fill_random(B, n);
    
    double start_time = get_time();
    
    int rows_per_proc = n / num_procs;
    for (int p = 0; p < num_procs; p++) {
        if (fork() == 0) {
            int start_row = p * rows_per_proc;
            int end_row = (p == num_procs - 1) ? n : (p + 1) * rows_per_proc;
            
            for (int i = start_row; i < end_row; i++) {
                for (int j = 0; j < n; j++) {
                    int sum = 0;
                    for (int k = 0; k < n; k++) {
                        sum += A[i * n + k] * B[k * n + j];
                    }
                    C[i * n + j] = sum;
                }
            }
            
            shmdt(A);
            shmdt(B);
            shmdt(C);
            _exit(0);
        }
    }
    
    for (int p = 0; p < num_procs; p++) {
        wait(NULL);
    }
    
    double end_time = get_time();
    double execution_time = end_time - start_time;
    execution_time = fabs(execution_time);
    printf("%.3f\n", execution_time);
    
    shmdt(A); shmdt(B); shmdt(C);
    shmctl(shmA, IPC_RMID, NULL);
    shmctl(shmB, IPC_RMID, NULL);
    shmctl(shmC, IPC_RMID, NULL);
    
    return 0;
}