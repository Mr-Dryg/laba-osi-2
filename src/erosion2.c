#include "../include/matrix2.h"
#include <stdlib.h>

void erosion_filter(matrix* origin, matrix* res, ull index_0, ull step)
{
    for (int i = index_0; i < origin->size; i += step)
    {
            ull points[4] = {
            min(i + 1, origin->size - 1),
            max(i - origin->columns, 0),
            max(i - 1, 0),
            min(i + origin->columns, origin->size - 1)
        };

        double min_value, value;
        get_value(origin, i, &min_value);
        for (int _ = 0; _ < 4; _++)
        {
            get_value(origin, points[_], &value);
            min_value = min(min_value, value);
        }
        set_value(res, i, min_value);
    }
}