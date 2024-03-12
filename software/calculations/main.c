#include <stdio.h>
#include <math.h>

#include "config.h"
#include "fileout.h"

//#define DEBUG
#ifdef DEBUG
#include "cap_matrix.h"
#endif

int main(int argc, char** argv)
{
    printf("begin.\n");

    #ifdef DEBUG
    printf("sizeof(cap_matrix) = %d\n", (int)sizeof(cap_matrix));
    printf("cap[0, 10] = %d\n", cap_matrix[0][10]);
    #endif

    printf("sample delay = %f ms\n", sample_delay*1000);
    printf("memory usage = %d (Byte)\n\n", n_samples*n_charge_levels*((int)sizeof(int)));

    int matrix[n_samples][n_charge_levels];

    for(int i_sample = 0; i_sample < n_samples; i_sample++) // for every sample
    {
        for(int i_charge = 0; i_charge < n_charge_levels; i_charge++) // for every charge level
        {
            long double real_time = sample_start_time + sample_delay * i_sample; // time of reading in seconds (seconds)
            long double real_charge = charge_per_level * i_charge; // charge at time of reading in percent (0.0-100.0)
            long double real_voltage = source_voltage * real_charge/100.0; // voltage in capacitor (volts)

            long double real_capacitance;
            if(real_charge == 0) // ignore case: unlimited capacitance or resistance
            {
                real_capacitance = 0;
            }
            else
            {
                // calculate capacitance with formula found somewhere on www
                real_capacitance = - (real_time / 
                    (serial_resistance * log((source_voltage - real_voltage)
                                                    /source_voltage)));
            }

            int cap_pf = (int)round(real_capacitance / PICO); // round capacitance to to pF
            if(real_capacitance > max_capacitance*2) cap_pf = 0; // filter unneccesary values

            matrix[i_sample][i_charge] = cap_pf;
        }
    }

    // print matrix
    /*
    for(int y = 0; y < n_charge_levels; y++)
    {
        for(int x = 0; x < n_samples; x++)
        {
            int cap = matrix[x][n_charge_levels-1-y];

            if(cap < min_capacitance/PICO)
            {
                printf("   .");
            }
            else
            {
                printf("%3d ", cap);
            }
            
        }
        printf("\n");
    }
    */

    printf("\nwriting file \"%s\".\n", output_file_name);

    output_file(matrix);

    printf("\ndone.\n");

    return 0;
}
