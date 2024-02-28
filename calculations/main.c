#include <stdio.h>
#include <math.h>

#define PICO 0.000000000001

// config
#define serial_resistance 10.0*1000*1000 /*ohms*/
#define max_capacitance 100 * PICO/*Farads*/ // max capacitance to be fully measured up to 4RC (~98%)
#define n_samples 35 // number of samples to calculate
#define n_charge_levels 64 // number of voltage levels (resolution) incl 0v

// more config
#define source_voltage 3.3 // voltage of source
#define time_constants 2 // time constants to calculate to calculate (on max_capacitance)
#define sample_delay serial_resistance * max_capacitance * time_constants/*RC*/ / n_samples // delay between each sample (seconds)
#define sample_start_time sample_delay // when the first sample will be (seconds)
#define max_charge_percentage 100.0 // max adc voltage to calculate (lower value cuts off highest charges)

// do not touch
#define charge_per_level max_charge_percentage/(n_charge_levels) // real volts per "voltage level"

int main(int argc, char** argv)
{
    printf("begin.\n");
    printf("sample delay = %f ms\n\n", sample_delay*1000);

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
            matrix[i_sample][i_charge] = cap_pf;
        }
    }

    // print matrix
    for(int y = 0; y < n_charge_levels; y++)
    {
        for(int x = 0; x < n_samples; x++)
        {
            printf("%4d ", matrix[x][n_charge_levels-1-y]);
        }
        printf("\n");
    }

    printf("\ndone.\n");

    return 0;
}
