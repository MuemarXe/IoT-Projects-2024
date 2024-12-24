   // Define pin numbers for the buttons and relay control pins
// Include Arduino Wire library for I2C

#include <Wire.h> 
// Include LCD display library for I2C
#include <LiquidCrystal_I2C.h>
// Include Keypad library

// Create LCD object
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buttonPin1 = 2; // Button for coffee combination 1
const int buttonPin2 = 6; // Button for coffee combination 2
const int buttonPin3 = 7; // Button for coffee combination 3
const int relayPin1 = A0;  // Relay control pin for pump 1
const int relayPin2 = A1;  // Relay control pin for pump 2
const int relayPin3 = A2;  // Relay control pin for pump 3

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set button pins as inputs
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);

  // Set relay control pins as outputs
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(relayPin3, OUTPUT);
  analogWrite(relayPin1, 255);
  analogWrite(relayPin2, 255);
  analogWrite(relayPin3, 255);
  
  lcd.backlight();
  lcd.init(); 
  lcd.begin(16, 2);
  lcd.setCursor(0, 0); 
  lcd.print("PICK COMBINATION");
}

void loop() {
  // Check if button for coffee combination 1 is pressed
  if (digitalRead(buttonPin1) == HIGH) {
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("Pre-WorkOut Gain");
    lcd.setCursor(3, 1); 
    lcd.print("      " );
    Serial.println("Gain ");
    // Activate pump 1
    analogWrite(relayPin1, 0);
    // Add any additional delay if needed for dispensing coffee
    delay(4000); // Example delay for 5 seconds
    // Turn off pump 1
    analogWrite(relayPin1, 255);
    delay(500);
    analogWrite(relayPin2, 0);
    // Add any additional delay if needed for dispensing coffee
    delay(3500); // Example delay for 5 seconds
    // Turn off pump 1
    analogWrite(relayPin2, 255);
    
    waiting();
  }

  // Check if button for coffee combination 2 is pressed
  if (digitalRead(buttonPin2) ==HIGH) {
     lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("PostWorkOut Gain");
    lcd.setCursor(3, 1); 
    lcd.print("      " );
    Serial.println("Making Coffee Combination 2");
    // Activate pump 2
    analogWrite(relayPin3, 0);
    // Add any additional delay if needed for dispensing coffee
    delay(4000); // Example delay for 5 seconds
    // Turn off pump 2
    analogWrite(relayPin3, 255);
        delay(500);
    analogWrite(relayPin1, 0);
    // Add any additional delay if needed for dispensing coffee
    delay(3500); // Example delay for 5 seconds
    // Turn off pump 1
    analogWrite(relayPin1, 255);
    
    waiting();
  }

  // Check if button for coffee combination 3 is pressed
  if (digitalRead(buttonPin3) == HIGH) {
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("PostWorkOut Loss");
    lcd.setCursor(0, 1); 
    lcd.print("    ");
    
    Serial.println("Making Coffee Combination 3");
    // Activate pump 3
    analogWrite(relayPin3, 0);
    // Add any additional delay if needed for dispensing coffee
    delay(4000); // Example delay for 5 seconds
    // Turn off pump 3
    analogWrite(relayPin3, 255);
        delay(500);
    analogWrite(relayPin2, 0);
    // Add any additional delay if needed for dispensing coffee
    delay(3500); // Example delay for 5 seconds
    // Turn off pump 1
    analogWrite(relayPin2, 255);
    waiting();  
  }

  // Add a small delay to debounce buttons and reduce loop frequency
  delay(1000);
  
}

void waiting(){
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("PICK COMBINATION");
  
  }
