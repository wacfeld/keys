import struct
import numpy as np
import os

from viz import plot

## write data

# write binary data to file
# data is a list of numbers
def write(data, fname, overwrite=False):
    if os.path.isfile(fname):
        if overwrite:
            print(f'overwriting {fname}')
        else:
            print(f'file {fname} already exists')
            return
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

def marimba2():
    tri = triangle()
    # triangle wave tuned up 2 octaves
    tri4 = double(double(triangle()))
    mar = (tri + tri4*3/4) / 1.75
    return mar.astype(int)

def marimba():
    tri = triangle()
    # triangle wave tuned up 2 octaves
    tri4 = double(double(triangle()))
    mar = (tri + tri4) // 2
    return mar

def marimba3():
    tri = triangle()
    # triangle wave tuned up 2 octaves
    tri4 = double(double(triangle()))
    mar = (tri + tri4/2) / 1.5
    return mar.astype(int)
