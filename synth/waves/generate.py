from waves import *
from myio import write

if __name__ == '__main__':
    # basic shapes
    write(sine(), 'sine.raw', overwrite=True)
    write(square(), 'square.raw', overwrite=True)
    write(triangle(), 'triangle.raw', overwrite=True)
    write(sawtooth(), 'sawtooth.raw', overwrite=True)

    write(pulse(10), "pulse10.raw", overwrite=True)
    write(pulse(20), "pulse20.raw", overwrite=True)
    write(pulse(30), "pulse30.raw", overwrite=True)
    write(pulse(40), "pulse40.raw", overwrite=True)

    write(quad(triangle), 'quadtriangle.raw', overwrite=True)
    write(quad(sine), 'quadsine.raw', overwrite=True)
    write(quad(square), 'quadsquare.raw', overwrite=True)
    write(quad(sawtooth), 'quadsawtooth.raw', overwrite=True)

    write(sync(sawtooth, SEMITONE**4), 'sync4sawtooth.raw', overwrite=True)
    write(sync(sawtooth, SEMITONE**8), 'sync8sawtooth.raw', overwrite=True)
    write(sync(sawtooth, SEMITONE**16), 'sync16sawtooth.raw', overwrite=True)
    write(sync(sawtooth, SEMITONE**20), 'sync20sawtooth.raw', overwrite=True)
