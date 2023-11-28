#include <Arduino.h>

const int motor = 13;
const int sensor = A0;

const int thresh = 500;
const int scan_time_s = 3600;
const int watering_time_s = 10;

void setup() {
  pinMode(motor, OUTPUT);
}

void loop() {
  int sensorValue = analogRead(sensor);

  if (sensorValue < thresh) {
    digitalWrite(motor, HIGH);
    delay(watering_time_s * 1000);
    digitalWrite(motor, LOW);
  }
  delay(scan_time_s * 1000);
}