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
    gpioSetMode(DATA, PI_INPUT);
    //gpioSetPullUpDown(DATA, PI_PUD_DOWN);

    // reset clock and raise latch
    gpioWrite(CLOCK, 0);
    gpioWrite(LATCH, 1);

    for(int i = 0; i < n; i++) {
        gpioWrite(CLOCK, 1);
        buf[i] = gpioRead(DATA);
        gpioWrite(CLOCK, 0);
    }

    gpioWrite(LATCH, 0);
}

// print a binary array
void printarr(int *arr, int n) {
    for(int i = 0; i < n; i++) {
        printf("%d", arr[i]);
    }
    putchar('\n');
}

void printbytes(unsigned char *bytes, int n) {
    for(int i = 0; i < n; i++) {
        int x = bytes[i];
        for(int j = 0; j < 8; j++) {
            printf("%d", x%2);
            x /= 2;
        }
    }
    putchar('\n');
}

void time_spi(int baud) {
    int h = spiOpen(0, baud, 4); // binary 100 sets CE0 to active high

    char buf[1];
    spiRead(h, buf, 1);
    printbytes((unsigned char*)buf, 1);

    spiClose(h);
}

int main() {
    gpioInitialise();

    //int buf[8];
    //bang(8, buf);
    //printarr(buf, 8);
    while(1) {
        time_spi(100000);
        usleep(100000);
    }

    gpioTerminate();
}
