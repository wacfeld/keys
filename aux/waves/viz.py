#!/bin/env python

## visualize/inspect *.raw data

import sys
import struct
import matplotlib.pyplot as plt

def plot(data, title=None):
    # plot it
    plt.plot(data)
    plt.xlabel('sample')
    plt.ylabel('amplitude')
    if title is not None:
        plt.title(title)
    for y in (0, 32767, -32767):
        plt.axhline(y=y, color='black', lw=0.5, alpha=0.2)
    plt.show()

if __name__ == '__main__':
    # enforce proper usage
    if len(sys.argv) == 1:
        print(f'Usage: {sys.argv[0]} FILENAME [OPTIONS]\nwhere FILENAME contains big-endian 16-bit raw data')
        exit(1)

    # read and unpack data
    fname = sys.argv[1]
    opts = sys.argv[2:]
    with open(fname, mode='rb') as file:
        content = file.read()

    data = struct.unpack('>' + 'h' * (len(content)//2), content)

    # print some info
    if '--print' in opts:
        print(data)
    print(f'data contains {len(data)} values')
    if len(data) >= 2:
        print(f'starts with {data[0]}, ends with {data[-1]}')
        print(f'max {max(data)}, min {min(data)}')

    plot(data, fname)
