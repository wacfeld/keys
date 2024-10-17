import busio
import board
import digitalio

# outputs, inputs, and keys are positive integers
# keys <= outputs * inputs
def init_matrix(outputs, inputs, keys):
    global _cs595, _cs165, _spi, _outputs, _inputs, _keys
    
    _outputs, _inputs, _keys = outputs, inputs, keys
    
    # initialize latch pins
    _cs595 = digitalio.DigitalInOut(board.GP1)
    _cs595.direction = digitalio.Direction.OUTPUT
    _cs165 = digitalio.DigitalInOut(board.GP17)
    _cs165.direction = digitalio.Direction.OUTPUT

    # set latch pins (595 active low, 165 active high)
    _cs595.value = True
    _cs165.value = False

    # connect to SPI
    _spi = busio.SPI(board.GP18, MOSI=board.GP19)
    while not _spi.try_lock():
        pass
    _spi.configure(baudrate=5000000, phase=0, polarity=0)

def cleanup_matrix():
    _spi.unlock()

def _write(data):
    _cs595.level = False
    _spi.write(data)
    _cs595.level = True

def _readinto(data):
    _cs165.level = True
    _spi.readinto(data)
    _cs165.level = False

def poll():
    buffer = [None] * _keys
    c = 0

    for i in range(_outputs):
        # compute output data (one hot)
        # e.x. i == 9 -> data == b'\x02\x00'
        data = bytes([(1 << i % 8)]) + bytes(i//8)
        _write(data)

        # allocate ceil(inputs/8) bytes for input
        result = bytearray(-(inputs//-8))
        _readinto(result)
        
        # write to buffer