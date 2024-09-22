## 74HC595 testing

import pigpio
import time

DATA = 10
CLOCK = 11
LATCH = 8

pi = pigpio.pi()

# bit bang some data
# A is a string of 0s and 1s
def bang(A):
    pi.write(CLOCK, 0)
    pi.write(LATCH, 0)
    
    for c in reversed(A):
        pi.write(CLOCK, 0)
        pi.write(DATA, int(c))
        pi.write(CLOCK, 1)
    
    pi.write(CLOCK, 0)
    pi.write(LATCH, 1)

# write some data using SPI
# data is a list of integers
def spi(data):
    # spi channel 0, 1MHz, mode 0
    h = pi.spi_open(0, 1000000, 0)
    pi.spi_write(h, data)
    pi.spi_close(h)

def count():
    for i in range(2**16):
        L = i % 256
        H = i // 256
        spi([L,H])
        time.sleep(0.01)

# bit bang a moving light pattern
def knightrider():
    A = [1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1]
    # reset
    pi.write(CLOCK, 0)
    pi.write(LATCH, 0)
    
    i = 0
    while True:
        i = i % 16
        val = 0 if i < 8 else 1
        
        pi.write(DATA, val)
        pi.write(CLOCK, 1)
        pi.write(CLOCK, 0)
        pi.write(LATCH, 1)
        pi.write(LATCH, 0)
        time.sleep(0.05)
        i += 1
