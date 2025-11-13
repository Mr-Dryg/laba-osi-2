#pragma once

typedef unsigned long long ull;

typedef struct
{
    double* buf;
    ull columns;
    ull rows;
    ull size;
} matrix;

matrix* create_matrix(ull rows, ull columns);

int set_value(matrix* mat, ull index, double value);

int get_value(matrix* mat, ull index, double* value);

void destroy_matrix(matrix* mat);

matrix* load_matrix_from_file(const char* filename);

void save_matrix_in_file(matrix* mat, const char* filename);

matrix* copy_matrix(matrix* mat);