#include "../include/matrix.h"
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

matrix* create_matrix(ull rows, ull columns)
{
    matrix* mat = malloc(sizeof(matrix));
    if (mat == NULL)
    {
        perror("Memory allocation error");
        exit(1);
    }
    mat->rows = rows;
    mat->columns = columns;
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
    *value = mat->buf[index];
    return 1;
}

void destroy_matrix(matrix* mat)
{
    free(mat->buf);
    free(mat);
}

matrix* load_matrix_from_file(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        perror("File opening for loading error");
        exit(1);
    }
    
    ull rows;
    if (fscanf(file, "%llu", &rows) != 1)
    {
        perror("Number of rows reading error");
        fclose(file);
        exit(1);
    }

    ull columns;
    if (fscanf(file, "%llu", &columns) != 1)
    {
        perror("Number of rows reading error");
        fclose(file);
        exit(1);
    }

    matrix* origin = create_matrix(rows, columns);

    double value;
    ull index = 0;
    while (fscanf(file, "%lf", &value) == 1)
    {
        if (!set_value(origin, index++, value))
        {
            perror("Too many elements in matrix");
            destroy_matrix(origin);
            fclose(file);
            exit(1);
        }
    }

    if (!feof(file))
    {
        perror("Reading error");
        destroy_matrix(origin);
        fclose(file);
        exit(1);
    }
    
    fclose(file);

    return origin;
}

void save_matrix_in_file(matrix* mat, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (!file)
    {
        perror("File opening for saving error");
        exit(1);
    }

    fprintf(file, "%llu %llu\n", mat->rows, mat->columns);
    
    double value;
    for (int i = 0; i < mat->size; i++)
    {
        get_value(mat, i, &value);
        fprintf(file, "%lf", value);
        if ((i + 1) % mat->columns == 0 && i != mat->size - 1)
            fprintf(file, "\n");
        else if (i != mat->size - 1)
            fprintf(file, " ");
    }

    fclose(file);
}

matrix* copy_matrix(matrix* mat)
{
    matrix* res = create_matrix(mat->rows, mat->columns);
    for (int i = 0; i < mat->size; i++)
        res->buf[i] = mat->buf[i];
    return res;
}