import sounddevice as sd
import soundfile as sf
from gpiozero import Button, PWMLED, DigitalOutputDevice

#configure gpio button & led
playSound = Button(26, pull_up = True)
swLED = PWMLED(12)
stripRelay = DigitalOutputDevice(14)

#assign sounds files
sound1 = '/media/pi/AUDIO/1.wav'
sound2 = '/media/pi/AUDIO/2.wav'
sound3 = '/media/pi/AUDIO/3.wav'
sound4 = '/media/pi/AUDIO/4.wav'
sound5 = '/media/pi/AUDIO/5.wav'
sound6 = '/media/pi/AUDIO/6.wav'

#load the files to RAM
play1, fs1 = sf.read(sound1)
play2, fs2 = sf.read(sound2)
play3, fs3 = sf.read(sound3)
play4, fs4 = sf.read(sound4)
play5, fs5 = sf.read(sound5)
play6, fs6 = sf.read(sound6)

#set starting position
currentAud = play1
fs = fs1
swTrack = 0;

def switchTrack(track):
    track += 1
    
    if track == 6:
        #reset the track position
        track = 0
    print("current track: " + str(track))
    match track:
        case 0:
            Aud = play1
            fsTemp = fs1
        case 1:
            Aud = play2
            fsTemp = fs2
        case 2:
            Aud = play3
            fsTemp = fs3
        case 3:
            Aud = play4
            fsTemp = fs4
        case 4:
            Aud = play5
            fsTemp = fs5
        case 5:
            Aud = play6
            fsTemp = fs6
            
    return track, Aud, fsTemp


while True:

    if playSound.is_pressed:
        print("play track")
        swTrack,currentAud,fs = switchTrack(swTrack);
        swLED.off()
        #if the strip is off when on function triggers
        #we need to switch the on/off functions around
        #as the strip must be looking for a low instead of high
        stripRelay.on()
        sd.play(currentAud, fs, blocking = True) #Blocks until Audio finishes
        stripRelay.off()
        print("done playing")
        swLED.on()
