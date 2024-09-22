#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

// MISO
#define DATA 9
#define CLOCK 11
#define LATCH 8

typedef unsigned long ulong;

// return the current unix time in nanoseconds
unsigned long gettime() {
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return time.tv_sec*1000000000 + time.tv_nsec;
}

// manually read n bits from shift register
void bang(int n, int *buf) {
    
}

int main() {
    gpioInitialise();

    gpioTerminate();
}
