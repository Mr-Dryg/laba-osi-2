#include "../include/matrix1.h"
#include <stdlib.h>

matrix* create_matrix(ull columns, ull rows)
{
    matrix* mat = malloc(sizeof(matrix));
    if (mat == NULL)
    {
        perror("Memory allocation error");
        exit(1);
    }
    mat->buf = malloc(sizeof(double) * columns * rows);
    if (mat->buf == NULL)
    {
        perror("Memory allocation error");
        free(mat);
        exit(1);
    }
    mat->columns = columns;
    mat->rows = rows;
    return mat;
}

int get_index(matrix* mat, ull i, ull j, ull* index)
{
    if ((0 <= i && i < mat->rows) && (0 <= j && j < mat->columns))
    {
        *index = i * mat->columns + j;
        return 1;
    }
    return 0;
}

int set_value(matrix* mat, ull i, ull j, double value)
{
    ull index;
    if (!get_index(mat, i, j, &index))
        return 0;
    mat->buf[index] = value;
    return 1;
}

int get_value(matrix* mat, ull i, ull j, double* value)
{
    ull index;
    if (!get_index(mat, i, j, &index))
        return 0;
    mat->buf[index] = *value;
    return 1;
}

void delete_matrix(matrix* mat)
{
    free(mat->buf);
    free(mat);
}