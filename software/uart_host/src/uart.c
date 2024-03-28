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

int connected = 0;

/* Initiate UART serial port. Returns 0 is success. */
void* uart_tstart(void* arg)
{
    uart_thread_arguments* utarg = (uart_thread_arguments*) arg;
    int serial_port;

    uart_init(&serial_port);

    while(*(utarg->running) != 0)
    {

        // ensure connection
        if(connected == 0)
        {
            //printf("trying to reconnect.\n");
            int res = uart_init(&serial_port);
            if(res != 0)
            {
                sleep(1); // 1s delay between retries
            }
            else
            {
                //printf("connected.\n");
            }
        }

        // fetch data
        if(connected == 1)
        {
            static char read_buf[256];
            memset(read_buf, '\0', sizeof(read_buf));

            int read_result = read(serial_port, read_buf, sizeof(read_buf));
            
            if(read_result > 0)
            {
                if(DEBUG != 0)printf("%s", read_buf);
                char* pad_end = strchr(read_buf, ',');
                if(pad_end == NULL)
                {
                    //error
                    continue;
                }
                //printf("found '%c'\n", *pad_end);

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

                //printf("pad: %d, cap: %f\n", pad, cap);
                (*(utarg->cap_buf))[pad] = cap;
                //printf("buffer ok\n");
                //print_matrix(cap_buf, 1, 1);
                (utarg->on_new_value)();
            }
            else if(read_result < 0) // error
            {
                if(errno == 35) // "resource temporarily unavailable"
                {
                    // just try again
                    continue;
                }

                //printf("error reading serial port: (%d) %s\n", errno, strerror(errno));
                //printf("closing.\n");
                close(serial_port);
                connected = 0;
                //break;
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
        //printf("error opening serial port: %s\n", strerror(errno));
        return 1;
    }
    else
    {
        //printf("successfully opened serial port.\n");
    }

    /*if(uart_init(serial_port) != 0)
    {
        printf("Error initiating serial port. Abort mission.\n");
        close(*serial_port);
        return 2;
    }*/

    // fetch tty settings
    struct termios tty;
    if(tcgetattr(*serial_port, &tty) != 0)
    {
        //printf("error fetching tty settings: %s\n", strerror(errno));
        close(*serial_port);
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
    tty.c_cc[VTIME] = 10; // timeout (ds)
    tty.c_cc[VMIN] = 0; // Any message length

    // set tty settings
    if(tcsetattr(*serial_port, TCSANOW, &tty) != 0)
    {
        //printf("error setting tty settings: %s\n", strerror(errno));
        close(*serial_port);
        return 4;
    }

    //printf("uart communication established.\n");
    connected = 1;
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
