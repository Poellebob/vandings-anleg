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

//defining custom character for LCD
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
void water(); // function to control watering based on sensor reading
void lcdPrint(String Write); // function to print text on LCD
void lcdPrint(String Write, int col, int row); // function to print text on LCD at specific position
int nummap(float x, float in_min, float in_max, float out_min, float out_max); // function to map a value from one range to another

//defining variables
int tiksSinceLastScan = 0; // variable to keep track of time since last scan

// 2D arrays to store the LCD screen content
byte newScreen[4][20];
byte onScreen[4][20];
int onScreenPos[2] = {0, 0}; // array to store the current position of the curser on the LCD screen

void setup() {
  lcd.init(); // initialize the LCD
  lcd.backlight(); // turn on the backlight
  pinMode(motor, OUTPUT); // set the motor pin as output
  Serial.begin(9600); // initialize the serial communication

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 20; j++)
    {
      newScreen[i][j] = ' '; // initialize the newScreen array with empty characters
    }
  }

  memcpy(onScreen, newScreen, sizeof(newScreen)); // Copy the content of newScreen to onScreen
}

void loop() { 
  // Print the humidity value on the LCD screen 
  lcdPrint("fugtighed: " + String( map(analogRead(sensorPin),1023,0,0,100)),1,2);
  Serial.println(analogRead(sensorPin)); // Print the sensor value to the serial monitor
}

// Function to map a value from one range to another
int nummap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Function to control watering based on sensor reading
void water() {
  int sensorValue = analogRead(sensorPin);
  
  if (sensorValue < thresh) // if the sensor value is below the threshold
    digitalWrite(motor, HIGH); // turn on the motor
  else
    digitalWrite(motor, LOW); // turn off the motor
}

// Function to print text on the LCD screen

// Function to print text on the LCD screen
void lcdPrint(String Write) {
  byte newLcd[4][20];
  memcpy(newLcd, newScreen, sizeof(newLcd)); // Copy the content of newScreen to newLcd

  char writearray[Write.length() + 1];
  strcpy(writearray, Write.c_str()); // Convert the string to a character array

  // Update the newLcd array with the characters from the writearray
  for ( int i = 0; i < strlen(writearray); i++) {
    newLcd[onScreenPos[0]][i + onScreenPos[1]] = byte(writearray[i]);
  }

  // Update the LCD screen with the newLcd content
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 20; j++) {
      if(newLcd[i][j] != onScreen[i][j]) {
        lcd.setCursor(j, i); // Set the cursor position on the LCD screen
        lcd.write(newLcd[i][j]); // Write the character to the LCD screen
      }
    }
  }

  memcpy(onScreen, newLcd, sizeof(newLcd)); // Copy the content of newLcd to onScreen
}

// Function to print text on the LCD screen at specific position
void lcdPrint(String Write, int col, int row) {
  byte newLcd[4][20];
  memcpy(newLcd, newScreen, sizeof(newLcd)); // Copy the content of newScreen to newLcd

  onScreenPos[0] = col; // Update the row position of the cursor
  onScreenPos[1] = row; // Update the column position of the cursor

  char writearray[Write.length() + 1];
  strcpy(writearray, Write.c_str()); // Convert the string to a character array

  // Update the newLcd array with the characters from the writearray at the specified position
  for ( int i = 0; i < strlen(writearray); i++) {
    newLcd[onScreenPos[0]][i + onScreenPos[1]] = byte(writearray[i]);
  }

  // Update the LCD screen with the newLcd content
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 20; j++) {
      if(newLcd[i][j] != onScreen[i][j]) {
        lcd.setCursor(j, i); // Set the cursor position on the LCD screen
        lcd.write(newLcd[i][j]); // Write the character to the LCD screen
      }
    }
  }
  
  memcpy(onScreen, newLcd, sizeof(newLcd)); // Copy the content of newLcd to onScreen
}

