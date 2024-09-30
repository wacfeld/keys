from write import write

data = (32767,) * 512 + (-32767,) * 512
write('squarewave.py', data)
