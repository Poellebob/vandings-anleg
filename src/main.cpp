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
  
  bool run_motor = (sensorValue < thresh);
  
  if (run_motor)digitalWrite(motor, HIGH);
  else digitalWrite(motor, LOW);
  
  //delay(scan_time_s * 1000);
  lcd.setCursor(1, 0);
  lcd.print("Soil Moisture: ");
  lcd.setCursor(1, 1);
  lcd.print(sensorValue);
}