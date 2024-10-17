import busio
import board
import digitalio

def init_matrix():
    global _cs595, _cs165, _spi
    # initialize latch pins
    _cs595 = digitalio.DigitalInOut(board.GP1)
    _cs595.direction = digitalio.Direction.OUTPUT
    _cs165 = digitalio.DigitalInOut(board.GP17)
    _cs165.direction = digitalio.Direction.OUTPUT

    # clear latch pins
    _cs595.value = False
    _cs165.value = False

    # connect to SPI
    _spi = busio.SPI(board.GP18, MOSI=board.GP19)
    while not _spi.try_lock():
        pass
    _spi.configure(baudrate=5000000, phase=0, polarity=0)

def cleanup_matrix():
    _spi.unlock()