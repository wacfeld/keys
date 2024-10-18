import usb_midi

_in = usb_midi.ports[0]
_out = usb_midi.ports[1]

def read(n):
    return _in.read(n)

def write(*args):
    _out.write(bytes(args))

def noteOn(note, vel=127, channel=0):
    assert 0 <= note <= 127
    assert 0 <= vel <= 127
    assert 0 <= channel < 16
    write(0x90 + channel, note, vel)

def noteOff(note, vel=127, channel=0):
    assert 0 <= note <= 127
    assert 0 <= vel <= 127
    assert 0 <= channel < 16
    write(0x80 + channel, note, vel)

def setInst(inst, channel=0):
    assert 0 <= inst <= 127
    assert 0 <= channel < 16
    write(0xC0 + channel, inst)

def setVol(vol, channel=0):
    assert 0 <= vol <= 127
    assert 0 <= channel < 16
    write(0xB0 + channel, 7, vol)

def pitchBend(val, channel=0):
    assert 0 <= val < 0x4000
    assert 0 <= channel < 16
    L = val & 0x7F
    M = val >> 7
    write(0xE0 + channel, L, M)