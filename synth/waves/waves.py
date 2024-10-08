import numpy as np

from viz import plot, freqs
from myio import read, write

## operators

def double(data):
    # every other element
    half = data[::2]
    return np.concatenate((half, half))

## basic waves

def sine():
    data = 32767 * np.sin(np.linspace(0, np.pi*2, 1024, endpoint=False))
    return data.astype(int)

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

def quad(f):
    d = f()
    d4 = double(double(f()))
    data = (d + d4/4) / 2
    return data.astype(int)
# write(quad(triangle), 'quadtriangle.raw'))
# write(quad(sine), 'quadsine.raw'))
# write(quad(square), 'quadsquare.raw'))
# write(quad(sawtooth), 'quadsawtooth.raw'))

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

# duty is a number from 0 to 100
def pulse(duty):
    a = int(1024 * duty/100)
    b = 1024 - a
    data = (32767,) * a + (-32767,) * b
    return np.array(data)

# write(pulse(10), "pulse10.raw")
# write(pulse(20), "pulse20.raw")
# write(pulse(30), "pulse30.raw")
# write(pulse(40), "pulse40.raw")
