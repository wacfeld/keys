import pigpio
import time

DATA = 10
CLOCK = 11
LATCH = 8

pi = pigpio.pi()

# bit bang a byte
def shift(A):
    pi.write(CLOCK, 0)
    pi.write(LATCH, 0)
    
    for i in range(7, -1, -1):
        pi.write(CLOCK, 0)
        pi.write(DATA, int(A[i]))
        pi.write(CLOCK, 1)
    
    pi.write(CLOCK, 0)
    pi.write(LATCH, 1)

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
knightrider()