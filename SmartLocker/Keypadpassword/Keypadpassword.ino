#include <Wire.h>
// Include LCD display library for I2C
#include <LiquidCrystal_I2C.h>
// Include Keypad library
#include <Keypad.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

 
// Length of password + 1 for null character
#define Password_Length 4
// Character to hold password input
char Data[Password_Length];
// Password
char Master[Password_Length] = "147";
 
// Pin connected to lock relay input
//int lockOutput = 13;
 
// Counter for character entries
byte data_count = 0;
 
// Character to hold key input
char customKey;
 
// Constants for row and column sizes
const byte ROWS = 4;
const byte COLS = 4;
 
// Array to represent keys on keypad
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
 
// Connections to Arduino
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 11};
 
// Create keypad object
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
 
// Create LCD object
LiquidCrystal_I2C lcd(0x27, 20, 4);
 enum LockerState {
  SCAN_RFID,
  COMPARE_UID,
  CHECK_PAYMENT_INFO,
};
LockerState currentState = SCAN_RFID;
void setup() {
  // Setup LCD with backlight and initialize
  lcd.backlight();
  lcd.init();
  myservo.attach(12); 
  lcd.begin(20, 4);
  // Set lockOutput as an OUTPUT pin
  //pinMode(lockOutput, OUTPUT);
}

 void clearData() {
  // Go through array and clear data
  while (data_count != 0) {
    Data[data_count--] = 0;
  }
  return;
}
void loop() {
 
  // Initialize LCD and print
  lcd.setCursor(0, 0);
  lcd.print("Enter Password:");
 
  // Look for keypress
  customKey = customKeypad.getKey();
  if (customKey) {
    // Enter keypress into array and increment counter
    Data[data_count] = customKey;
    lcd.setCursor(data_count, 1);
    lcd.print(Data[data_count]);
    data_count++;
  }
 
  // See if we have reached the password length
  if (data_count == Password_Length - 1) {
    lcd.clear();
 
    if (!strcmp(Data, Master)) {
      // Password is correct
      lcd.print("Correct");
      // Turn on relay for 5 seconds
      for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
     myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
     }
    }
    else {
      // Password is incorrect
      lcd.print("Incorrect");
      delay(1000);
    }
 
    // Clear data and LCD display
    lcd.clear();
    clearData();
  }
}


 
