#!/bin/env python

## visualize/inspect *.raw data

import re
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

def freqs(data, title=None, threshold=1e-5):
    # take the fft and take the absolute value
    ft = np.abs(fft(data))
    # discard the negative frequencies
    pos = ft[:len(ft)//2]

    # zero out anything below the threshold (noise)
    if threshold is not None:
        m = np.max(pos)
        pos[pos/m < threshold] = 0

    # plot
    plt.scatter(np.arange(len(pos)), pos)

    # log axes
    ax = plt.gca()
    ax.set_xscale('log')
    ax.set_yscale('log')

    # labels
    if title is not None:
        plt.title(title)
    plt.xlabel('freq')
    plt.ylabel('amplitude')

    # reference lines
    plt.axvline(x=1, color='black', lw=0.5, alpha=0.2)
    plt.axhline(y=1, color='black', lw=0.5, alpha=0.2)

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

    # TODO do proper python getopt and then have a --threshold option for freqs()

    if '--freqs' in opts:
        freqs(data, title=fname)
    else:
        plot(data, title=fname, hlines=(0, 32767, -32767))
