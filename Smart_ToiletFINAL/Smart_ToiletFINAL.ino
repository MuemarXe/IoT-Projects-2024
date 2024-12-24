
#include <Wire.h> 
// Include LCD display library for I2C
#include <LiquidCrystal_I2C.h>
// Include Keypad library
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

// Create LCD object
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define Door_IR_Pin 12      // IR sensor pin for door opening
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
  PERSON_DETECTED2,
  OPENING_DOOR
};

ToiletState currentState = WAITING_FOR_PERSON;

void setup() {
  Serial.begin(9600);
  myservo.attach(11); 
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
  myservo.write(100);  

}

void loop() {
  switch (currentState) {
    case WAITING_FOR_PERSON:
     myservo.write(100); 
    digitalWrite(NOT_OCCUPIED, HIGH);
     lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Waiting 4 Person");
      
      if (digitalRead(Person_IR_Pin) == LOW) {
        delay(1000);
        myservo.write(170);  
        delay(1000);
        currentState = PERSON_DETECTED;
      }
      break;

    case PERSON_DETECTED:

      if (digitalRead(Door_IR_Pin) == LOW) {
        lcd.clear();
    lcd.setCursor(0, 0);
      lcd.print("Toilet  ");
      lcd.setCursor(0, 1);
      lcd.print("Occupied      ");
      digitalWrite(NOT_OCCUPIED, LOW);
          digitalWrite(OCCUPIED, HIGH);
           delay(1000);
        myservo.write(100);  
        delay(1000);
         delay(5000);

        currentState = HANDS_NOT_WASHED;
      }
      break;

    case HANDS_NOT_WASHED:
     digitalWrite(NOT_OCCUPIED, LOW);
    digitalWrite(OCCUPIED, HIGH);
       lcd.clear();
       lcd.setCursor(0, 0);
      lcd.print("Kindly Wash");
      lcd.setCursor(0, 1);
      lcd.print("Hands when done");

      if (digitalRead(Moisture_Sensor_Pin) == LOW) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Good Hygiene");
      lcd.setCursor(0, 1);
      lcd.print("Keep Up");
      delay(5000);
        currentState = PERSON_DETECTED2;
      }
      break;
          case PERSON_DETECTED2:
          
      if (digitalRead(Door_IR_Pin) == LOW) {
         digitalWrite(NOT_OCCUPIED, HIGH);
          digitalWrite(OCCUPIED, LOW);
         delay(3000);

        currentState = OPENING_DOOR;
      }
      break;

    case OPENING_DOOR:
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Opening Door  ");
       delay(1000);
        myservo.write(170);  
        delay(1000);
      // Code to trigger the door to open using IR
      // Add your code here to control the door mechanism
      
      delay(6000); // Wait for the door to open
      currentState = WAITING_FOR_PERSON;
      break;
  }

  delay(500); // Delay for stability
}

