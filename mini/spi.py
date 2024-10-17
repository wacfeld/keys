import busio
import board
import digitalio

cs = digitalio.DigitalInOut(board.GP17)
cs.direction = digitalio.Direction.OUTPUT

spi = busio.SPI(board.GP18, MOSI=board.GP19)

while not spi.try_lock():
    pass

spi.configure(baudrate=5000000, phase=0, polarity=0)
cs.value = False
dat = bytes([0b00011111])
spi.write(dat)
cs.value = True

spi.unlock()