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

    ## remove trailing tiny values
    #if threshold != None:
    #    # get max
    #    m = np.max(pos)

    #    # array of booleans flagging very small values
    #    small = (pos/m < threshold)
    #    # indices of non-small values
    #    nonsmalls = np.where(np.logical_not(small))[0]
    #    if len(nonsmalls) != 0:
    #        # get index of last non-small value, add 2, and cap at length of array
    #        i = min(nonsmalls[-1] + 2, len(pos))
    #        pos = pos[:i]

    # make y axis logarithmic
    pos = 10*np.log10(pos)
    pos[pos==-inf] = -200

    plt.plot(pos)
    if title is not None:
        plt.title(title)
    plt.xlabel('Hz')
    plt.ylabel('db')
    plt.ylim(bottom=-100)
    #plt.xlim(right=50)
    plt.xlim(left=0)
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
