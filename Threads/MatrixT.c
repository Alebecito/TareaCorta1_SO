#include <string.h>
#include <stdio.h>
#include <unistd.h> // for sleep()
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include "../matrices.h"

typedef struct arguments
{
    int *row;
    int row_size;
    int row_idx;
    int **matrixB;
    int **matrixC;
} Thread_Arguments;

// print matrix functions

// void *multiply_row_by_matrix_threading(int *row, int row_size, int row_idx, int **matrixB, int **matrixC)
void *multiply_row_by_matrix_threading(void *arguments)
{
    // printf("HELLO FROM THREAD\n");
    Thread_Arguments *args = arguments;
    int *row = args->row,
        row_size = args->row_size,
        row_idx = args->row_idx,
        **matrixB = args->matrixB,
        **matrixC = args->matrixC;

    int columns, column_size = row_size;
    int *result = (int *)malloc(sizeof(int) * row_size);
    for (int i = 0; i < row_size; i++)
    {
        result[i] = 0;
    }

    for (int i = 0; i < row_size; i++)
    {
        int *column = get_nth_column(matrixB, column_size, i);
        for (int j = 0; j < column_size; j++)
        {
            result[i] += row[j] * column[j];
        }
    }
    // print_array(result, row_size);
    matrixC[row_idx] = result;
}

void multiply_row_by_matrix_normal(int *row, int row_size, int row_idx, int **matrixB, int **matrixC)
{
    // printf("HELLO FROM NORMAL\n");
    // loop through each column of matrixB
    int columns = row_size, column_size = row_size;
    int *result = (int *)malloc(sizeof(int) * row_size);
    for (int i = 0; i < row_size; i++)
    {
        result[i] = 0;
    }

    for (int i = 0; i < row_size; i++)
    {
        int *column = get_nth_column(matrixB, column_size, i);
        // printf("\t\tCOLUMN #%d -------> ", i);
        // print_array(column, column_size);
        for (int j = 0; j < column_size; j++)
        {
            result[i] += row[j] * column[j];
        }
    }
    // print_array(result, row_size);
    matrixC[row_idx] = result;
}

// create_threads(&threads, matrix_size, A, B, C, matrix_size);
void create_threads(pthread_t *threads, int threads_number, int **matrixA, int **matrixB, int **matrixC, int matrix_size)
{
    for (int i = 0; i < threads_number; i++)
    {
        Thread_Arguments *arguments = (Thread_Arguments *)malloc(sizeof(Thread_Arguments));
        arguments->row = matrixA[i];
        arguments->row_size = matrix_size;
        arguments->row_idx = i;
        arguments->matrixB = matrixB;
        arguments->matrixC = matrixC;
        pthread_create(&threads[i], NULL, multiply_row_by_matrix_threading, arguments);
    }
}

struct timespec start, end;

int main()
{
    // receive a matrix_size and check if it is positive
    int matrix_size;
    printf("Enter the size N for the matrices: ");
    scanf("%d", &matrix_size);
    if (matrix_size < 0)
    {
        printf("N must be positive\n");
        return 0;
    }

    // allocate memory for the matrices
    int **A = generate_random_matrix(matrix_size),
        **B = generate_random_matrix(matrix_size),
        **C = generate_random_matrix(matrix_size);

    clock_gettime(CLOCK_MONOTONIC, &start);

    // create a list of N threads for each

    // ==================== THREADS =================
    // pthread_t threads[matrix_size];

    // create_threads(threads, matrix_size, A, B, C, matrix_size);

    // for (size_t i = 0; i < matrix_size; i++)
    // {
    //     pthread_join(threads[i], NULL);
    // }

    // ================= REGULAR ================
    for (int i = 0; i < matrix_size; i++)
    {
        multiply_row_by_matrix_normal(A[i], matrix_size, i, B, C);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    // print the results
    // printf("Matrix A:\n");
    // print_matrix(A, matrix_size);
    // printf("Matrix B:\n");
    // print_matrix(B, matrix_size);
    // printf("\nThe result of A * B is:\n");
    // print_matrix(C, matrix_size);

    double diff = (end.tv_sec - start.tv_sec);
    diff += (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("Time: %f seconds\n", diff);
}