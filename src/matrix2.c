#include "../include/matrix2.h"
#include <stdlib.h>

matrix* create_matrix(ull columns, ull rows)
{
    matrix* mat = malloc(sizeof(matrix));
    if (mat == NULL)
    {
        perror("Memory allocation error");
        exit(1);
    }
    mat->columns = columns;
    mat->rows = rows;
    mat->size = columns * rows;
    mat->buf = malloc(sizeof(double) * mat->size);
    if (mat->buf == NULL)
    {
        perror("Memory allocation error");
        free(mat);
        exit(1);
    }
    return mat;
}

int good_index(matrix* mat, ull index)
{
    return 0 <= index && index < mat->size;
}

int set_value(matrix* mat, ull index, double value)
{
    if (!good_index(mat, index))
        return 0;
    mat->buf[index] = value;
    return 1;
}

int get_value(matrix* mat, ull index, double* value)
{
    if (!good_index(mat, index))
        return 0;
    mat->buf[index] = *value;
    return 1;
}

void delete_matrix(matrix* mat)
{
    free(mat->buf);
    free(mat);
}