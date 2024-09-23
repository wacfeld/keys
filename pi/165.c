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

void time_spi(int baud, int n) {
    int h = spiOpen(0, baud, 4); // binary 100 sets CE0 to active high

    ulong total = 0;

    ulong a = gettime();
    for(int i = 0; i < n; i++) {
        char buf[2];
        spiRead(h, buf, 2);
        total += buf[0] + buf[1];
        //printbytes((unsigned char*)buf, 2);
    }
    ulong b = gettime();

    ulong ms = (b-a)/1000000;
    printf("SPI at %d baud took %ld ms to make %d reads, i.e. %ld reads a second\n", baud, ms, n, 1000*n/ms);
    printf("the total is %ld\n", total);

    spiClose(h);
}

int main() {
    gpioInitialise();

    //int buf[8];
    //bang(8, buf);
    //printarr(buf, 8);
    time_spi(5000000, 10000);

    gpioTerminate();
}
