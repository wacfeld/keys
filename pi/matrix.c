#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

// assumptions:
// - the 595 is CE0
// - the 165 is CE1

// 24 keys requires a 5x5 keyboard matrix
#define N_OUT 5
#define N_IN 5
#define N_KEYS 24

// SPI handles
int OUT, IN;

// poll all the keys and write to the output buffer
void poll(char *buf) {
    int c = 0;
    for(int i = 0; i < N_OUT; i++) {
        char data = 1 << i;
        spiWrite(OUT, &data, 1);

        spiRead(IN, &data, 1);
        for(int j = 0; j < N_IN && c < N_KEYS; j++) {
            // transpose the result (inputs vary slower than outputs)
            buf[j*N_OUT+i] = data % 2;
            data /= 2;
            c++;
        }
    }
}

void printbuf(char *buf) {
    for(int i = 0; i < N_KEYS; i++) {
        printf("%d", buf[i]);
    }
    putchar('\n');
}

int main() {
    // init
    gpioInitialise();
    // 8 = 0b1000 means that CE0 is active low and CE1 is active high
    // since the latch is low when shifting out to the 595 and high when shifting in from the 165
    OUT = spiOpen(0, 1000000, 8);
    IN  = spiOpen(1, 1000000, 8);

    char buf[N_KEYS];
    while(1) {
        poll(buf);
        printbuf(buf);
        usleep(1000);
    }

    // cleanup
    spiClose(OUT);
    spiClose(IN);
    gpioTerminate();
}
