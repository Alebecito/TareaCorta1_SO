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
        free(column);
    }
    // print_array(result, row_size);
    matrixC[row_idx] = result;
    free(arguments);
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
    // pthread_attr_t a;
    // pthread_attr_init(&a);
    // pthread_attr_setdetachstate(&a, PTHREAD_CREATE_DETACHED);

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
    stats_file = fopen("Stats.txt", "w");

    srand(time(0));

    // receive a matrix_size and check if it is positive
    int matrix_size;
    printf("Enter the size N for the matrices: ");
    scanf("%d", &matrix_size);
    if (matrix_size < 0)
    {
        printf("N must be positive\n");
        return 0;
    }

    double average = 0;

    for (int i = 0; i < 100; i++)
    {
        // allocate memory for the matrices and initialize them with random values
        int **A = generate_random_matrix(matrix_size),
            **B = generate_random_matrix(matrix_size),
            **C = generate_random_matrix(matrix_size);

        clock_gettime(CLOCK_MONOTONIC, &start);

        // create a list of N threads for each

        // ==================== THREADS =================

        pthread_t threads[matrix_size];

        create_threads(threads, matrix_size, A, B, C, matrix_size);

        // ================= REGULAR ================
        // for (int i = 0; i < matrix_size; i++)
        // {
        //     multiply_row_by_matrix_normal(A[i], matrix_size, i, B, C);
        // }

        clock_gettime(CLOCK_MONOTONIC, &end);

        for (int x = 0; x < matrix_size; x++)
        {
           pthread_join(threads[x], NULL);
        }
        // print the results
        // printf("Matrix A:\n");
        // print_matrix(A, matrix_size);
        // printf("Matrix B:\n");
        // print_matrix(B, matrix_size);
        // printf("\nThe result of A * B is:\n");
        // print_matrix(C, matrix_size);

        char filename[12];
        sprintf(filename, "Mat_%s%d.txt", i < 10 ? "0" : "", i);


        write_matrix(filename, C, matrix_size);

        double diff = (end.tv_sec - start.tv_sec);
        diff += (end.tv_nsec - start.tv_nsec) / 1000000000.0;

        // printf("Time: %f seconds\n", diff);

        char iteration[18];
        sprintf(iteration, "Iteration_%s%d: ", i < 10 ? "0" : "", i);

        write_stats(diff, iteration);
        // fprintf(stats_file, "JUPON\n");
        // fputs("JUPON\n", stats_file);

        // printf("diff in %f: %d\n", i, diff);
        average += diff;
        for (int z = 0; z < matrix_size; z++)
        {
            free(A[z]);
            free(B[z]);
            free(C[z]);
        }
        free(A);
        free(B);
        free(C);
    }
    write_stats(average / 100, "Average:");
    fclose(stats_file);
    return 0;
}