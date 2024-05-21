#include <define.h>

void setup() 

{  
  //start serial
  Serial.begin(9600);

  //uncomment to force the uController to wait 
  //until serial connects, comment out to autostart
  //while(!Serial);

  //create led strip
  FastLED.addLeds<NEOPIXEL, leds_Pin>(tasteRings, totalLeds);
  //restricts brightness
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 2000);

  pinMode(ir1Pin, INPUT);
  pinMode(ir2Pin, INPUT);
  pinMode(ir3Pin, INPUT);
  pinMode(ir4Pin, INPUT);
  pinMode(ir5Pin, INPUT);
  pinMode(13,OUTPUT);

  //Strip test
  //Changes all LEDs r/g/b to test
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

void heartpulse()
{
  if(!heartbeat.running())
  {
    on = !on;
    digitalWrite(13,on);
    heartbeat.setTime(500);
    #ifdef debug
    Serial.print("LED:");
    Serial.println(on);
    #endif
  }
}

void readSesnor()
{
  //gate the sesnors, makes it easier to flip logic
  ir1 = digitalRead(ir1Pin);
  ir2 = digitalRead(ir2Pin);
  ir3 = digitalRead(ir3Pin);
  ir4 = digitalRead(ir4Pin);
  ir5 = digitalRead(ir5Pin);

  #ifdef debug
  Serial.println("----------------------");
  Serial.print("Ir1: ");
  Serial.println(ir1);
  Serial.print("Ir2: ");
  Serial.println(ir2);
  Serial.print("Ir3: ");
  Serial.println(ir3);
  Serial.print("Ir4: ");
  Serial.println(ir4);
  Serial.print("Ir5: ");
  Serial.println(ir5);
  Serial.println("----------------------");
  #endif
  
}

void setTriggers()
{
  //set the triggers true if it has been trigger once
  if(ir1)
  {
    ir1T = true;
  }
  if(ir2)
  {
    ir2T = true;
  }
  if(ir3)
  {
    ir3T = true;
  }
  if(ir4)
  {
    ir4T = true;
  }
  if(ir5)
  {
    ir5T = true;
  }
}

void reset()
{
  ir1T = false;
  ir2T = false;
  ir3T = false;
  ir4T = false;
  ir5T = false;
  resetTimer.setTime(60000);
}

//sweet, savory, spicy, sour, salty
void loop()
{
  //blink on board led, used as a quick check that it is looping
  heartpulse();
  //read & update all the sensors 
  readSesnor();
  //update perm trigger
  setTriggers();
  //check to see if the timeout triggerd
  if(!resetTimer.running())
  {
    reset();
  }
  //if any of the triggers hit, turn on the light based
  //on the proper color

  if(ir1)
  {
    //RGB
    fill_solid(sweet,numPerRing,CRGB(color1));
     resetTimer.setTime(60000);
  }  
  if(ir2)
  { 

    fill_solid(savory,numPerRing,CRGB(color2));
    resetTimer.setTime(60000);
  }
  if(ir3)
  {
    fill_solid(spicy,numPerRing,CRGB(color3));
    resetTimer.setTime(60000);

  }
  if(ir4)
  {
    fill_solid(sour,numPerRing,CRGB(color4));
    resetTimer.setTime(60000);
  }
   if(ir5)
  {
    fill_solid(salty,numPerRing,CRGB(color5));
    resetTimer.setTime(60000);
  }

if(ir1T && ir2T && ir3T && ir4T && ir5T)
  { 

    winTimer.setTime(5000);
    //set to black before starting effects
    fill_solid(tasteRings,totalLeds,CRGB::Black);
    FastLED.show();
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
    reset();
  }

  FastLED.show();

}
