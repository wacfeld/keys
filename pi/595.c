#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

typedef unsigned long ulong;

// return the current unix time in nanoseconds
unsigned long gettime() {
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return time.tv_sec*1000000000 + time.tv_nsec;
}

int main() {
    //gpioInitialise();
    //gpioTerminate();
}
