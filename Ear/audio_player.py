#!/usr/bin/env python

import librosa
import sounddevice as sd
from gpiozero import Button, PWMLED
import time
import board
import digitalio
from adafruit_seesaw.seesaw import Seesaw
from adafruit_seesaw.digitalio import DigitalIO

i2c = board.I2C()

print("Sleep to give i2c a chance to link")
time.sleep(2)

try:
    arcade_1 = Seesaw(i2c, addr=0x3A)
    arcade_2 = Seesaw(i2c, addr=0x3B)
    arcade_3 = Seesaw(i2c, addr=0x3C)
except Exception as e:
    try:
        print("I2C did not connect, attempting again...")
        print("Error: " + e)
        arcade_1 = Seesaw(i2c, addr=0x3A)
        arcade_2 = Seesaw(i2c, addr=0x3B)
        arcade_3 = Seesaw(i2c, addr=0x3C)
    except Exception as e:
        print("Failed to connect twice, exiting.")
        print("Error: " + e)
        exit()
    
print("Connected to i2c!")
#arcade_group = (arcade_1, arcade_2, arcade_3)

# Button Layout starting at top of pitch selection:
# Arcade_1:
# 0 - btn1:12
# 1 - btn2:10
# 2 - btn3:8
# 3 - btn4:6
# 
# Arcade_2:
# 4 - btn5:4
# 5 - btn6:2
# -------------
# 6 - btn7:-2
# 7 - btn8:-4
# 
# Arcade_3:
# 8 - btn9:-6
# 9 - btn10:-8
# 10 - btn11:-10
# 11 - btn12:-12

print("set button pins")
#setup buttons
#(1,2,3,4)
button_pins = (18, 19, 20, 2)
buttons = []
for arcade_btn in arcade_group:
    for button_pin in button_pins:
        button = DigitalIO(arcade_btn, button_pin)
        button.direction = digitalio.Direction.INPUT
        button.pull = digitalio.Pull.UP
        buttons.append(button)

print("set led pins")
#setup LEDs
#(1,2,3,4)        
led_pins = (12, 13, 0, 1)
leds = []
for arcade_led in arcade_1:
    for led_pin in led_pins:
        led = DigitalIO(arcade_led, led_pin)
        led.direction = digitalio.Direction.OUTPUT
        leds.append(led)       


#load sounds files
sound1 = '/media/pi/AUDIO/1.wav'
sound2 = '/media/pi/AUDIO/2.wav'
sound3 = '/media/pi/AUDIO/3.wav'
sound4 = '/media/pi/AUDIO/4.wav'
sound5 = '/media/pi/AUDIO/5.wav'
sound6 = '/media/pi/AUDIO/6.wav'

#configure gpio button & led
switchAudio = Button(4)
swLED = PWMLED(13, frequency=1000)

print("Attempting to load audio...")

#load audio into ram & load SR as is
try:
    print("Loading Audio 1")
    a1, sr1 = librosa.load(sound1,sr=None)

except Exception as e:
    print("failed to load Audio 1")
    print("reason: " + e)
    
try:
    print("Loading Audio 2")
    a2, sr2 = librosa.load(sound2,sr=None)

except Exception as e:
    print("failed to load Audio 2")
    print("reason: " + e)

try:
    print("Loading Audio 3")    
    a3, sr3 = librosa.load(sound3,sr=None)
except Exception as e:
    print("failed to load Audio 3")
    print("reason: " + e)

try:
    print("Loading Audio 4")    
    a4, sr4 = librosa.load(sound4,sr=None)
except Exception as e:
    print("failed to load Audio 4")
    print("reason: " + e)

try:
    print("Loading Audio 5")
    a5, sr5 = librosa.load(sound5,sr=None)
except Exception as e:
    print("failed to load Audio 5")
    print("reason: " + e)

try:
    print("Loading Audio 6")
    a6, sr6 = librosa.load(sound6,sr=None)
except Exception as e:
    print("failed to load Audio 6")
    print("reason: " + e)

##create and load vars
audioPos = 0
currentAud = a1
currentSr = sr1

step = 0
playOnce = False


timeout = 0.75
lastTime = 0
LedTimer = 0.01
lastLed = 0
ledVal = 0
forward = True



#pass audio and pitch amount
def pitch (currentAud, currentSr, pitchStep):
    print("Audio array: " + str(currentAud))
    print("Audio rate: " + str(currentSr))
    try:
        Pitched = librosa.effects.pitch_shift(currentAud, sr=currentSr, n_steps=pitchStep)
        sd.play(Pitched, currentSr, blocking =False)
    except Exception as e:
        print("Audio failed!")
        print("Error" + e)

#step through the sounds when button is pressed    
def changeSound(audPos,curAud, curSr):
    print("Current Audio: " + str(audPos))
    
    match audPos:
        case 0:
            #sd.stop()
            curAud = a1
            curSr = sr1
            sd.play(curAud, curSr, blocking =False)
            return curAud, curSr
        case 1:
            #sd.stop()            
            curAud = a2
            curSr = sr2
            sd.play(curAud, curSr, blocking =False)
            return curAud, curSr
        case 2:
            #sd.stop()            
            curAud = a3
            curSr = sr3
            sd.play(curAud, curSr, blocking =False)
            return curAud, curSr
        case 3:
            #sd.stop()            
            curAud = a4
            curSr = sr4
            sd.play(curAud, curSr, blocking =False)
            return curAud, curSr
        case 4:
            #sd.stop()            
            curAud = a5
            curSr = sr5
            sd.play(curAud, curSr, blocking =False)
            return curAud, curSr
        case 5:
            #sd.stop()            
            curAud = a6
            curSr = sr6
            sd.play(curAud, curSr, blocking =False)
            return curAud, curSr


#Loop loop loooooop
while True:
    #this controls the led buttons lights.
    #flip the logic if you want them to respond in the 
    #oppisite way.
    off = False
    on = True
    
    #track time to lock switchAudio for small amount of time
    currentTime = time.time()
    
    #Switch audio tracks and swap around vars
    if switchAudio.is_pressed and timeout < (currentTime-lastTime):
        lastTime = currentTime
        audioPos += 1
        if audioPos == 6:
            print("Audio Pos above 6: " + str(audioPos))
            audioPos = 0
            
        #Send to switch current audio  & return the updated var      
        currentAud, currentSr = changeSound(audioPos,currentAud,currentSr)

    #pulse swAud Light

    if LedTimer < (currentTime - lastLed):
        lastLed = currentTime
        if forward:
            ledVal += 1
            if ledVal == 100:
                forward = False
        else:
            ledVal -= 1
            if ledVal == 0:
                forward = True            
        #ledVal = round(ledVal, 1)
        swLED.value = ledVal/100
        
    #Pitch buttons are attached via I2C boards
#-------Start Pitch Shift Up-----------        
    if not buttons[0].value:
        print("Button 1 value: " + str(buttons[0].value))
        #shut leds off
        for leds_num, discard in enumerate(leds):
            leds[leds_num].value = off
        #turn on current led button
        leds[0].value = on        
        #pass pitch and play
        pitch(currentAud, currentSr, 12)

    if not buttons[1].value:
        print("Button 2 value: " + str(buttons[1].value))
        #shut leds off
        for leds_num, discard in enumerate(leds):
            leds[leds_num].value = off
        #turn on current led button
        leds[1].value = on
        #pass pitch and play
        pitch(currentAud, currentSr, 10)

    if not buttons[2].value:
        print("Button 3 value: " + str(buttons[2].value)
        #shut leds off
        for leds_num, discard in enumerate(leds):
            leds[leds_num].value = off
        #turn on current led button
        leds[2].value = on
        #pass pitch and play
        pitch(currentAud, currentSr, 8)
        
    if not buttons[3].value:
        print("Button 4 value: " + str(buttons[3].value)
        #shut leds off
        for leds_num, discard in enumerate(leds):
            leds[leds_num].value = off
        #turn on current led button
        leds[3].value = on
        #pass pitch and play
        pitch(currentAud, currentSr, 6)
        
    if not buttons[4].value:
        print("Button 5 value: " + str(buttons[4].value)
        #shut leds off
        for leds_num, discard in enumerate(leds):
            leds[leds_num].value = off
        #turn on current led button
        leds[4].value = on
        #pass pitch and play
        pitch(currentAud, currentSr, 4)
        
    if not buttons[5].value:
        print("Button 6 value: " + str(buttons[5].value)
        #shut leds off
        for leds_num, discard in enumerate(leds):
            leds[leds_num].value = off
        #turn on current led button
        leds[5].value = on
        #pass pitch and play
        pitch(currentAud, currentSr, 2)
        
#-------Start Pitch Shift Down-----------
    if not buttons[6].value:
        print("Button 7 value: " + str(buttons[6].value)
        #shut leds off
        for leds_num, discard in enumerate(leds):
            leds[leds_num].value = off
        #turn on current led button
        leds[6].value = on
        #pass pitch and play
        pitch(currentAud, currentSr, -2)

    if not buttons[7].value:
        print("Button 8 value: " + str(buttons[7].value)
        #shut leds off
        for leds_num, discard in enumerate(leds):
            leds[leds_num].value = off
        #turn on current led button
        leds[7].value = on
        #pass pitch and play
        pitch(currentAud, currentSr, -4)
        
    if not buttons[8].value:
        print("Button 9 value: " + str(buttons[8].value)
        #shut leds off
        for leds_num, discard in enumerate(leds):
            leds[leds_num].value = off
        #turn on current led button
        leds[8].value = on
        #pass pitch and play
        pitch(currentAud, currentSr, -6)
        
    if not buttons[9].value:
        print("Button 10 value: " + str(buttons[9].value)
        #shut leds off
        for leds_num, discard in enumerate(leds):
            leds[leds_num].value = off
        #turn on current led button
        leds[9].value = on
        #pass pitch and play
        pitch(currentAud, currentSr, -8)
        
    if not buttons[10].value:
        print("Button 11 value: " + str(buttons[10].value)
        #shut leds off
        for leds_num, discard in enumerate(leds):
            leds[leds_num].value = off
        #turn on current led button
        leds[10].value = on
        #pass pitch and play
        pitch(currentAud, currentSr, -10)
        
    if not buttons[11].value:
        print("Button 12 value: " + str(buttons[11].value)
        #shut leds off
        for leds_num, discard in enumerate(leds):
            leds[leds_num].value = off
        #turn on current led button
        leds[11].value = on
        #pass pitch and play
        pitch(currentAud, currentSr, -12)