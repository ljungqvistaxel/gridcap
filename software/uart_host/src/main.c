#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char** argv)
{
    printf("Good morning sir.\n");

    static char uart_dev[] = "/dev/ttyX";

    int serial_port = open(uart_dev, O_RDWR);
    if(serial_port < 0)
    {
        printf("Error opening serial port: %s\n", strerror(errno));
        return 1;
    }
    else
    {
        printf("Successfully opened serial port.\n");
    }


    printf("Good night.\n");
    return 0;
}
