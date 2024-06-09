#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG 0

#define MICRO 0.000001
#define NANO  0.000000001
#define PICO  0.000000000001

#define serial_resistance 220 * 1000.0 // resistance in series when charging capacitor (ohm)
#define flip_voltage 1.74 //1.6 // voltage where schmitt trigger flips (volt)
#define charging_voltage 3.3 // voltage supplied for charging capacitor (volt)
#define flip_delay 200.0 // constant delay for the schmitt trigger to flip (ns)

#endif
