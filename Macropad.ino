#include <Keypad.h>
#include "Keyboard.h"
#include <ClickEncoder.h>
#include <TimerOne.h>

#define KEY_F13   0xF0
#define KEY_F14   0xF1
#define KEY_F15   0xF2
#define KEY_F16   0xF3
#define KEY_F17   0xF4
#define KEY_F18   0xF5
#define KEY_F19   0xF6
#define KEY_F20   0xF7
#define KEY_F21   0xF8
#define KEY_F22   0xF9
#define KEY_F23   0xFA
#define KEY_F24   0xFB

const byte ROWS = 3; 
const byte COLS = 4; 

int hexaKeys[ROWS][COLS] = {
  {KEY_F13,KEY_F14,KEY_F15,KEY_F16},
  {KEY_F17,KEY_F18,KEY_F19,KEY_F20},
  {KEY_F21,KEY_F22,KEY_F23,KEY_F24}
};

byte rowPins[ROWS] = {18,19,20}; 
byte colPins[COLS] = {15,14,16,10}; 

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

ClickEncoder *encoderl;
ClickEncoder *encoderr;

int16_t lastl, lastr, valuel, valuer;

void timerIsr() {
  encoderl->service();
  encoderr->service();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Keyboard.begin();

  encoderl = new ClickEncoder(4, 5, 3);
  encoderr = new ClickEncoder(6, 7, 2);
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 
  
  lastl = -1;
  lastr = -1;
}

void loop() {
  // put your main code here, to run repeatedly:
  int customKey = customKeypad.getKey();
  valuel += encoderl->getValue();
  valuer += encoderr->getValue();
  
  if (customKey){
    Keyboard.press(customKey);
    delay(10);
  }

  if (valuel != lastl) {
    Keyboard.press(0x84);
    if(lastl<valuel){
      Keyboard.press(0xF1);
    }else{
      Keyboard.press(0xF2);
    }
    lastl = valuel;
  }
  if(valuer != lastr){
    Keyboard.press(0x84);
    if(lastr<valuer){
      Keyboard.press(0xF3);
    }else{
      Keyboard.press(0xF4);
    }
    lastr = valuer;
  }

  ClickEncoder::Button l = encoderl->getButton();  
  ClickEncoder::Button r = encoderr->getButton();
  if(l != ClickEncoder::Open){
    if(l == ClickEncoder::Clicked){
      Keyboard.press(0x84);
      Keyboard.press(0xF6);
    }
  }

  if(r != ClickEncoder::Open){
    if(r == ClickEncoder::Clicked){
      Keyboard.press(0x84);
      Keyboard.press(0xF7);
    }
  }
  Keyboard.releaseAll();

  delay(10);
}
