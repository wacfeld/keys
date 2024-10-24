#include "matrix.h"

#include <stdio.h>

// SPI handles
static int OUT, IN;

// init pigpio and SPI
void initMatrix(int baud) {
    gpioInitialise();

    // assumptions:
    // - the 595 is CE0
    // - the 165 is CE1
    // 8 = 0b1000 means that CE0 is active low and CE1 is active high
    // since the latch is low when shifting out to the 595 and high when shifting in from the 165
    OUT = spiOpen(0, baud, 8);
    IN  = spiOpen(1, baud, 8);
}

// cleanup pigpio and SPI
void cleanupMatrix() {
    spiClose(OUT);
    spiClose(IN);
    gpioTerminate();
}

// poll all the keys and write to the output buffer
void poll(Matrix *mat) {
    // TODO generalize to multiple bytes
    if(mat->out > 8 || mat->in > 8) {
        std::cerr << "poll(): matrix side lengths greater than 8 not supported yet\n";
        exit(1);
    }
    // TODO resolve this or simply eliminate the keys field
    if(mat->out * mat->in != mat->keys) {
        std::cerr << "poll(): keys != in * out\n";
    }
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
        printf("%d", (int) mat->buf[i]);
    }
    putchar('\n');
}
