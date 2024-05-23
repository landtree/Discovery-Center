//include libraries
#include <Arduino.h>
#include <FastLED.h>
#include <Timer.h>

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
#define ir8Pin 10 //Not used currently

//Led defintion for the full strip and break to sections
#define leds_Pin 11
#define Num_leds 60
#define numPerSec round(Num_leds/6) //sets the strip into number of sensors and rounds to fix any decimals
CRGB leds[Num_leds];


//break into sections
CRGB* startSection = &leds[0]; 
CRGB* texture1 = &leds[(numPerSec * 1)]; 
CRGB* texture2 = &leds[(numPerSec * 2)]; 
CRGB* texture3 = &leds[(numPerSec * 3)]; 
CRGB* texture4 = &leds[(numPerSec * 4)]; 
CRGB* texture5 = &leds[(numPerSec * 5)]; 
CRGB* texture6 = &leds[(numPerSec * 6)]; 
CRGB* exitSection = &leds[(numPerSec * 6)];

//create timers
MoToTimer senTimeout,ledTimeout, gameTimeout,checkSensor,beat,winTimer;

//define time
#define gameTime 60000
#define senTime 75

//create flags
bool senTriggered,beaton,secOn;
bool gameOn;

bool startDetect, detect1,detect2 ,detect3 ,detect4 ,detect5,detect6,exitDetect,easterEgg;   

uint8_t lastSensor, currentSensor;
uint8_t int1 =0, int2=Num_leds, hue=0;

int ledRain = 0;
bool rainDir = 1;
uint8_t c1;

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
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 2000);

  Serial.begin(9600);
  //while(!Serial); turn of to wait to connect
  senTimeout.setTime(senTime);
  gameTimeout.setTime(gameTime);
  checkSensor.setTime(75);

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
}

void reset()
{
  Serial.println("reset");
  //turn off game and set LEDs black
  gameOn = 0; 
  fill_solid(leds,Num_leds,CRGB::Black);
  FastLED.show();
}

void win()
{
Serial.println("Win");
winTimer.setTime(8000);
fill_solid(leds,Num_leds,CRGB::Black);
FastLED.show();



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


void loop() 
{

  if(!beat.running()){heartbeat();beat.setTime(300);}

   if(!gameOn && !senTimeout.running())
  {
      startDetect = digitalRead(irStartPin);

      Serial.print("Sensors trigging game start:");
      Serial.print(" Start:");
      Serial.print(startDetect);
      // Serial.print("| 1:");
      // Serial.print(detect1);      
      // Serial.print("| 2:");
      // Serial.print(detect2);   
      // Serial.print("| 3:");
      // Serial.print(detect3);   
      // Serial.print("| 4:");
      // Serial.print(detect4);   
      // Serial.print("| 5:");
      // Serial.print(detect5);   
      // Serial.print("| Exit:");
      // Serial.print(exitDetect);   
      // Serial.print("| EE:");
      // Serial.println(easterEgg);   

    if(startDetect)
    {
      fill_solid(leds,Num_leds,CRGB::Black);
      gameOn = true;
      Serial.println("start");
      currentSensor = 0;
      secOn= 1;
      senTimeout.restart();
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
      exitDetect = digitalRead(irExitPin);
      easterEgg = digitalRead(ir7Pin);
    }


    if(detect1 && !senTimeout.running())
    {
      checkSensor.setTime(75);
      lastSensor = currentSensor;
      Serial.println("Sensor 1 triggered!");
      senTimeout.restart();
      senTriggered = true;
      currentSensor = 1;
      
    }

    if(detect2 && !senTimeout.running())
    {
      checkSensor.setTime(75);
      lastSensor = currentSensor;
      Serial.println("Sensor 2 triggered!");
      senTimeout.restart();
      senTriggered = true;
      currentSensor = 2;
      
    }

    if(detect3 && !senTimeout.running())
    {
      checkSensor.setTime(75);
      lastSensor = currentSensor;
      Serial.println("Sensor 3 triggered!");
      senTimeout.restart();
      senTriggered = true;
      currentSensor = 3;
    }

    if(detect4 && !senTimeout.running())
    {
      checkSensor.setTime(75);
      lastSensor = currentSensor;
      Serial.println("Sensor 4 triggered!");
      senTimeout.restart();
      senTriggered = true;
      currentSensor = 4;
    }

    if(detect5 && !senTimeout.running())
    {
      checkSensor.setTime(75);
      lastSensor = currentSensor;
      Serial.println("Sensor 5 triggered!");
      senTimeout.restart();
      senTriggered = true;
      currentSensor = 5;
    }

    if(exitDetect && !senTimeout.running() && lastSensor == 5)
    {
      checkSensor.setTime(75);
      lastSensor = currentSensor;
      Serial.println("Exit triggered!");
      senTimeout.restart();
      senTriggered = true;
      currentSensor = 7;
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

    if(senTriggered && !checkSensor.running())
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
}