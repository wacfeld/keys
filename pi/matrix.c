#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

// assumptions:
// - the 595 is CE0
// - the 165 is CE1

// 24 keys requires a 5x5 keyboard matrix
#define NROWS 5
#define NKEYS 24

// SPI handles
int OUT, IN;

// poll all the keys and write to the output buffer
void poll(int *buf) {
    
}

int main() {
    // init
    gpioInitialise();
    // 8 = 0b1000 means that CE0 is active low and CE1 is active high
    // since the latch is low when shifting out to the 595 and high when shifting in from the 165
    OUT = spiOpen(0, 1000000, 8);
    IN  = spiOpen(1, 1000000, 8);

    

    // cleanup
    spiClose(OUT);
    spiClose(IN);
    gpioTerminate();
}
