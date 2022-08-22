#ifndef _MATRICES_H_
#define _MATRICES_H_

#include <stdlib.h>

void print_matrix(int **matrix, int size);

void print_array(int *array, int size);

int *get_nth_column(int **matrix, int size, int nth_column);

void *multiply_row_by_matrix(int *row, int row_size, int row_idx, int **matrixB, int **matrixC);

int **generate_random_matrix(int size);

void write_matrix(char *filename, int **matrix, int size);

void write_matrix_memseg(char *filename, int size, int (*matrix)[size]);

#endif