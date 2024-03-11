#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <signal.h>

#include <stdlib.h>

#include "uart.h"

int cap_buf[64];
int tare_buf[64];

int running = 1; // set to 0 to stop program safely

void print_cap();

void signal_handler(int s)
{
    running = 0;
}

int main(int argc, char** argv)
{
    signal(SIGINT, signal_handler);

    printf("Good morning sir.\n");

    //memset(cap_buf, 0, sizeof(cap_buf)); // empty capacitances

    static char uart_dev[] = "/dev/tty.usbmodem1103";

    int serial_port = open(uart_dev, O_RDWR|O_NOCTTY|O_NONBLOCK);
    if(serial_port < 0)
    {
        printf("Error opening serial port: %s\n", strerror(errno));
        return 1;
    }
    else
    {
        printf("Successfully opened serial port.\n");
    }

    if(uart_init(&serial_port) != 0)
    {
        printf("Error initiating serial port. Abort mission.\n");
        close(serial_port);
        return 1;
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
            int pad = atoi(read_buf);
            int cap = atoi(pad_end+1);

            if(pad < 0 || pad >= 64)
            {
                //printf("Error. Pad number \"%d\" invalid.\n");
                return 1;
            }

            //printf("pad: %d, cap: %d\n", pad, cap);
            cap_buf[pad] = cap;
            print_cap();
        }
        else
        {
            //printf("no read\n");
        }
    }

    printf("Closing serial port.\n");
    close(serial_port);
    printf("Good night.\n");
    return 0;
}

void print_cap()
{
    printf("\033[9A");

    printf("Gridcap readings:\n");
    
    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            printf("%4d ", cap_buf[y*8+x]);
        }
        printf("\n");
    }
}
