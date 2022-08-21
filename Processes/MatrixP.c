#include <string.h>
#include <stdio.h>
#include <unistd.h> // for sleep()
#include <stdlib.h>
#include <math.h>
#include "../matrices.h"
#include "stdlib.h"
#include <sys/wait.h>

void create_processes(int **matrixA, int **matrixB, int **matrixC, int matrix_size)
{
    int p[matrix_size][matrix_size];
    int buff[1024];

    for (int i = 0; i < matrix_size; i++)
    {
        pipe(p[i]);
        pid_t c = fork();
        if (c == 0) // fork returns 0 to the child process so it enters "if" block
        {
            // close(p[0]);
            // printf("[son] pid %d from [parent] pid %d\n", getpid(), getppid());

            multiply_row_by_matrix(matrixA[i], matrix_size, i, matrixB, matrixC);

            printf("Resultado: %d\n", matrixC[i][i]);
            // child exits
            write(p[i][1], &matrixC[i], sizeof(matrixC[i]));
            // close(p[i]);
            exit(0);
        } else {
            wait(&c);
            read(p[i][0], buff, sizeof(buff));
            printf("Hola2 %ls\n", buff);
        }
    }
}

int main()
{
    // receive a number and check if it is positive
    int matrix_size;
    printf("Enter the size N for the matrices: ");
    scanf("%d", &matrix_size);


    // allocate memory for the matrices
    int **A = generate_random_matrix(matrix_size);
    int **B = generate_random_matrix(matrix_size);

    // create a matrix C to store the result of A * B
    int **C = (int **)malloc(matrix_size * sizeof(int *));

    create_processes(A, B, C, matrix_size);
    

    // while(wait(NULL) > 0) {
    //     printf("TODAVIA HAY SUBPROCESOS");
    // }


    // print the results
    printf("Matrix A:\n");
    print_matrix(A, matrix_size);
    printf("Matrix B:\n");
    print_matrix(B, matrix_size);
    // printf("\nThe result of A * B is:\n");
    // for (int i = 0; i < matrix_size; i++)
    // {
    //     printf("%p", &C[i]);
    // }
}