#include "../include/matrix1.h"
#include <stdlib.h>

void erosion_filter(matrix* origin, matrix* res, ull i_0, ull j_0, ull step)
{
    for (int i = i_0; i < origin->rows; i++)
    {
        // fix j_0 for each row
        for (int j = j_0; j < origin->columns; j += step)
        {
            ull points[4][2] = {
                {i, min(j + 1, origin->columns - 1)},
                {max(i - 1, 0), j},
                {i, max(j - 1, 0)},
                {min(i + 1, origin->rows - 1), j}
            };

            double min_value, value;
            get_value(origin, i, j, &min_value);
            for (int _ = 0; _ < 4; _++)
            {
                get_value(origin, points[_][0], points[_][1], &value);
                min_value = min(min_value, value);
            }
            set_value(res, i, j, min_value);
        }
    }
}