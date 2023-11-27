#include <Keypad.h>
#include "Keyboard.h"
#include <ClickEncoder.h>
#include <TimerOne.h>

const byte ROWS = 4; 
const byte COLS = 3; 

char hexaKeys[ROWS][COLS] = {
  {'0','1','2'},
  {'4','5','6'},
  {'8','9','A'},
  {'C','D','E'}
};

byte rowPins[ROWS] = {15,14,16,10}; 
byte colPins[COLS] = {18,19,20}; 

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
  char customKey = customKeypad.getKey();
  valuel += encoderl->getValue();
  valuer += encoderr->getValue();
  
  switch(customKey){
    case 'C':
      Keyboard.write(KEY_F13);
      break;
    case '8':
      Keyboard.write(KEY_F14);
      break;
    case '4':
      Keyboard.write(KEY_F15);
      break;
    case '0':
      Keyboard.write(KEY_F16);
      break;
    case 'D':
      Keyboard.write(KEY_F17);
      break;
    case '9':
      Keyboard.write(KEY_F18);
      break;
    case '5':
      Keyboard.write(KEY_F19);
      break;
    case '1':
      Keyboard.write(KEY_F20);
      break;
    case 'E':
      Keyboard.write(KEY_F21);
      break;
    case 'A':
      Keyboard.write(KEY_F22);
      break;
    case '6':
      Keyboard.write(KEY_F23);
      break;
    case '2':
      Keyboard.write(KEY_F24);
      break;
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
