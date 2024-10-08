import struct
import os

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
    with open(fname, 'wb') as file:
        file.write(content)
        print(f'wrote {len(data)} samples to {fname}')

def read(fname):
    with open(fname, 'rb') as file:
        content = file.read()
    data = struct.unpack('>' + 'h' * (len(content)//2), content)
    return data
