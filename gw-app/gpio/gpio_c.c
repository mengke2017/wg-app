#include "gpio/gpio_c.h"
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <termios.h>  
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

unsigned char data;
int gpio_open(const char* dev)
{
    int fd = -1;

    fd = open(dev,O_RDWR|O_NOCTTY);
    return fd;
}

void gpio_close(int fd)
{
    if(fd != -1) {
        close(fd);
    }
}

void gpio_ioctl(int fd,unsigned int io,unsigned char* value)
{
    data = *value;
    ioctl(fd,io,&data);
    *value = data;
}

