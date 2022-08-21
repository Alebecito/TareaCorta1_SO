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


void create_processes(int **matrixA, int **matrixB, int **matrixC, int matrix_size)
{
    int buff;
    int (*result)[matrix_size]; // pointer to array

    buff = shmget(IPC_PRIVATE, sizeof(int[matrix_size][matrix_size]), IPC_CREAT | 0666);
    result = shmat(buff, 0, 0);

    for(int i = 0; i < matrix_size; i++){
        for(int j = 0; j < matrix_size; j++){
            result[i][j] = 0;
        }
    }

    for (int i = 0; i < matrix_size; i++)
    {
        pid_t c = fork();

        if (c == -1) {
            printf("Error creating child process\n");
            exit(0);
        } else if (c == 0) // fork returns 0 to the child process so it enters "if" block
        {
            // close(p[0]);
            // printf("[son] pid %d from [parent] pid %d\n", getpid(), getppid());
            multiply_row_by_matrix(matrixA[i], matrix_size, i, matrixB, matrixC);
            
            for (int j = 0; j < matrix_size; j++)
            {
                result[i][j] += matrixC[i][j];
            }
            exit(0);
        } else {
            wait(&c);
        }
    }
    for(int i = 0; i < matrix_size; i++){
        for(int j = 0; j < matrix_size; j++){
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }
    //return result;
}

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

    
    create_processes(A, B, C, matrix_size);
    

    // while(wait(NULL) > 0) {
    //     printf("TODAVIA HAY SUBPROCESOS");
    // }

    // print the results
    printf("Matrix A:\n");
    print_matrix(A, matrix_size);
    printf("Matrix B:\n");
    print_matrix(B, matrix_size);
}