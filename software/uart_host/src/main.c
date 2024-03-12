#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <signal.h>

#include <stdlib.h>
#include <math.h>
//#include <pthread.h>

#include "uart.h"

#define MICRO 0.000001
#define PICO 0.000000000001

#define serial_resistance 20 * 1000.0 // resistance in series when charging capacitor (ohm)
#define flip_voltage 2.8 // voltage where schmitt trigger flips (volt)
#define charging_voltage 3.3 // voltage supplied for charging capacitor (volt)
#define flip_delay 0.0 // constant delay for the schmitt trigger to flip (seconds)

int cap_buf[64];
int tare_buf[64];

int running = 0; // set to 0 to stop program safely

void print_matrix(int* matrix, int overwrite, int tare_adjust);
double cap_from_time(double);

void show_live();

void signal_handler(int s)
{
    running = 0;
}

int main(int argc, char** argv)
{
    signal(SIGINT, signal_handler);

    printf("Good morning sir.\n");

    //printf("2.234 us = %f pF\n", (cap_from_time(2.234 * MICRO) / PICO));

    memset(cap_buf, 0, sizeof(cap_buf)); // empty capacitances
    memset(tare_buf, 0, sizeof(tare_buf)); // empty tare buffer

    char scan_buf[256];

    while(1)
    {
        printf("\nMenu:\n live\n tare\n reset (tare and buffers)\n exit\n\n");
        printf("gridcap > ");
        scanf("%s", scan_buf);
        if(strcmp(scan_buf, "exit") == 0)
        {
            return 0;
        }
        else if(strcmp(scan_buf, "live") == 0)
        {
            running = 1;
            show_live();
        }
        else if(strcmp(scan_buf, "tare") == 0)
        {
            memcpy(tare_buf, cap_buf, sizeof(tare_buf));
            print_matrix(tare_buf, 0, 0);
        }
        else if(strcmp(scan_buf, "reset") == 0)
        {
            memset(tare_buf, 0, sizeof(tare_buf)); // empty tare buffer
            memset(cap_buf, 0, sizeof(cap_buf)); // empty capacitances
            print_matrix(tare_buf, 0, 0);
            printf("\n");
            print_matrix(cap_buf, 0, 0);
        }

    }

    printf("Good night.\n");
    return 0;
}

void print_matrix(int* matrix, int overwrite, int tare_adjust)
{
    if(overwrite == 1)
    {
        printf("\033[8A");
    }
    
    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            if(tare_adjust == 1)
            {
                printf("%4d ", matrix[y*8+x]-tare_buf[y*8+x]);
            }
            else
            {
                printf("%4d ", matrix[y*8+x]);
            }
        }
        printf("\n");
    }
}

double cap_from_time(double t)
{
    static double precalc_const = 0;
    if(precalc_const == 0)
    {
        precalc_const = serial_resistance*log(1-(flip_voltage/charging_voltage));
        precalc_const = 1/precalc_const;
    }

    return -(t*precalc_const);
}

void show_live()
{
    static char uart_dev[] = "/dev/tty.usbmodem1103";
    int serial_port = open(uart_dev, O_RDWR|O_NOCTTY|O_NONBLOCK);
    if(serial_port < 0)
    {
        printf("Error opening serial port: %s\n", strerror(errno));
        return;
    }
    else
    {
        printf("Successfully opened serial port.\n");
    }

    if(uart_init(&serial_port) != 0)
    {
        printf("Error initiating serial port. Abort mission.\n");
        close(serial_port);
        return;
    }
    
    printf("\n");
    printf("\n\n\n\n\n\n\n\n\n");

    char read_buf[256];
    while(running)
    {
        memset(read_buf, '\0', sizeof(read_buf));

        int read_result = read(serial_port, read_buf, sizeof(read_buf));

        if(read_result > 0)
        {
            //printf("%s", read_buf);
            char* pad_end = strchr(read_buf, ',');

            // if separator (,) not found
            if(pad_end - read_buf >= sizeof(read_buf)-1)
            {
                continue;
            }

            int pad = atoi(read_buf);
            int time = atoi(pad_end+1);

            // check invalid pad
            if(pad < 0 || pad >= 64)
            {
                //printf("Error. Pad number \"%d\" invalid.\n");
                continue;
            }

            int cap = round(cap_from_time(time) / PICO);

            //printf("pad: %d, cap: %d\n", pad, cap);
            cap_buf[pad] = cap;
            print_matrix(cap_buf, 1, 1);
        }
        else
        {
            //printf("no read\n");
        }
    }

    printf("Closing serial port.\n");
    close(serial_port);
}
