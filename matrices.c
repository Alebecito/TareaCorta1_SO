#include "matrices.h"
#include <stdio.h>

void print_matrix(int **matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void write_matrix(char *filename, int **matrix, int size)
{
    FILE *file = fopen(filename, "w");
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            fprintf(file, "%d ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void write_stats(double time, char *message)
{
    //  stats_file = fopen("Stats.txt", "a");
    fprintf(stats_file, "%s %f\n", message, time);
    // printf("CHOTA\n");

    // printf("WRITE: %s %f\n", message, time);
    // fclose(stats_file);
}

void write_matrix_memseg(char *filename, int size, int (*matrix)[size])
{
    FILE *file = fopen(filename, "w");
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            fprintf(file, "%d ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void print_array(int *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int *get_nth_column(int **matrix, int size, int nth_column)
{
    int *result = (int *)malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++)
    {
        result[i] = matrix[i][nth_column];
    }
    return result;
}

void *multiply_row_by_matrix(int *row, int row_size, int row_idx, int **matrixB, int **matrixC)
{
    // printf("ROW #%d\n", row_idx);
    // printf("\tROW -------> ");
    // print_array(row, row_size);

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
        free(column);
    }
    // print_array(result, row_size);
    matrixC[row_idx] = result;
}

int **generate_random_matrix(int size)
{
    int **matrix = (int **)malloc(size * sizeof(int *));

    // initialize the matrices with random values
    for (int i = 0; i < size; i++)
    {
        matrix[i] = (int *)malloc(size * sizeof(int));
        for (int j = 0; j < size; j++)
        {
            matrix[i][j] = rand() % 1000;
        }
    }
    return matrix;
}