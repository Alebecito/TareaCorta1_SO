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

// void copy_matrix(int **original_matrix, int **copy_matrix, int matrix_size)
// {
//     // for (int i = 0; i < rows; i++) {
//     //     for (int j = 0; j < columns; j++) {
//     //         copy_matrix[i][j] = original_matrix[i][j];
//     //     }
//     // }
//     memcpy(copy_matrix, original_matrix, matrix_size * matrix_size * sizeof(int));
// }

void create_processes(int matrix_size, int (*result)[matrix_size], int **matrixA, int **matrixB, int **matrixC)
{
    pid_t p_ids[matrix_size];
    pid_t pid;
    for (int i = 0; i < matrix_size; i++)
    {
        pid = fork();
        p_ids[i] = pid;

        // if (pid == 0)
        // {
        //     printf("[son] pid %d from [parent] pid %d\n", getpid(), getppid());
        //     exit(0);
        // }

        // if (pid == -1)
        // {
        //     printf("Error creating child process\n");
        //     exit(0);
        // }
        if (pid == 0) // fork returns 0 to the child process so it enters "if" block
        {
            // close(p[0]);
            // printf("[son] pid %d from [parent] pid %d\n", getpid(), getppid());
            multiply_row_by_matrix(matrixA[i], matrix_size, i, matrixB, matrixC);

            // copy the result to the shared memory segment
            // for (int j = 0; j < matrix_size; j++)
            // {
            //     // just copy the new row to the shared memory segment
            //     // if not matrixC will have a weird behavior
            //     result[i][j] += matrixC[i][j];
            // }
            // result[i] = *matrixC[i];
            // printf("\t hola from %d\n", i);
            memcpy(result[i], matrixC[i], matrix_size * sizeof(int));

            exit(0);
        }
        else
        {
            wait(&pid);
        }
    }
    // if (pid != 0)
    // {
    //     int cpid;
    //     int status;
    //     while ((cpid = wait(&status)) > 0)
    //         printf("Parent (%d): child (%d) exited with status 0x%.4x\n", (int)getpid(), cpid, status);
    //     ;
    // }
    // printf("============================\n");
    // fputs("JUPON\n", stats_file);

    // copy_matrix(matrixC, (int **)result, matrix_size);

    // printf("Matrix RESULT:\n");
    // for (int i = 0; i < matrix_size; i++)
    // {
    //     for (int j = 0; j < matrix_size; j++)
    //     {
    //         printf("%d ", result[i][j]);
    //     }
    //     printf("\n");
    // }

    // return (int **)result;
    // return matrixC;

    // printf("Matrix C:\n");
    // print_matrix(matrixC, matrix_size);
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

    for (int i = 0; i < 100; i++)
    {
        stats_file = fopen("Stats.txt", "a");

        int **A = generate_random_matrix(matrix_size),
            **B = generate_random_matrix(matrix_size),
            **C = generate_random_matrix(matrix_size);

        // print the results
        // printf("Matrix A:\n");
        // print_matrix(A, matrix_size);
        // printf("Matrix B:\n");
        // print_matrix(B, matrix_size);

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

        // printf("Matrix RESULT:\n");
        // for (int i = 0; i < matrix_size; i++)
        // {
        //     for (int j = 0; j < matrix_size; j++)
        //     {
        //         printf("%d ", result[i][j]);
        //     }
        //     printf("\n");
        // }

        char filename[12];
        sprintf(filename, "Mat_%s%d.txt", i < 10 ? "0" : "", i);
        write_matrix_memseg(filename, matrix_size, result);

        double diff = (end.tv_sec - start.tv_sec);
        diff += (end.tv_nsec - start.tv_nsec) / 1000000000.0;

        char iteration[18];
        sprintf(iteration, "Iteration_%s%d: ", i < 10 ? "0" : "", i);

        write_stats(diff, iteration);
        // fprintf(stats_file, "JUPON\n");
        // fputs("JUPON\n", stats_file);

        // printf("diff in %f: %d\n", i, diff);
        average += diff;

        // printf("Time: %f seconds\n", diff);
        fclose(stats_file);
        free(A), free(B), free(C);
        shmdt(result); // free segment
        shmctl(buff, IPC_RMID, NULL);
    }
    stats_file = fopen("Stats.txt", "a");

    // printf("AVERAGE: %f seconds\n", average);
    // printf("Average time: %f seconds\n", average / 100);
    write_stats(average / 100, "Average:");
    return 0;
    // allocate memory for the matrices and initialize them with random values
    // int **A = generate_random_matrix(matrix_size),
    //     **B = generate_random_matrix(matrix_size),
    //     **C = generate_random_matrix(matrix_size);

    // // print the results
    // char filename[12];
    // sprintf(filename, "Mat_%s%d.txt", 5 < 10 ? "0" : "" , 5);
    // write_matrix(filename, A, matrix_size);
    // printf("Matrix A:\n");
    // print_matrix(A, matrix_size);
    // printf("Matrix B:\n");
    // print_matrix(B, matrix_size);

    // // ============= NEW CODE =============
    // // memory segment (ID)
    // int buff;
    // // array to share
    // int(*result)[matrix_size]; // pointer to array

    // buff = shmget(IPC_PRIVATE, sizeof(int[matrix_size][matrix_size]), IPC_CREAT | 0666);
    // // result receives the memory segment from buff
    // result = shmat(buff, 0, 0);

    // clock_gettime(CLOCK_MONOTONIC, &start);

    // create_processes(matrix_size, result, A, B, C);

    // // ============= FINIH =============

    // shmctl(buff, IPC_RMID, NULL); // free segment

    // clock_gettime(CLOCK_MONOTONIC, &end);

    // // printf("Matrix RESULT:\n");
    // // for (int i = 0; i < matrix_size; i++)
    // // {
    // //     for (int j = 0; j < matrix_size; j++)
    // //     {
    // //         printf("%d ", result[i][j]);
    // //     }
    // //     printf("\n");
    // // }

    // // printf("RESULT\n");
    // // print_matrix(new_matrix_result, matrix_size);

    // double diff = (end.tv_sec - start.tv_sec);
    // diff += (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    // printf("Time: %f seconds\n", diff);

    // while(wait(NULL) > 0) {
    //     printf("TODAVIA HAY SUBPROCESOS");
    // }
}