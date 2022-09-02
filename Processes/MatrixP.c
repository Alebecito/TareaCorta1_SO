#include <string.h>
#include <stdio.h>
#include <unistd.h> // for sleep()
#include <stdlib.h>
#include <math.h>
#include "../matrices.h"
#include "stdlib.h"
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <time.h>


void create_processes(int matrix_size, int (*result)[matrix_size], int **matrixA, int **matrixB, int **matrixC)
{
    pid_t p_ids[matrix_size];
    pid_t pid;
    for (int i = 0; i < matrix_size; i++)
    {
        // printf("=========\n");
        pid = fork();
        p_ids[i] = pid;

        if (pid == 0) // fork returns 0 to the child process so it enters "if" block
        {
            multiply_row_by_matrix(matrixA[i], matrix_size, i, matrixB, matrixC);
            memcpy(result[i], matrixC[i], matrix_size * sizeof(int));
            free(matrixC[i]);
            exit(0);
        }
    }
    if (pid != 0)
    {
        int cpid;
        int status;
        while ((cpid = wait(&status)) > 0)
            sleep(0.000001);
            ;
        ;
    }
}

struct timespec start, end;

int main()
{
    stats_file = fopen("Stats.txt", "w");

    srand(time(0));

    // receive a number and check if it is positive
    int matrix_size;
    printf("Enter the size N for the matrices: ");
    scanf("%d", &matrix_size);

    double average = 0;

    // for (int i = 0; i < 100; i++)
    for (int i = 0; i < 100; i++)
    {
        stats_file = fopen("Stats.txt", "a");

        int **A = generate_random_matrix(matrix_size),
            **B = generate_random_matrix(matrix_size),
            **C = generate_random_matrix(matrix_size);


        // ============= NEW CODE =============
        // memory segment (ID)
        int buff;
        // array to share
        int(*result)[matrix_size]; // pointer to array

        buff = shmget(IPC_PRIVATE, sizeof(int[matrix_size][matrix_size]), IPC_CREAT | 0666);
        // result receives the memory segment from buff
        result = shmat(buff, 0, 0);

        clock_gettime(CLOCK_MONOTONIC, &start);

        create_processes(matrix_size, result, A, B, C);

        // ============= FINIH =============

        clock_gettime(CLOCK_MONOTONIC, &end);

        char filename[12];
        sprintf(filename, "Mat_%s%d.txt", i < 10 ? "0" : "", i);
        write_matrix_memseg(filename, matrix_size, result);

        double diff = (end.tv_sec - start.tv_sec);
        diff += (end.tv_nsec - start.tv_nsec) / 1000000000.0;

        char iteration[18];
        sprintf(iteration, "Iteration_%s%d: ", i < 10 ? "0" : "", i);
        printf("%s%f\n", iteration, diff);

        write_stats(diff, iteration);
        average += diff;

        // printf("Time: %f seconds\n", diff);
        fclose(stats_file);
        for (int z = 0; z < matrix_size; z++)
        {
            free(A[z]);
            free(B[z]);
            free(C[z]);
        }
        free(A), free(B), free(C);
        shmdt(result); // free segment
        shmctl(buff, IPC_RMID, NULL);
    }
    stats_file = fopen("Stats.txt", "a");

    // printf("AVERAGE: %f seconds\n", average);
    // printf("Average time: %f seconds\n", average / 100);
    write_stats(average / 100, "Average:");
    return 0;
}