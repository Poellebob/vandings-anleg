#include <Arduino.h>

const int motor = 1;
const int sensor = 'A0';

const int thresh= 500;
const int scantime_s = 3600;
const int waterring_time_s = 10;

void setup() {
  pinMode(motor, OUTPUT);
  
}

void loop() {
  int sensorValue = analogRead(sensor);

  if (sensorValue < thresh) {
    digitalWrite(motor, HIGH);
    delay(waterring_time_s * 1000);
    digitalWrite(motor, LOW);
  }
  delay(scantime_s * 1000);
}