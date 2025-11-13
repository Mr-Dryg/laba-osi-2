#pragma once

typedef unsigned long long ull;

typedef struct
{
    double* buf;
    ull columns;
    ull rows;
    ull size;
} matrix;

matrix* create_matrix(ull columns, ull rows);

int set_value(matrix* mat, ull index, double value);

int get_value(matrix* mat, ull index, double* value);

void delete_matrix(matrix* mat);