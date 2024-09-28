#include "matrix.h"

#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

// SPI handles
static int OUT, IN;

// init pigpio and SPI
void initMatrix() {
    gpioInitialise();

    // assumptions:
    // - the 595 is CE0
    // - the 165 is CE1
    // 8 = 0b1000 means that CE0 is active low and CE1 is active high
    // since the latch is low when shifting out to the 595 and high when shifting in from the 165
    OUT = spiOpen(0, 1000000, 8);
    IN  = spiOpen(1, 1000000, 8);
}

// cleanup pigpio and SPI
void cleanupMatrix() {
    spiClose(OUT);
    spiClose(IN);
    gpioTerminate();
}

// poll all the keys and write to the output buffer
void poll(Matrix *mat) {
    int c = 0;
    for(int i = 0; i < mat->out; i++) {
        char data = 1 << i;
        spiWrite(OUT, &data, 1);

        spiRead(IN, &data, 1);
        for(int j = 0; j < mat->in && c < mat->keys; j++) {
            // transpose the result (inputs vary slower than outputs)
            mat->buf[j*mat->out+i] = data % 2;
            data /= 2;
            c++;
        }
    }
}

void printmat(Matrix *mat) {
    for(int i = 0; i < mat->keys; i++) {
        printf("%d", mat->buf[i]);
    }
    putchar('\n');
}
