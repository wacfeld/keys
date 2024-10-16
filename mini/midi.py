import usb_midi

_out = usb_midi.ports[1]
def write(*args):
    _out.write(bytes(args))

def noteOn(note, vel=127, channel=0):
    assert 0 <= note and note <= 127
    assert 0 <= vel and vel <= 127
    assert 0 <= channel and channel < 16
    write(0x90 + channel, note, vel)

def noteOff(note, vel=127, channel=0):
    assert 0 <= note and note <= 127
    assert 0 <= vel and vel <= 127
    assert 0 <= channel and channel < 16
    write(0x80 + channel, note, vel)

def setInst(inst, channel=0):
    assert 0 <= inst and inst <= 127
    assert 0 <= channel and channel < 16
    write(0xC0 + channel, inst)

def setVol(vol, channel=0):
    assert 0 <= vol and vol <= 127
    assert 0 <= channel and channel < 16
    write(0xB0 + channel, 7, vol)

def pitchBend(val, channel=0):
    assert 0 <= val and val < 0x4000
    assert 0 <= channel and channel < 16
    L = val & 0x7F
    M = val >> 7
    message(0xE0 + channel, L, M)