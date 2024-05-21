#include <define.h>

int encodedScent = 000;
int encodedImg = 000;

int readScentIR(int encoded)
{ 
  //reads the bool of each encode ring
  bool ir1 = !digitalRead(encode1); 
  bool ir2 = !digitalRead(encode2);
  bool ir3 = !digitalRead(encode3);

  if(ir1 || ir2 || ir3)
  {
    fanTimer.setTime(60000);
  }
  //write the bits to update encoded signal
  bitWrite(encodedScent,0,ir1);
  bitWrite(encodedScent,1,ir2);
  bitWrite(encodedScent,2,ir3);
  return encoded;
}

int readImgIR(int encoded)
{
  //reads the bool of each encode ring
  bool ir4 = !digitalRead(encode4); 
  bool ir5 = !digitalRead(encode5);
  bool ir6 = !digitalRead(encode6);

    if(ir4 || ir5 || ir6)
  {
    fanTimer.setTime(60000);
  }

  //write the bits to update encoded signal
  bitWrite(encodedScent,0,0);
  bitWrite(encodedScent,1,0);
  bitWrite(encodedScent,2,0);
  return encoded;
}


void setup() {
  Serial.begin(9600);

  //setup neopixel ring
  FastLED.addLeds<NEOPIXEL, leds_Pin>(ring, numLeds).setCorrection( TypicalSMD5050 );
  //restricts brightness
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 2300);

  //setup button
  startBtn.attach(startBtnPin, INPUT_PULLUP);
  startBtn.setPressedState(LOW);

  //setup pinmodes
  pinMode(encode1, INPUT);
  pinMode(encode2, INPUT);
  pinMode(encode3, INPUT);
  pinMode(encode4, INPUT);
  pinMode(encode5, INPUT);
  pinMode(encode6, INPUT);
  pinMode(fan, OUTPUT);

  fill_solid(ring,numLeds,CRGB::Red);
  FastLED.show();
  delay(1000);
  fill_solid(ring,numLeds,CRGB::Green);
  FastLED.show();
  delay(1000);
  fill_solid(ring,numLeds,CRGB::Blue);
  FastLED.show();
  delay(1000);
  fill_solid(ring,numLeds,CRGB::Black);
  FastLED.show();

}

void printdebug()
{

  Serial.print("Fan Status: ");
  if(fanOn == 1){Serial.println("Fan On");}else{Serial.println("Fan Off");}
  delay(300);

  Serial.println("**********************");
  encodedScent = readImgIR(encodedScent);
  Serial.print("Scent Encoder: ");
  Serial.println(encodedScent);
  delay(300);
  Serial.println("**********************");
  encodedImg = readScentIR(encodedImg);
  Serial.print("Image Encoder: ");
  Serial.println(encodedImg);
  delay(300);
  Serial.println("**********************");

  Serial.println("Button Pressed: ");
  if(startBtn.isPressed()){Serial.println("Pressed");}else{Serial.println("Not Pressed");}


}


void loop() 
{

  #ifdef debug
    printdebug();
  #endif
  startBtn.update();

  if(startBtn.pressed() && !buttonTimer.running())
  {
    //read the line breaks for the smells & encode
    encodedScent = readScentIR(encodedScent);
    //read the line breaks for the smells & encode
    encodedImg = readImgIR(encodedImg);
    //turn on fan and button timers
    fanTimer.setTime(60000);
    buttonTimer.setTime(400);
  }

  //run the fan for 1sec and then turn off
  if(!fanTimer.running())
  {
    fanOn = !fanOn;
    digitalWrite(fan,fanOn);
  }

  if(encodedScent == encodedImg)
  {
    fill_solid(ring,numLeds,CRGB::Green);
  }else
  {
    fill_solid(ring,numLeds,CRGB::Red);
  }

  FastLED.show();

}




