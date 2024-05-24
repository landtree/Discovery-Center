#include <Arduino.h>
#include <Timer.h>
#include <Bounce2.h>
#include <FastLED.h>


//used to check scent position
#define encode1 18
#define encode2 19
#define encode3 20

//used to check image position 
#define encode4 21
#define encode5 22
#define encode6 23


//Based on infro from Inscribed Engineering
//Picture Wheel --> Smell Wheel
// 1 --> 3
// 2 --> 6
// 3 --> 2
// 4 --> 5 
// 5 --> 1
// 6 --> 7
// 7 --> 4

//Encode number
#define img1 7 
#define img2 6 
#define img3 5 
#define img4 4
#define img5 3 
#define img6 2 
#define img7 1 
#define noimg 0 

//encode number
#define scent1 7 
#define scent2 6 
#define scent3 5 
#define scent4 4 
#define scent5 3 
#define scent6 2
#define scent7 1 
#define noScent 0 



//neopixel rings 24 per ring
#define leds_Pin 11
#define numLeds 24

CRGB ring[numLeds];

#define startBtnPin 1

MoToTimer buttonTimer,fanTimer,debugTimer;

Bounce2::Button startBtn = Bounce2::Button();

#define fan 10
bool fanOn, match;

int encodedScent = 0;
int encodedImg = 0;

#define debug