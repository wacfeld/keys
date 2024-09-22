#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

#define N 65536

typedef unsigned long ulong;

// return the current unix time in nanoseconds
unsigned long gettime() {
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return time.tv_sec*1000000000 + time.tv_nsec;
}

void time_spi(int baud) {
    int h = spiOpen(0, baud, 0);

    uint16_t spi_data[N];
    for(int i = 0; i < N; i++) {
        spi_data[i] = i;
    }

    ulong a = gettime();
    for(int i = 0; i < N; i++) {
        spiWrite(h, (char*)&spi_data[i], 2);
    }
    ulong b = gettime();

    ulong ms = (b-a)/1000000;
    printf("SPI took %ld ms to make %d writes, i.e. %ld writes a second\n", ms, N, 1000*N/ms);

    spiClose(h);
}

int main() {
    gpioInitialise();

    time_spi(25000000);

    gpioTerminate();
}
