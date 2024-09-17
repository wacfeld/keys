from machine import Pin
import utime

datapin = 13
clockpin = 14
latchpin = 15

data = Pin(datapin, Pin.OUT)
latch = Pin(latchpin, Pin.OUT)
clock = Pin(clockpin, Pin.OUT)

def shift_update(A):
  #put latch down to start data sending
  clock.value(0)
  latch.value(0)
  clock.value(1)
  
  #load data in reverse order
  for i in range(7, -1, -1):
    clock.value(0)
    data.value(int(A[i]))
    clock.value(1)

  #put latch up to store data on register
  clock.value(0)
  latch.value(1)
  clock.value(1)

def count():
    for i in range(256):
        s = f"{i:08b}"
        shift_update(s)
        utime.sleep(1)