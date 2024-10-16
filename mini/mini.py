mport usb_midi
import time

out = usb_midi.ports[1]

out.write(bytes([192,5]))
out.write(bytes([176,7,100]))
out.write(bytes([144,64,90]))
time.sleep(1)
out.write(bytes([128,64,40]))
