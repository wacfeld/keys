from waves import *
from myio import write

if __name__ == '__main__':
    # basic shapes
    write(sine(), 'sine.raw', overwrite=True)
    write(square(), 'square.raw', overwrite=True)
    write(triangle(), 'triangle.raw', overwrite=True)
    write(sawtooth(), 'sawtooth.raw', overwrite=True)

    # marimbas
    write(marimba(), 'marimba.raw', overwrite=True)
    write(marimba2(), 'marimba2.raw', overwrite=True)
    write(marimba3(), 'marimba3.raw', overwrite=True)
