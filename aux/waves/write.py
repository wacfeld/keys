import struct

# write binary data to file
# data is a list of numbers
def write(data, fname):
    # pack into big-endian 16-bit data
    content = struct.pack('>' + 'h' * len(data))

    # write
    print(f'writing {len(data)} samples to {fname}')
    with open(fname, 'wb') as file:
        file.write(content)
