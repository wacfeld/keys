import board
import digitalio
from microcontroller import delay_us

pin = digitalio.DigitalInOut(board.GP16)
pin.direction = digitalio.Direction.INPUT
pin.pull = digitalio.Pull.UP

current = False
n = 0

while True:
    val = pin.value
    if val == current:
        n += 1
    else:
        print(f'{n} of {current}')
        current = val
        n = 0