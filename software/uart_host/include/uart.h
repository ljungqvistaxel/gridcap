#ifndef UART_H
#define UART_H

typedef struct 
{
    int* running;
    double* cap_buf[64];
    void (*on_new_value) (void);
    void (*on_error) (void);
} uart_thread_arguments;

void* uart_tstart(void* arg);
double cap_from_time(double t);

#endif