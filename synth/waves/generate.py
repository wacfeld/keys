from waves import *
from myio import write

if __name__ == '__main__':
    write(sine(), 'sine.raw', overwrite=True)
