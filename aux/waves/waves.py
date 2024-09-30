import struct

# write binary data to file
# data is a list of numbers
def write(fname, data):
    if not fname.endswith('.raw'):
        print(f'filename {fname} does not end in ".raw"')
        exit(1)
    # pack into big-endian 16-bit data
    content = struct.pack('>' + 'h' * len(data), *data)

    # write
    print(f'writing {len(data)} samples to {fname}')
    with open(fname, 'wb') as file:
        file.write(content)

def squarewave():
    data = (32767,) * 512 + (-32767,) * 512
