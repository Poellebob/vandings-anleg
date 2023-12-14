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

//defining variables
int tiksSinceLastScan = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  pinMode(motor, OUTPUT);
}

void loop() {
  if(tiksSinceLastScan > scan_time_s * second_t) water();

  lcd.clear();
  lcd.setCursor(1, 0);

  lcd.print("fugtighed: " + analogRead(sensorPin));
  
  tiksSinceLastScan++;
}

void water() {
  int sensorValue = analogRead(sensorPin);
  
  if (sensorValue < thresh)digitalWrite(motor, HIGH);
  else digitalWrite(motor, LOW);
}