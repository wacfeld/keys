import usb_midi

_out = usb_midi.ports[1]
def write(*args):
    _out.write(bytes(args))

def noteOn(note, vel=127, channel=0):
    pass

def noteOff(note, vel=127, channel=0):
    pass

def setInst(inst, channel=0):
    pass

def setVol(vol, channel=0):
    pass

def pitchBend(val, channel=0):
    pass