//include libraries
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

//defining constants
#define second_t         16000
#define thresh_s         10
#define watering_time_s  10
#define beep_time_s      1

#define water_sensor     4
#define buzzer           3
#define motor            12
#define sensorPin1       A0
#define sensorPin2       A1
#define sensorPin3       A2
#define sensorPin4       A3

//defining lcd
LiquidCrystal_I2C lcd(0x27, 20, 4);

//defining functions
void lcdPrint(String Write); 
void lcdPrint(String Write, int col, int row); 
int nummap(float x, float in_min, float in_max, float out_min, float out_max); 
int findIndexOfLargest(const int arr[]);

//defining variables
int tiksSinceLastScan = 0; 
int sensor[4] = {0, 0, 0, 0}; 
int largestSensorIndex = 0;
int beep_time = 0;
bool beep = false;

// 2D array to store the LCD screen content
byte onScreen[4][20] = {
  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
};
int onScreenPos[2] = {0, 0};

void setup() {
  // initialize the LCD
  lcd.init(); 
  lcd.backlight(); 
  pinMode(motor, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(sensorPin3, INPUT);
  pinMode(sensorPin4, INPUT);  
  Serial.begin(9600);

  // Read the sensor values and store them in the sensor array
  sensor[0] = analogRead(sensorPin1);
  sensor[1] = analogRead(sensorPin2);
  sensor[2] = analogRead(sensorPin3);
  sensor[3] = analogRead(sensorPin4);
}

void loop() { 
  //analogWrite(3, 128);
  // Print the humidity value on the LCD screen 
  lcdPrint("fugtighed: " + String(map(analogRead(largestSensorIndex),1023,0,0,100)),0,1);

  Serial.println(
    "\nsensor1:" + String(analogRead(sensorPin1)) + "       " + String(map(analogRead(sensorPin1),1023,0,0,100)) +
    "\nsensor2:" + String(analogRead(sensorPin2)) + "       " + String(map(analogRead(sensorPin2),1023,0,0,100)) +
    "\nsensor3:" + String(analogRead(sensorPin3)) + "       " + String(map(analogRead(sensorPin3),1023,0,0,100)) +
    "\nsensor4:" + String(analogRead(sensorPin4)) + "       " + String(map(analogRead(sensorPin4),1023,0,0,100))
  );

  if(digitalRead(water_sensor) == LOW && beep){
    analogWrite(buzzer, 128);
    beep_time++;
    if(beep_time == beep_time_s * second_t){
      beep = false;
    }
  }else if(!beep){
    analogWrite(buzzer, 0);
    beep_time--;
    if(beep_time == 0){
      beep = true;
    }
  }

  // Check if the humidity is less than 50%
  if (map(analogRead(largestSensorIndex),1023,0,0,100) < 50) {
    // Turn on the motor
    digitalWrite(motor, HIGH);
    // Print the watering time on the LCD screen
    lcdPrint("vander: " + String(watering_time_s - tiksSinceLastScan/second_t),1,2);
    // Turn off the motor after the watering time
    digitalWrite(motor, LOW);
  }

  // Check if the time since last scan is greater than the threshold
  if(thresh_s * second_t <= tiksSinceLastScan){ 
    // Read the sensor values and store them in the sensor array
    sensor[0] = analogRead(sensorPin1);
    sensor[1] = analogRead(sensorPin2);
    sensor[2] = analogRead(sensorPin3);
    sensor[3] = analogRead(sensorPin4);

    // Check which sensor has the largest value
    largestSensorIndex = findIndexOfLargest(sensor);
  }

  tiksSinceLastScan++;
}

// Function to find the index of the largest value in an array
int findIndexOfLargest(const int arr[]) {
  int maxIndex = 0; // Assume the first element is the largest

  for (int i = 1; i < sizeof(arr)/sizeof(arr[0]); ++i) {
    if (arr[i] > arr[maxIndex]) {
      maxIndex = i;
    }
  }

  return maxIndex;
}

// Function to map a value from one range to another
int nummap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Function to print text on the LCD screen
void lcdPrint(String Write) {
  byte newLcd[4][20]{
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
  };

  //convert the Write string to a character array
  char writearray[Write.length() + 1];
  strcpy(writearray, Write.c_str());

  // Update the newLcd array with the characters from the writearray at the specified position
  for ( int i = 0; i < strlen(writearray); i++) {
    newLcd[onScreenPos[0]][i + onScreenPos[1]] = byte(writearray[i]);
  }

  // Update the LCD screen with the newLcd content
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 20; j++) {
      if(newLcd[i][j] != onScreen[i][j]) {
        lcd.setCursor(j, i); 
        lcd.write(newLcd[i][j]);
      }
    }
  }
  
  // Copy the content of newLcd to onScreen
  memcpy(onScreen, newLcd, sizeof(newLcd));
}

// Function to print text on the LCD screen at specific position
void lcdPrint(String Write, int col, int row) {
  byte newLcd[4][20]{
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
  };

  // Update the onScreenPos array with the new position
  onScreenPos[0] = col; 
  onScreenPos[1] = row; 

  //convert the Write string to a character array
  char writearray[Write.length() + 1];
  strcpy(writearray, Write.c_str());

  // Update the newLcd array with the characters from the writearray at the specified position
  for ( int i = 0; i < strlen(writearray); i++) {
    newLcd[onScreenPos[0]][i + onScreenPos[1]] = byte(writearray[i]);
  }

  // Update the LCD screen with the newLcd content
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 20; j++) {
      if(newLcd[i][j] != onScreen[i][j]) {
        lcd.setCursor(j, i); 
        lcd.write(newLcd[i][j]);
      }
    }
  }
  
  // Copy the content of newLcd to onScreen
  memcpy(onScreen, newLcd, sizeof(newLcd));
}