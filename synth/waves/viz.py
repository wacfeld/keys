#!/bin/env python

## visualize/inspect *.raw data

import sys
import struct
import matplotlib.pyplot as plt
import numpy as np
from numpy import inf
from numpy.fft import fft

from myio import read

def plot(data, xdata=None, title=None, hlines=None):
    # plot it
    if xdata is None:
        plt.plot(data)
    else:
        plt.plot(xdata, data)
    plt.xlabel('sample')
    plt.ylabel('amplitude')
    if title is not None:
        plt.title(title)
    if hlines is not None:
        for y in hlines:
            plt.axhline(y=y, color='black', lw=0.5, alpha=0.2)
    plt.show()

def freqs(data, title=None, threshold=0.001):
    # take the fft, absolute value, and get the positive frequencies
    ft = np.abs(fft(data))
    pos = ft[:len(ft)//2]

    plt.scatter(np.arange(len(pos)), pos)
    ax = plt.gca()
    ax.set_xscale('log')
    ax.set_yscale('log')

    if title is not None:
        plt.title(title)
    plt.xlabel('freq')
    plt.ylabel('amplitude')

    plt.axvline(x=1)
    plt.axhline(y=1)

    plt.show()

if __name__ == '__main__':
    # enforce proper usage
    if len(sys.argv) == 1:
        print(f'Usage: {sys.argv[0]} FILENAME [OPTIONS]\nwhere FILENAME contains big-endian 16-bit raw data')
        exit(1)

    # read and unpack data
    fname = sys.argv[1]
    opts = sys.argv[2:]
    data = read(fname)

    # print some info
    if '--print' in opts:
        print(data)
    print(f'data contains {len(data)} values')
    if len(data) >= 2:
        print(f'starts with {data[0]}, ends with {data[-1]}')
        print(f'max {max(data)}, min {min(data)}')

    if '--freqs' in opts:
        freqs(data, title=fname)
    else:
        plot(data, title=fname, hlines=(0, 32767, -32767))
