import struct
import numpy as np
from viz import plot

## write data

# write binary data to file
# data is a list of numbers
def write(fname, data):
    if not fname.endswith('.raw'):
        print(f'filename {fname} does not end in ".raw"')
        exit(1)
    # pack into big-endian 16-bit data
    content = struct.pack('>' + 'h' * len(data), *data)

    # write
    print(f'writing {len(data)} samples to {fname}')
    with open(fname, 'wb') as file:
        file.write(content)

## operators

def double(data):
    # every other element
    half = data[::2]
    return np.concatenate((half, half))

## basic waves

def square():
    data = (32767,) * 512 + (-32767,) * 512
    return np.array(data)

def triangle():
    data = [32767*i//256 for i in range(256)] + [-32767*i//256 + 32767 for i in range(512)] + [32767*i//256 - 32767 for i in range(256)]
    return np.array(data)

def sawtooth():
    data = [32767*i//512 for i in range(512)] + [32767*i//512 - 32767 for i in range(512)]
    return np.array(data)

## sophisticated waves

def marimba():
    tri = triangle()
    # triangle wave tuned up 2 octaves
    tri4 = double(double(triangle()))
    return (tri + tri4) / 2