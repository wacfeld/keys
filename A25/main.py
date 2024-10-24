import board
import digitalio
import time

pin = digitalio.DigitalInOut(board.GP16)
pin.direction = digitalio.Direction.INPUT
pin.pull = digitalio.Pull.UP

a = [0,0]
while True:
    val = pin.value
    if val:
        a[0]+=1
    else:
        a[1]+=1
    print(a)