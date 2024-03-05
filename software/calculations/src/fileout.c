#include "fileout.h"
#include "config.h"
#include <stdio.h>
#include <malloc.h>

#define buf_size 5000000

char* fileformat = 
"#ifndef CAP_MATRIX_H\n"
"#define CAP_MATRIX_H\n"
"\n"
"#include <stdint.h>\n"
"\n"
"// Generated file with pre-calculated capacitances. Sample delay: %f us\n"
"// Usage: cap_matrix[SAMPLE NUMBER (0-%d)][VOLTAGE (0-%d)] -> returns capacitance value in pF (uint8_t).\n"
"\n"
"static const uint8_t cap_matrix[%d][%d] = {%s};\n"
"\n"
"#endif\n";

int res = 0;

void output_file(int matrix[n_samples][n_charge_levels])
{
    char* matrix_buf = malloc(sizeof(char) * buf_size);
    int matrix_buf_size = 0;

    for(int t = 0; t < n_samples; t++) // time
    {
        if(t != 0)
        {
            matrix_buf[matrix_buf_size++] = ','; // add ,
        }
        matrix_buf[matrix_buf_size++] = '\n';
        matrix_buf[matrix_buf_size++] = '{';
        for(int v = 0; v < n_charge_levels; v++) // adc value
        {
            if(v != 0)
            {
                res = sprintf_s(&matrix_buf[matrix_buf_size], buf_size-matrix_buf_size, ", "); // add ,
                if(res < 0) {printf("error writing v to matrix_buf. t=%d, v=%d\n", t, v);}
                matrix_buf_size += res;
            }

            res = sprintf_s(&matrix_buf[matrix_buf_size], buf_size-matrix_buf_size, "%3d", matrix[t][v]); // add cap value
            if(res < 0) {printf("error writing v to matrix_buf. t=%d, v=%d\n", t, v);}
            matrix_buf_size += res;
        }
        matrix_buf[matrix_buf_size++] = '}';
    }
    matrix_buf[matrix_buf_size++] = '\0'; // end of string

    char* file_buf = malloc(sizeof(char) * buf_size);
    int file_len = sprintf_s(file_buf, buf_size, fileformat, sample_delay/MICRO, n_samples-1, n_charge_levels-1, n_samples, n_charge_levels, matrix_buf);
    printf("file length is %d\n", file_len);

    FILE* hfile = fopen(output_file_name, "w");
    fwrite(file_buf, sizeof(char), file_len, hfile);

    fclose(hfile);
    free(matrix_buf);
    free(file_buf);
}
