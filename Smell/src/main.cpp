#include <define.h>

int readScentIR(int encodedS)
{ 
  //reads the bool of each encode ring
  bool ir1 = digitalRead(encode1); 
  bool ir2 = digitalRead(encode2);
  bool ir3 = digitalRead(encode3);

  //write the bits to update encoded signal
  bitWrite(encodedScent,0,ir1);
  bitWrite(encodedScent,1,ir2);
  bitWrite(encodedScent,2,ir3);

  encodedS = encodedScent;
  return encodedS;
}

int readImgIR(int encodedI)
{
  //reads the bool of each encode ring
  bool ir4 = digitalRead(encode4); 
  bool ir5 = digitalRead(encode5);
  bool ir6 = digitalRead(encode6);

  //write the bits to update encoded signal
  bitWrite(encodedImg,0,ir4);
  bitWrite(encodedImg,1,ir5);
  bitWrite(encodedImg,2,ir6);

  encodedI = encodedImg;
  return encodedI;
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

bool checkMatch(bool match)
{
  //Picture Wheel --> Smell Wheel
  // 1 --> 3
  // 2 --> 6
  // 3 --> 2
  // 4 --> 5 
  // 5 --> 1
  // 6 --> 7
  // 7 --> 4

  //assume match is false unless a below statement is true
  match = false;

  //true statements
  if(encodedImg == img1 && encodedScent == scent3){match = true;}
  if(encodedImg == img2 && encodedScent == scent6){match = true;}
  if(encodedImg == img3 && encodedScent == scent2){match = true;}
  if(encodedImg == img4 && encodedScent == scent5){match = true;}
  if(encodedImg == img5 && encodedScent == scent1){match = true;}
  if(encodedImg == img6 && encodedScent == scent7){match = true;}
  if(encodedImg == img7 && encodedScent == scent4){match = true;}

  Serial.print("match: ");
  Serial.println(match);
  
  return match;

}

void printdebug()
{
  Serial.println("**********************");
  encodedScent = readScentIR(encodedScent);
  Serial.print("Scent Encoder: ");
  Serial.println(encodedScent);
  Serial.println("**********************");

  encodedImg = readImgIR(encodedImg);
  Serial.print("Image Encoder: ");
  Serial.println(encodedImg);
  Serial.println("**********************");

  Serial.print("Button Pressed: ");
  if(startBtn.isPressed()){Serial.println("Pressed");}else{Serial.println("Not Pressed");}

}


void loop() 
{

  #ifdef debug
    if(!debugTimer.running())
    {
      printdebug();
      debugTimer.setTime(1000);
    }
  #endif

  startBtn.update();

  if(startBtn.pressed() && !buttonTimer.running())
  {
    //read the line breaks for the smells & encode
    encodedScent = readScentIR(encodedScent);
    //read the line breaks for the smells & encode
    encodedImg = readImgIR(encodedImg);
    //check to see if a match was made
    match = checkMatch(match);
    //allow button reset button timer
    buttonTimer.setTime(100);
  }


  if(match)
  {
    fill_solid(ring,numLeds,CRGB::Green);
  }else
  {
    fill_solid(ring,numLeds,CRGB::Red);
  }

  FastLED.show();

}




