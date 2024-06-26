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
//they should be daisychained, they are logically split
#define leds_Pin 11
#define numRings 5
#define numPerRing 24
#define totalLeds numPerRing*numRings
CRGB tasteRings[totalLeds];

//color def in RGB adjust as needed
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

//set the timers
#define fade 25
MoToTimer ledDelay,winTimer,resetTimer,heartbeat;

//set all the flags
//defaults to false
bool winTrigger = false;
bool ir1,ir2,ir3,ir4,ir5;
bool ir1T,ir2T,ir3T,ir4T,ir5T;
bool on = true;
int ledPos = 0;

//comment out to turn off debug, but it won't hurt to leave
#define debug