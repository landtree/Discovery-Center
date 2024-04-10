#include <define.h>




void setup() 

{   Serial.begin(9600);
  //create led strip
  FastLED.addLeds<NEOPIXEL, leds_Pin>(tasteRings, totalLeds);
  //restricts brightness
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 2300);

  pinMode(ir1Pin, INPUT);
  pinMode(ir2Pin, INPUT);
  pinMode(ir3Pin, INPUT);
  pinMode(ir4Pin, INPUT);
  pinMode(ir5Pin, INPUT);

  //Strip test
  fill_solid(tasteRings,totalLeds,CRGB::Red);
  FastLED.show();
  delay(1000);
  fill_solid(tasteRings,totalLeds,CRGB::Blue);
  FastLED.show();
  delay(1000);
  fill_solid(tasteRings,totalLeds,CRGB::Green);
  FastLED.show();
  delay(1000);  
  fill_solid(tasteRings,totalLeds,CRGB::Black);
  FastLED.show();

}

bool ir1,ir2,ir3,ir4,ir5;
byte ledPos =0;

void readSesnor()
{
  ir1 = !digitalRead(ir1Pin);
  ir2 = !digitalRead(ir2Pin);
  ir3 = !digitalRead(ir3Pin);
  ir4 = !digitalRead(ir4Pin);
  ir5 = !digitalRead(ir5Pin);

  // Serial.println("----------------------");
  // Serial.print("Ir1: ");
  // Serial.println(ir1);
  // Serial.print("Ir2: ");
  // Serial.println(ir2);
  // Serial.print("Ir3: ");
  // Serial.println(ir3);
  // Serial.print("Ir4: ");
  // Serial.println(ir4);
  // Serial.print("Ir5: ");
  // Serial.println(ir5);
  // Serial.println("**********************");

  

}

//sweet, savory, spicy, sour, salty
void loop()
{

  readSesnor();

  //if any of the triggers hit, turn on the light based
  //one the proper color

  if(ir1)
  {
    //RGB
    fill_solid(sweet,numPerRing,CRGB(color1));
    winTrigTimer.setTime(2000);
  }else
  {
    if(!fade1Timer.running())
    {
    fadeToBlackBy(sweet,numPerRing,50);
    fade1Timer.setTime(fade);
    }
  }
  
    if(ir2)
  { 

    fill_solid(savory,numPerRing,CRGB(color2));
    winTrigTimer.setTime(2000);
  }else
  {
    if(!fade2Timer.running())
    {
    fadeToBlackBy(savory,numPerRing,50);
    fade2Timer.setTime(fade);
    }
  }

    if(ir3)
  {
    fill_solid(spicy,numPerRing,CRGB(color3));
    winTrigTimer.setTime(2000);
  }else
  {
    if(!fade3Timer.running())
    {
    fadeToBlackBy(spicy,numPerRing,50);
    fade3Timer.setTime(fade);

    }
  }


    if(ir4)
  {
    //fill_solid(sour,numPerRing,CHSV(163, 35, 58));
    fill_solid(sour,numPerRing,CRGB(color4));
    winTrigTimer.setTime(2000);
  }else
  {
    if(!fade4Timer.running())
    {
    fadeToBlackBy(sour,numPerRing,50);
    fade4Timer.setTime(fade);
    }
  }



  if(ir5)
  {
    fill_solid(salty,numPerRing,CRGB(color5));
    winTrigTimer.setTime(2000);
  }else
  {
    if(!fade5Timer.running())
    {
    fadeToBlackBy(salty,numPerRing,50);
    fade5Timer.setTime(fade);
    }
  }



if(ir1 && ir2 && ir3 && ir4 && ir5 && winTrigTimer.running())
  { 

    winTimer.setTime(5000);
   do
   {

      //small little win sequence
      if(!ledDelay.running())
      {
        ledPos++;
        if(ledPos > numPerRing){ledPos = 0;}
        //sweet, savory, spicy, sour, salty
        //set colors and pos
        sweet[ledPos] = CRGB(color1);
        savory[ledPos] = CRGB(color2);
        spicy[ledPos] = CRGB(color3);
        sour[ledPos] = CRGB(color4);
        salty[ledPos] = CRGB(color5);
        //update lights
    
        //fade to create trail
        fadeToBlackBy(sweet,numPerRing,50);
        fadeToBlackBy(savory,numPerRing,50);
        fadeToBlackBy(spicy,numPerRing,50);
        fadeToBlackBy(sour,numPerRing,50);
        fadeToBlackBy(salty,numPerRing,50);
        //update to have fade
    
        //restart timer
        ledDelay.setTime(75);
        FastLED.show();
      }

   } while (winTimer.running());

  }

  FastLED.show();

}
