#include "uart.h"

#include <stdio.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

/* Initiate UART serial port. Returns 0 is success. */
int uart_init(int* serial_port)
{
    struct termios tty;

    // fetch tty settings
    if(tcgetattr(*serial_port, &tty) != 0)
    {
        printf("Error fetching tty settings: %s\n", strerror(errno));
        return errno;
    }

    cfsetispeed(&tty, B115200); // baud in
    cfsetospeed(&tty, B115200); // baud out

    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable backspace erasure
    tty.c_cflag |= CREAD | CLOCAL; // ?
    //tty.c_lflag &= ~ICANON; // ?
    tty.c_lflag |= ICANON; // ?
    tty.c_lflag &= ~ECHONL; // Disable "echo new line"
    tty.c_cc[VTIME] = 1; // timeout
    tty.c_cc[VMIN] = 0; // Any message length

    // set tty settings
    if(tcsetattr(*serial_port, TCSANOW, &tty) != 0)
    {
        printf("Error setting tty settings: %s\n", strerror(errno));
        return errno;
    }

    printf("Configured serial port successfully.\n");

    return 0;
}