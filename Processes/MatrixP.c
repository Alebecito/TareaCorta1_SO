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

    for (int i = 0; i < matrix_size; i++)
    {
        pid_t c = fork();

        if (c == -1)
        {
            printf("Error creating child process\n");
            exit(0);
        }
        else if (c == 0) // fork returns 0 to the child process so it enters "if" block
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
            memcpy(result[i], matrixC[i], matrix_size * sizeof(int));

            exit(0);
        }
        else
        {
            wait(&c);
        }
    }

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
    // receive a number and check if it is positive
    int matrix_size;
    printf("Enter the size N for the matrices: ");
    scanf("%d", &matrix_size);

    // allocate memory for the matrices and initialize them with random values
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

    shmctl(buff, IPC_RMID, NULL); // free segment

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

    // printf("RESULT\n");
    // print_matrix(new_matrix_result, matrix_size);

    double diff = (end.tv_sec - start.tv_sec);
    diff += (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("Time: %f seconds\n", diff);

    // while(wait(NULL) > 0) {
    //     printf("TODAVIA HAY SUBPROCESOS");
    // }
}