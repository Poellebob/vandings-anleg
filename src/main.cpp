#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const int motor = 13;
const int sensor = A0;

const int thresh = 500;
const int scan_time_s = 3600;
const int watering_time_s = 10;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello, world!"); 
  pinMode(motor, OUTPUT);
}

void loop() {
  //int sensorValue = analogRead(sensor);
  
  //bool run_motor = (sensorValue < thresh);
  
  //if (run_motor)digitalWrite(motor, HIGH);
  //else digitalWrite(motor, LOW);
  
  //delay(scan_time_s * 1000);


}