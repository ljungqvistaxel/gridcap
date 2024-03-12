#ifndef CONFIG_H
#define CONFIG_H

#define PICO 0.000000000001
#define MICRO 0.000001

// config
#define serial_resistance 20000 //10.0*1000*1000 /*ohms*/
#define min_capacitance 30 * PICO/*Farads*/ // min capacitance
#define max_capacitance 150 * PICO/*Farads*/ // max capacitance to be fully measured up to 4RC (~98%)
//#define n_samples 10 // number of samples to calculate
#define n_charge_levels 256 // number of voltage levels (resolution) incl 0v

// more config
#define source_voltage 3.3 // voltage of source
#define time_constants 3 // time constants to calculate to calculate (on max_capacitance)
#define sample_delay 1 * MICRO //serial_resistance * max_capacitance * time_constants/*RC*/ / n_samples // delay between each sample (seconds)
#define sample_start_time sample_delay // when the first sample will be (seconds)
#define max_charge_percentage 100.0 // max adc voltage to calculate (lower value cuts off highest charges)
#define output_file_name "cap_matrix.h"

#define n_samples 10

// do not touch
#define charge_per_level max_charge_percentage/(n_charge_levels) // real volts per "voltage level"

#endif
