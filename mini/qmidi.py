import midi

def init(inst=0, vol=127):
    setInst(inst)
    setVol(vol)
    midi.pitchBend(0x2800, 1)
    midi.pitchBend(0x2000, 0)

def noteOn(note, vel=127):
    assert 0 <= note <= 255
    midi.noteOn(note // 2, vel, note % 2)

def noteOff(note, vel=127):
    assert 0 <= note <= 255
    midi.noteOff(note // 2, vel, note % 2)

def setVol(vol):
    midi.setVol(vol, 0)
    midi.setVol(vol, 1)

def setInst(inst):
    midi.setInst(inst, 0)
    midi.setInst(inst, 1)