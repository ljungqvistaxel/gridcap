#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <signal.h>

#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#include "uart.h"
#include "config.h"

enum program_state
{
    menu,
    live,
    exiting
};

double cap_buf[64];
double tare_buf[64];

double test_buf[64] =
{
    0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7,
    0.1, 0.2, 0.7, 3.7, 5.5, 3.2, 0.4, 0.3,
    0.3, 0.5, 1.2, 10.7, 24.5, 11.3, 7.0, 5.6,
    0.1, 0.2, 0.7, 3.7, 5.5, 3.2, 0.4, 0.3,
    0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7,
    0.0, 0.2, 0.2, 0.2, 0.1, 0.2, 0.3, 0.2,
    -0.1, -0.2, -0.7, -3.7, -5.5, -3.2, -0.4, -0.3,
    -24.7, -0.1, -38.5, -0.3, -3, -15, -0.6, -0.7
};

int run_threads = 1; // set to 0 to stop program safely
pthread_t uart_thread;

enum program_state program_state = menu;

void print_matrix(double* matrix, int overwrite, int tare_adjust);
void on_new_cap();
void on_uart_err();
void show_live();
void safe_exit();

void signal_handler(int s)
{
    if(program_state == live)
    {
        program_state = menu;
    }
    else if(program_state == menu)
    {
        program_state = exiting;
        safe_exit();
    }
}

int main(int argc, char** argv)
{
    printf("Good morning sir.\n");

    //print_matrix(test_buf, 0, 0);

    signal(SIGINT, signal_handler); // for catching ctrl-c

    memset(cap_buf, 0, sizeof(cap_buf)); // clear capacitances
    memset(tare_buf, 0, sizeof(tare_buf)); // clear tare buffer

    // init threads
    uart_thread_arguments utarg;
    utarg.cap_buf = &cap_buf;
    utarg.running = &run_threads;
    utarg.on_new_value = &on_new_cap;
    utarg.on_error = NULL;

    pthread_create(&uart_thread, NULL, uart_tstart, (void*)&utarg);

    printf("4488 ns = %f pF\n", cap_from_time((4488.0 * NANO))/PICO);

    char scan_buf[256];

    // menu loop
    while(program_state != exiting)
    {
        printf("\nMenu:\n live\n tare\n reset (tare and buffers)\n exit\n\n");
        printf("gridcap > ");
        scanf("%s", scan_buf);
        if(strcmp(scan_buf, "exit") == 0)
        {
            program_state = exiting;
        }
        else if(strcmp(scan_buf, "live") == 0)
        {
            show_live();
        }
        else if(strcmp(scan_buf, "tare") == 0)
        {
            int filled = 1;
            for(int i = 0; i < 64; i++) // check if sensor data has been received.
            {
                if(cap_buf[i] == 0) filled = 0;
            }

            if(filled == 1) // ok, do tare
            {
                memcpy(tare_buf, cap_buf, sizeof(tare_buf));
                print_matrix(tare_buf, 0, 0);
            }
            else // do not tare
            {
                printf("Not enought data to tare.\n");
            }
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

    safe_exit();
    return 0;
}

void safe_exit()
{
    printf("exiting.\n");
    run_threads = 0;
    pthread_join(uart_thread, NULL);
    printf("Good night.\n");
    exit(0);
}

void print_matrix(double* matrix, int overwrite, int tare_adjust)
{
    if(overwrite == 1)
    {
        printf("\033[16A");
    }
    
    double val = 0;
    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {

            if(tare_adjust == 1)
            {
                val = matrix[y*8+x]-tare_buf[y*8+x];
            }
            else
            {
                val = matrix[y*8+x];
            }

            if(val < 0 && val > -25) // negative
            {
                printf("\033[38;5;%dm", (int)(22 + 1*((int)-val/5)));
            }
            else if(val >= 0 && val < 25) // positive
            {
                printf("\033[38;5;%dm", (int)(22 + 36*((int)val/5)));
            }
            else // out of range
            {
                printf("\033[38;5;%dm", 160);
            }

            printf("%6.1f ", val);
            printf("\033[0m");
        }
        printf("\n\n");
    }
}

void show_live()
{
    program_state = live;
    
    printf("\n");
    printf("\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n");

    print_matrix(cap_buf, 1, 1);
    while(program_state == live)
    {

    }
}

void on_new_cap()
{
    if(program_state == live)
    {
        print_matrix(cap_buf, 1, 1);
    }
}

void on_uart_err()
{

}
