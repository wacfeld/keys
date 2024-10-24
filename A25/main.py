from machine import Pin
import time

pin = Pin(16, Pin.IN, Pin.PULL_UP)

while True:
    print(pin.value())
    time.sleep(0.1)