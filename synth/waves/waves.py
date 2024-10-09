import numpy as np

from viz import plot, freqs
from myio import read, write

SEMITONE = 2**(1/12)
INTMAX = 32767

## operators

def double(data):
    # every other element
    half = data[::2]
    return np.concatenate((half, half))

## basic waves

def sine():
    data = INTMAX * np.sin(np.linspace(0, np.pi*2, 1024, endpoint=False))
    return data.astype(int)

def square():
    data = (INTMAX,) * 512 + (-INTMAX,) * 512
    return np.array(data)

def triangle():
    data = [INTMAX*i//256 - INTMAX for i in range(512)] + [-INTMAX*i//256 + INTMAX for i in range(512)]
    #data = [INTMAX*i//256 for i in range(256)] + [-INTMAX*i//256 + INTMAX for i in range(512)] + [INTMAX*i//256 - INTMAX for i in range(256)]
    return np.array(data)

def sawtooth():
    data = [INTMAX*i//512 - INTMAX for i in range(1024)]
    #data = [INTMAX*i//512 for i in range(512)] + [INTMAX*i//512 - INTMAX for i in range(512)]
    return np.array(data)

## functionals

# consume another wave function and add it to its quadrupling
def quad(f):
    d = f()
    d4 = double(double(f()))
    data = (d + d4/4) / 2
    return data.astype(int)
# write(quad(triangle), 'quadtriangle.raw'))
# write(quad(sine), 'quadsine.raw'))
# write(quad(square), 'quadsquare.raw'))
# write(quad(sawtooth), 'quadsawtooth.raw'))

# scale f up by c and cut it off
def sync(f, c):
    data = f()
    i = int(1024*c)
    data = np.resize(data, i)
    return data
# write(sync(sawtooth, SEMITONE**4), 'sync4sawtooth.raw')
# write(sync(sawtooth, SEMITONE**8), 'sync8sawtooth.raw')
# write(sync(sawtooth, SEMITONE**16), 'sync16sawtooth.raw')
# write(sync(sawtooth, SEMITONE**20), 'sync20sawtooth.raw')

## sophisticated waves

def marimba():
    tri = triangle()
    # triangle wave tuned up 2 octaves
    tri4 = double(double(triangle()))
    mar = (tri + tri4) // 2
    return mar

def marimba2():
    tri = triangle()
    # triangle wave tuned up 2 octaves
    tri4 = double(double(triangle()))
    mar = (tri + tri4*3/4) / 1.75
    return mar.astype(int)

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
    data = (INTMAX,) * a + (-INTMAX,) * b
    return np.array(data)

# write(pulse(10), "pulse10.raw")
# write(pulse(20), "pulse20.raw")
# write(pulse(30), "pulse30.raw")
# write(pulse(40), "pulse40.raw")
