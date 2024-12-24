#include <Wire.h> 
// Include LCD display library for I2C
#include <LiquidCrystal_I2C.h>
// Include Keypad library

// Create LCD object
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define Door_IR_Pin 12       // IR sensor pin for door opening
#define Person_IR_Pin 10      // IR sensor pin for presence detection
#define Moisture_Sensor_Pin 9 // Moisture sensor pin
#define OCCUPIED 4   
#define NOT_OCCUPIED  6  
#define Gas  A0

enum ToiletState {
  WAITING_FOR_PERSON,
  PERSON_DETECTED,
  HANDS_NOT_WASHED,
  HANDS_WASHED,
  OPENING_DOOR
};

ToiletState currentState = WAITING_FOR_PERSON;

void setup() {
  Serial.begin(9600);

  pinMode(Door_IR_Pin, INPUT);
  pinMode(Person_IR_Pin, INPUT);
    pinMode(NOT_OCCUPIED, OUTPUT);
  pinMode(OCCUPIED, OUTPUT);
  pinMode(Moisture_Sensor_Pin, INPUT);
  
  lcd.backlight();
  lcd.init(); 
  lcd.begin(16, 2);
  lcd.setCursor(0, 0); 
  lcd.print("LIMURU GIRLS");
  

}

void loop() {
  switch (currentState) {
    case WAITING_FOR_PERSON:
      lcd.setCursor(0, 1);
      digitalWrite(NOT_OCCUPIED ,HIGH);
      lcd.print("Waiting       ");
      
      if (digitalRead(Person_IR_Pin) == LOW) {
          lcd.setCursor(0, 1);
      lcd.print("Welcome   ");
        currentState = PERSON_DETECTED;
      }
      break;

    case PERSON_DETECTED:
      if (digitalRead(Door_IR_Pin) == LOW) {
         lcd.setCursor(0, 1);
         lcd.print("Occupied      ");
         digitalWrite(OCCUPIED ,HIGH);
         
        currentState = HANDS_NOT_WASHED;
      }
      break;

    case HANDS_NOT_WASHED:
      lcd.setCursor(0, 1);
      lcd.print("Wash Hands!   ");

      if (digitalRead(Moisture_Sensor_Pin) == LOW) {
        currentState = OPENING_DOOR;
      }
      break;

    case OPENING_DOOR:
      lcd.setCursor(0, 1);
      lcd.print("Opening Door  ");
      // Code to trigger the door to open using IR
      // Add your code here to control the door mechanism
      
      delay(3000); // Wait for the door to open
      currentState = WAITING_FOR_PERSON;
      break;
  }

  delay(500); // Delay for stability
}

bool checkHandsWashed() {
  // Read moisture sensor to check if hands are washed
  int moistureLevel = analogRead(Moisture_Sensor_Pin);
  // Assuming a threshold value to determine if hands are washed
  return moistureLevel > 500; // Adjust threshold value as needed
}
  
