#include <Arduino.h>
#include <Timer.h>
#include <FastLED.h>


//break beam sensors
#define ir1Pin 18 //sweet
#define ir2Pin 19 //savory
#define ir3Pin 20 //spicy
#define ir4Pin 21 //sour
#define ir5Pin 22 //salty

//neopixel rings 24 per ring
#define leds_Pin 11
#define numRings 5
#define numPerRing 24
#define totalLeds numPerRing*numRings
CRGB tasteRings[totalLeds];

#define color1 75, 0, 130
#define color2 255, 105, 180
#define color3 100, 149, 237
#define color4 59, 211, 195
#define color5 124, 252, 0


//break into sections
//sweet, savory, spicy, sour, salty
CRGB* sweet = &tasteRings[0]; 
CRGB* savory = &tasteRings[(numPerRing * 1)]; 
CRGB* spicy = &tasteRings[(numPerRing * 2)]; 
CRGB* sour = &tasteRings[(numPerRing * 3)]; 
CRGB* salty = &tasteRings[(numPerRing * 4)]; 

#define fade 25
bool winTrigger = false;
MoToTimer ledDelay,winTimer,fade1Timer,fade2Timer,fade3Timer,fade4Timer,fade5Timer, winTrigTimer;

