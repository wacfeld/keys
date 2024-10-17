import busio
import board
import digitalio

# outputs, inputs are positive integers
def init_matrix(outputs, inputs):
    global _cs595, _cs165, _spi, _outputs, _inputs
    
    _outputs, _inputs, = outputs, inputs
    
    # initialize latch pins
    _cs595 = digitalio.DigitalInOut(board.GP1)
    _cs595.direction = digitalio.Direction.OUTPUT
    _cs165 = digitalio.DigitalInOut(board.GP17)
    _cs165.direction = digitalio.Direction.OUTPUT

    # set latch pins (595 active low, 165 active high)
    _cs595.value = True
    _cs165.value = False

    # connect to SPI
    _spi = busio.SPI(board.GP18, MOSI=board.GP19, MISO=board.GP16)
    while not _spi.try_lock():
        pass
    _spi.configure(baudrate=5000000, phase=0, polarity=0)

def cleanup_matrix():
    _spi.unlock()

def _write(data):
    _cs595.value = False
    _spi.write(data)
    _cs595.value = True

def _readinto(data):
    _cs165.value = True
    _spi.readinto(data)
    _cs165.value = False

def poll():
    buffer = [None] * _inputs * _outputs

    for i in range(_outputs):
        # compute output data (one hot)
        # e.x. i == 9 -> data == b'\x02\x00'
        data = bytes([(1 << i % 8)]) + bytes(i//8)
        _write(data)

        # allocate ceil(inputs/8) bytes for input
        result = bytearray(-(_inputs//-8))
        _readinto(result)
        # transfer to buffer
        for j in range(_inputs):
            if j % 8 == 0 and j > 0:
                # TODO check this is not backwards
                # remove last byte as it's been read
                result = result[:-1]
            
            buffer[j * _outputs + i] = result[-1] % 2
            result[-1] //= 2
    
    return buffer