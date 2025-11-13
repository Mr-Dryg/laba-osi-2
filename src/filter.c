#include "../include/filter.h"
#include <stdio.h>
#include <stdlib.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

void erosion_filter(matrix* erosion_origin, matrix* erosion_res, ull index_0, ull step)
{
    for (ull i = index_0; i < erosion_origin->size; i += step)
    {
        ull points[4] = {
            min(i + 1, erosion_origin->size - 1),
            max(i - erosion_origin->columns, 0),
            max(i - 1, 0),
            min(i + erosion_origin->columns, erosion_origin->size - 1)
        };

        double min_value, value;
        get_value(erosion_origin, i, &min_value);

        for (int j = 0; j < 4; j++)
        {
            get_value(erosion_origin, points[j], &value);
            min_value = min(min_value, value);
        }
        set_value(erosion_res, i, min_value);
    }
}

void dilation_filter(matrix* dilation_origin, matrix* dilation_res, ull index_0, ull step)
{
    for (ull i = index_0; i < dilation_res->size; i += step)
    {
        ull points[4] = {
            min(i + 1, dilation_res->size - 1),
            max(i - dilation_res->columns, 0),
            max(i - 1, 0),
            min(i + dilation_res->columns, dilation_res->size - 1)
        };

        double min_value, max_value, value;
        get_value(dilation_origin, i, &max_value);

        for (int j = 0; j < 4; j++)
        {
            get_value(dilation_origin, points[j], &value);
            max_value = max(max_value, value);
        }
        set_value(dilation_res, i, max_value);
    }
}