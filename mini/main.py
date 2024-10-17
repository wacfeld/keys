import matrix
import qmidi

OUTPUTS = 5
INPUTS = 5
MIDIOFFSET = 104

if __name__ == '__main__':
    matrix.init(OUTPUTS, INPUTS)
    qmidi.init(5, 127)
    
    held = set()
    while True:
        buffer = matrix.poll()
        
        for i in range(len(buffer)):
            if buffer[i] == 1 and i not in held:
                held.add(i)
                qmidi.noteOn(MIDIOFFSET + i)
            elif buffer[i] == 0 and i in held:
                held.remove(i)
                qmidi.noteOff(MIDIOFFSET + i)
    
    matrix.cleanup()