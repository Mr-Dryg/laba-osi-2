#pragma once

typedef unsigned long long ull;

typedef struct
{
    double* buf;
    ull columns;
    ull rows;
} matrix;

matrix* create_matrix(ull columns, ull rows);

int set_value(matrix* mat, ull i, ull j, double value);

int get_value(matrix* mat, ull i, ull j, double* value);

void delete_matrix(matrix* mat);