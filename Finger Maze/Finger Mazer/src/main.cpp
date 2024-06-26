#include <Arduino.h>
#include <FastLED.h>
#include <Timer.h>
#include <wavTrigger.h>

//IR Definitions
#define irStartPin 7
#define irExitPin 5
#define ir1Pin 18 //texture 1
#define ir2Pin 19 //texture 2
#define ir3Pin 20 //texture 3
#define ir4Pin 21 //texture 4
#define ir5Pin 22 //texture 5
#define ir6Pin 23 //texture 6
#define ir7Pin 9 //EasterEgg
#define ir8Pin 10 //Not used
//Led defintions
#define leds_Pin 11
#define Num_leds 32
#define numPerSec 4
CRGB leds[Num_leds];


//break into sections
CRGB* startSection = &leds[0]; 
CRGB* texture1 = &leds[(numPerSec * 1)]; 
CRGB* texture2 = &leds[(numPerSec * 2)]; 
CRGB* texture3 = &leds[(numPerSec * 3)]; 
CRGB* texture4 = &leds[(numPerSec * 4)]; 
CRGB* texture5 = &leds[(numPerSec * 5)]; 
CRGB* texture6 = &leds[(numPerSec * 6)]; 
CRGB* exitSection = &leds[(numPerSec * 7)];

MoToTimer senTimeout,ledTimeout,gameTimeout,checkSensor,beat,winTimer,eggTimer;

#define gameTime 30000
#define senTime 500

bool senTriggered,beaton,secOn;

wavTrigger wavTrig;


void heartbeat()
{
  if(beaton)
  {
    digitalWrite(13, HIGH);  
    beaton = !beaton;
  }else{digitalWrite(13, LOW);beaton = !beaton;}
}
void setup() 
{
  //set pinmodes
  pinMode(13, OUTPUT);
  pinMode(irStartPin, INPUT);
  pinMode(irExitPin, INPUT);
  pinMode(ir1Pin, INPUT);
  pinMode(ir1Pin, INPUT);
  pinMode(ir2Pin, INPUT);
  pinMode(ir3Pin, INPUT);
  pinMode(ir4Pin, INPUT);
  pinMode(ir5Pin, INPUT);
  pinMode(ir6Pin, INPUT);
  pinMode(ir7Pin, INPUT);
  pinMode(ir8Pin, INPUT);

  //create led strip
  FastLED.addLeds<NEOPIXEL, leds_Pin>(leds, Num_leds).setCorrection( TypicalSMD5050 );
  //restricts brightness
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 2300);

  Serial.begin(115200);
  wavTrig.start();
  delay(10); //small delay for connection
  wavTrig.stopAllTracks();// incase of retart
  wavTrig.samplerateOffset();

  //test LEDs
  fill_solid(leds,Num_leds,CRGB::Red);
  FastLED.show();
  delay(1000);
  fill_solid(leds,Num_leds,CRGB::Green);
  FastLED.show();
  delay(1000);
  fill_solid(leds,Num_leds,CRGB::Blue);
  FastLED.show();
  delay(1000);
  fill_solid(leds,Num_leds,CRGB::Black);
  FastLED.show();


  senTimeout.setTime(senTime);
  gameTimeout.setTime(gameTime);
  gameTimeout.stop();
  checkSensor.setTime(300);
}

bool gameOn;
byte lastSensor, currentSensor;


void reset()
{
  gameOn =0;
  fill_solid(leds,Num_leds,CRGB::Black);
  FastLED.show();
}

void win()
{
winTimer.setTime(8000);
fill_solid(leds,Num_leds,CRGB::Black);
FastLED.show();

byte int1 =0, int2=Num_leds, hue=0;

do
{
    if(!ledTimeout.running())
  {
    int1++;
    int2--;
    hue+=5;
    ledTimeout.setTime(50);
    if(int1 >= Num_leds)
    {
      int1 = 0;
      int2 = Num_leds;
      hue = 0;
    }
  }
  leds[int1] = CHSV(hue,255,255);
  leds[int2] = CHSV(hue,255,255);
  leds[int1-1].fadeToBlackBy(2);
  leds[int2+1].fadeToBlackBy(2);
  fadeToBlackBy(leds,Num_leds,2);
  FastLED.show();

} while (winTimer.running());

reset();


}

int ledRain = 0;
bool rainDir = 1;
byte c1;
void attractor()
{
  if(!ledTimeout.running())
  {
    if(!rainDir)
    {ledRain--; c1=255;}
    else{ledRain++; c1=0;}

    if(ledRain==Num_leds || ledRain ==0){rainDir=!rainDir;}
    ledTimeout.setTime(75);
  }
  fill_rainbow_circular(leds,ledRain,c1);
  FastLED.show();

}

void updateProgressbar(byte Section, bool on)
{


  switch (Section)
  {
    case 0:
      if(on)
      {
        fill_solid(startSection, numPerSec, CHSV(0,255,255));  
        FastLED.show();
      }else
      {
        fill_solid(startSection, numPerSec, CRGB::Black); 
        FastLED.show();
      }
    break;

    case 1:
      if(on)
      {
        fill_solid(texture1, numPerSec, CHSV(31,255,255));
        FastLED.show();        
      }else
      {
        fill_solid(texture1, numPerSec, CRGB::Black);
        FastLED.show(); 
      }

    break;

    case 2:
      if(on)
      {
        fill_solid(texture2, numPerSec,CHSV(62,255,255)); 
        FastLED.show();                
      }else
      {
        fill_solid(texture2, numPerSec, CRGB::Black);
        FastLED.show(); 
      }

    break;

    case 3:
      if(on)
      {
        fill_solid(texture3, numPerSec, CHSV(93,255,255));
        FastLED.show();        
      }else
      {
        fill_solid(texture3, numPerSec, CRGB::Black);
        FastLED.show(); 
      }

    break;

    case 4:
      if(on)
      {
        fill_solid(texture4, numPerSec, CHSV(124,255,255));
        FastLED.show();        
      }else
      {
        fill_solid(texture4, numPerSec, CRGB::Black);
        FastLED.show(); 
      }

    break;

    case 5:
      if(on)
      {
        fill_solid(texture5, numPerSec, CHSV(155,255,255));
        FastLED.show();        
      }else
      {
        fill_solid(texture5, numPerSec, CRGB::Black);
        FastLED.show(); 
      }

    break;

    case 6:
      if(on)
      {
        fill_solid(texture6, numPerSec,CHSV(186,255,255));
        FastLED.show();        
      }else
      {
        fill_solid(texture6, numPerSec, CRGB::Black);
        FastLED.show(); 
      }

    break;

    case 7:
      if(on)
      {
        fill_solid(texture6, numPerSec,CHSV(217,255,255));
        FastLED.show();
        win();        
      }else
      {
        fill_solid(exitSection, numPerSec, CRGB::Black);
        FastLED.show(); 
      }


    break;


  }

}

bool detect1,detect2 ,detect3 ,detect4 ,detect5,detect6,exitDetect,easterEgg;  

void loop() 
{

  if(!beat.running()){heartbeat();beat.setTime(300);}
   if(!gameOn && !senTimeout.running())
  {
    bool startDetect = digitalRead(irStartPin);
    
    if(!startDetect)
    {
      gameOn = !gameOn;
      Serial.println("start");
      currentSensor = 0;
      secOn =1;
      senTimeout.restart();
      gameTimeout.restart();
    }
    
  }

  if(!gameOn){attractor();}

  if(gameOn)
  {
    if(!checkSensor.running())
    {
      senTriggered = false;
      detect1 = digitalRead(ir1Pin);
      detect2 = digitalRead(ir2Pin);
      detect3 = digitalRead(ir3Pin);
      detect4 = digitalRead(ir4Pin);
      detect5 = digitalRead(ir5Pin);
      detect6 = digitalRead(ir6Pin);
      exitDetect = digitalRead(irExitPin);
      easterEgg = digitalRead(ir7Pin);
    }


    if(!detect1 && !senTimeout.running())
    {
      lastSensor = currentSensor;
      Serial.println("Sensor 1 triggered!");
      senTimeout.restart();
      senTriggered = true;
      currentSensor = 1;
       gameTimeout.restart();
      
    }

    if(!detect2 && !senTimeout.running())
    {
      lastSensor = currentSensor;
      Serial.println("Sensor 2 triggered!");
      senTimeout.restart();
      senTriggered = true;
      currentSensor = 2;
       gameTimeout.restart();
    }

    if(!detect3 && !senTimeout.running())
    {
      lastSensor = currentSensor;
      Serial.println("Sensor 3 triggered!");
      senTimeout.restart();
      senTriggered = true;
      currentSensor = 3;
       gameTimeout.restart();
    }

    if(!detect4 && !senTimeout.running())
    {
      lastSensor = currentSensor;
      Serial.println("Sensor 4 triggered!");
      senTimeout.restart();
      senTriggered = true;
      currentSensor = 4;
       gameTimeout.restart();
    }

    if(!detect5 && !senTimeout.running())
    {
      lastSensor = currentSensor;
      Serial.println("Sensor 5 triggered!");
      senTimeout.restart();
      senTriggered = true;
      currentSensor = 5;
       gameTimeout.restart();
    }

    if(!detect6 && !senTimeout.running())
    {
      lastSensor = currentSensor;
      Serial.println("Sensor 6 triggered!");
      senTimeout.restart();
      senTriggered = true;
      currentSensor = 6;
       gameTimeout.restart();
    }

    if(!exitDetect && !senTimeout.running() && lastSensor == 6)
    {
      lastSensor = currentSensor;
      Serial.println("Exit triggered!");
      senTimeout.restart();
      senTriggered = true;
      currentSensor = 7;
       gameTimeout.restart();
    }

    if(!easterEgg && !senTimeout.running() && !eggTimer.running())
    {
      //playsound on wavtrigger 1
      wavTrig.trackPlaySolo(1);
      eggTimer.restart();
      gameTimeout.restart();
    }
    //Check the direction and update lights
    if(lastSensor >= currentSensor && lastSensor != currentSensor && senTriggered)
    {
      secOn = 0;
      updateProgressbar(currentSensor+1, secOn);
      Serial.println("Moved Back");
    }

    if(lastSensor <= currentSensor && lastSensor != currentSensor && senTriggered)
    {
      secOn = 1;
      Serial.println("Moved Forward");
    }

        if(senTriggered)
    {
        Serial.print("Current Sensor: ");
        Serial.println(currentSensor);
        Serial.print("last Sensor: ");
        Serial.println(lastSensor);
        Serial.print("Turn on section: ");
        Serial.println(secOn);  
    }
    
    updateProgressbar(currentSensor, secOn);
   
  }

  if(!gameTimeout.running()){reset();}
}