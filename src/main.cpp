/*
hvis der include error med LiquidCrystal så instaler LiquidCrystal biblioteket
med <pio pkg install --library "arduino-libraries/LiquidCrystal@^1.0.7">
i pio terminalen som du åbner hved at trykke på "new terminal" knappen i platformio
*/
#include <Arduino.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int motor = 13;
const int sensor = A0;

const int thresh = 500;
const int scan_time_s = 3600;
const int watering_time_s = 10;

void setup() {
  pinMode(motor, OUTPUT);
  lcd.begin(16, 2);
}

void loop() {
  int sensorValue = analogRead(sensor);
  
  if (sensorValue < thresh) { 
    digitalWrite(motor, HIGH);
    //delay(watering_time_s * 1000);
  }
  if (sensorValue > thresh){
    digitalWrite(motor, LOW);
  }
  
  lcd.setCursor(1, 0);
  lcd.print("Soil Moisture: ");
  lcd.setCursor(1, 1);
  lcd.print(sensorValue);
}