#include "uart.h"

#include <stdio.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

#include "config.h"
#include "mux_map.h"

int uart_init(int* serial_port);

/* Initiate UART serial port. Returns 0 is success. */
void* uart_tstart(void* arg)
{
    uart_thread_arguments* utarg = (uart_thread_arguments*) arg;

    int serial_port;
    while(*(utarg->running) != 0)
    {
        // init connection
        while(uart_init(&serial_port) != 0 && *(utarg->running) != 0)
        {   
            // could not connect
            sleep(1); // 1s delay between retries
            //printf("trying to reconnect.\n");
        }

        char read_buf[256];
        while(*(utarg->running) != 0)
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

                pad = mux_map[pad]; // get correct pad index

                double cap = cap_from_time((time+flip_delay) * NANO)/PICO;

                //printf("pad: %d, cap: %d\n", pad, cap);
                (*(utarg->cap_buf))[pad] = cap;
                //print_matrix(cap_buf, 1, 1);
                utarg->on_new_value();
            }
            else if(read_result < 0) // error
            {
                printf("connection lost\n");
                break;
            }
        }
    }

    printf("closing serial port.\n");
    close(serial_port);
    return 0;
}

int uart_init(int* serial_port)
{
    static char uart_dev[] = "/dev/tty.usbmodem1103";

    *serial_port = open(uart_dev, O_RDWR|O_NOCTTY|O_NONBLOCK);
    if(*serial_port < 0)
    {
        //printf("Error opening serial port: %s\n", strerror(errno));
        return 1;
    }
    else
    {
        //printf("Successfully opened serial port.\n");
    }

    if(uart_init(serial_port) != 0)
    {
        //printf("Error initiating serial port. Abort mission.\n");
        close(*serial_port);
        return 2;
    }

    // fetch tty settings
    struct termios tty;
    if(tcgetattr(*serial_port, &tty) != 0)
    {
        //printf("Error fetching tty settings: %s\n", strerror(errno));
        return 3;
    }

    cfsetispeed(&tty, B115200); // baud in
    cfsetospeed(&tty, B115200); // baud out

    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable backspace erasure
    tty.c_cflag |= CREAD | CLOCAL; // ?
    //tty.c_lflag &= ~ICANON; // do not wait for \n
    tty.c_lflag |= ICANON; // wait for \n
    tty.c_lflag &= ~ECHONL; // Disable "echo new line"
    tty.c_cc[VTIME] = 1; // timeout
    tty.c_cc[VMIN] = 0; // Any message length

    // set tty settings
    if(tcsetattr(*serial_port, TCSANOW, &tty) != 0)
    {
        //printf("Error setting tty settings: %s\n", strerror(errno));
        return 4;
    }

    printf("uart communication established.\n");
    return 0;
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
