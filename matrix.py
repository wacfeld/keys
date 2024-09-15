# Complete project details at https://RandomNerdTutorials.com/raspberry-pi-pico-analog-inputs-micropython/
from machine import Pin, PWM
from picozero import Speaker
from time import sleep

# gpio pin numbers
ingpio = (2, 3, 4, 5)
outgpio = (6, 7, 8, 9)
buzzgpio = (18, 19, 20, 21, 22)

# keyboard matrix pins
outputs = [Pin(n, Pin.OUT) for n in outgpio]
inputs = [Pin(n, Pin.IN, Pin.PULL_DOWN) for n in ingpio]

# buzzers
buzzers = [Speaker(n) for n in buzzgpio]
# keep track of which buzzer is playing what note
buffer = [None for n in buzzgpio]


freqs = [262, 277, 294, 311, 330, 348, 370, 392, 415, 440, 466, 494, 523, 554, 587, 622]

verbose = True

def message(msg):
    if verbose:
        print(msg)

# play a frequency on an available buzzer
def play(hz):
    global buffer
    
    if hz in buffer:
        message(f"{hz} Hz already being played")
        return
    
    if None not in buffer:
        message(f"all buzzers are in use, cannot play {hz} Hz")
        return
    
    i = buffer.index(None)
    try:
        buzzers[i].play(hz, duration=None)
    except:
        pass
    buffer[i] = hz
    message(f"playing {hz} Hz")

# stop the buzzer playing the given frequency
def stop(hz):
    global buffer
    
    if hz not in buffer:
        message(f"{hz} Hz not in buffer, cannot stop")
        return
    
    i = buffer.index(hz)
    buzzers[i].off()
    buffer[i] = None
    message(f"stopped playing {hz} Hz")

# stop all buzzers
def stopall():
    for b in buzzers:
        b.off()

# poll every key and return a 2d array of booleans
#def poll():