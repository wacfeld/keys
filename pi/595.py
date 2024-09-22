## 74HC595 testing

import pigpio
import time

DATA = 10
CLOCK = 11
LATCH = 8

pi = pigpio.pi()

# bit bang some data
# A is a list of 0s and 1s
def bang(A):
    pi.write(CLOCK, 0)
    pi.write(LATCH, 0)
    
    for val in A:
        pi.write(CLOCK, 0)
        pi.write(DATA, val)
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

n = 2**16
# compare the performance of bit banging with SPI
def time_spi(baud):
    # precompute the data
    spi_data = [(i%256, i//256) for i in range(n)]
    
        
    print("timing SPI...")
    h = pi.spi_open(0, baud, 0)
    a=time.time_ns()
    for data in spi_data:
        pi.spi_write(h, data)
    b = time.time_ns()
    pi.spi_close(h)
    
    ms = (b-a)/1000000
    print(f"SPI took {ms} ms to make {n} writes, i.e. {1000*n/ms} writes a second")
    
        
def time_bang():
    bang_data = [tuple(map(int, tuple(f'{i:016b}'))) for i in range(n)]
    print("timing bit bang...")
    a = time.time_ns()
    for data in bang_data:
        bang(data)
    b = time.time_ns()
    ms = (b-a)/1000000
    print(f"bit bang took {ms} ms to make {n} writes, i.e. {1000*n/ms} writes a second")

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
