//include libraries
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

//defining constants
#define second_t 16000000
#define second 1000

#define motor 13
#define sensorPin A0

#define thresh  500
#define scan_time_s 3600
#define watering_time_s 10

const byte costumchar[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

//defining lcd
LiquidCrystal_I2C lcd(0x27, 20, 4);

//defining functions
void water();
void lcdPrint(String Write);
void lcdPrint(String Write, int col, int row);
int nummap(float x, float in_min, float in_max, float out_min, float out_max);

//defining variables
int tiksSinceLastScan = 0;

byte newScreen[4][20] = {
  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
};
byte onScreen[4][20] = {
  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
};
int onScreenPos[2] = {0, 0};

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(motor, OUTPUT);
  Serial.begin(9600);
}

void loop() { 

  lcdPrint("fugtighed: " + String( map(analogRead(sensorPin),1023,0,0,100)),1,2);
  Serial.println(analogRead(sensorPin));
  
}

int nummap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void water() {
  int sensorValue = analogRead(sensorPin);
  
  if (sensorValue < thresh)digitalWrite(motor, HIGH);
  else digitalWrite(motor, LOW);
}

void lcdPrint(String Write) {
  byte newLcd[4][20];
  memcpy(newLcd, newScreen, sizeof(newLcd));

  char writearray[Write.length() + 1];
  strcpy(writearray, Write.c_str());

  for ( int i = 0; i < strlen(writearray); i++) {
    newLcd[onScreenPos[0]][i + onScreenPos[1]] = byte(writearray[i]);
  }

  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 20; j++) {
      if(newLcd[i][j] != onScreen[i][j]) {
        lcd.setCursor(j, i);
        lcd.write(newLcd[i][j]);
      }
    }
  }

  memcpy(onScreen, newLcd, sizeof(newLcd));
}

void lcdPrint(String Write, int col, int row) {
  byte newLcd[4][20];
  memcpy(newLcd, newScreen, sizeof(newLcd));

  onScreenPos[0] = col;
  onScreenPos[1] = row;

  char writearray[Write.length() + 1];
  strcpy(writearray, Write.c_str());

  for ( int i = 0; i < strlen(writearray); i++) {
    newLcd[onScreenPos[0]][i + onScreenPos[1]] = byte(writearray[i]);
  }

  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 20; j++) {
      if(newLcd[i][j] != onScreen[i][j]) {
        lcd.setCursor(j, i);
        lcd.write(newLcd[i][j]);
      }
    }
  }
  
  memcpy(onScreen, newLcd, sizeof(newLcd));
}