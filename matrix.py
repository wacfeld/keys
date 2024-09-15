# Complete project details at https://RandomNerdTutorials.com/raspberry-pi-pico-analog-inputs-micropython/
from machine import Pin, PWM
from utime import sleep, ticks_us

# gpio pin numbers
ingpio = (2, 3, 4, 5)
outgpio = (6, 7, 8, 9)
buzzgpio = (17, 19, 21, 26)

# keyboard matrix pins
outputs = [Pin(n, Pin.OUT) for n in outgpio]
inputs = [Pin(n, Pin.IN, Pin.PULL_DOWN) for n in ingpio]
nkeys = len(outputs) * len(inputs)

# buzzers
buzzers = [PWM(Pin(n)) for n in buzzgpio]
# keep track of which buzzer is playing what note
buffer = [None for n in buzzgpio]


freqs = [262, 277, 294, 311, 330, 348, 370, 392, 415, 440, 466, 494, 523, 554, 587, 622]

verbose = True

def message(msg):
    if verbose:
        print(msg)

def buzz(pin, freq):
    pin.freq(freq)
    pin.duty_u16(30000)

def unbuzz(pin):
    pin.duty_u16(0)

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
    print(i)
    buzz(buzzers[i], hz)
    buffer[i] = hz
    message(f"playing {hz} Hz")

# stop the buzzer playing the given frequency
def stop(hz):
    global buffer
    
    if hz not in buffer:
        message(f"{hz} Hz not in buffer, cannot stop")
        return
    
    i = buffer.index(hz)
    unbuzz(buzzers[i])
    buffer[i] = None
    message(f"stopped playing {hz} Hz")

# stop all buzzers
def stopall():
    for b in buzzers:
        unbuzz(b)

# poll every key and return an array of booleans
def poll():
    A = [None for i in range(nkeys)]
    
    # clear every output pin to be safe
    for i in range(len(outputs)):
        outputs[i].off()
    
    for i in range(len(outputs)):
        outputs[i].on()
        
        for j in range(len(inputs)):
            val = inputs[j].value()
            A[j*len(outputs) + i] = val
        
        outputs[i].off()
    
    return A

# time a function (returns microseconds)
def timeit(f, n=500):
    a = ticks_us()
    for i in range(n):
        f()
    b = ticks_us()
    return (b-a)/n

def sing():
    A = [0 for i in range(nkeys)]
    while True:
        newA = poll()
        for i in range(nkeys):
            if A[i] != newA[i]:
                if newA[i] == 0:
                    stop(freqs[i])
                    print(buffer)
                else:
                    play(freqs[i])
                    print(buffer)
        A = newA

#while True:
#    print(poll())
#    sleep(0.1)