#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int gasSensorPin = A0;  // Analog input pin for gas sensor
const int relayPin = 10;      // Digital output pin for relay module
const int greenLedPin = 7;    // Green LED connected to digital pin D7
const int redLedPin = 6;      // Red LED connected to digital pin D6

int threshold = 500;  // Set your threshold value here

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  pinMode(gasSensorPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  // Display startup message on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Kuwa Dere Smart");
  lcd.setCursor(0, 1);
  lcd.print("Drive safely");
  delay(1000);  // Display for 2 seconds
}

void loop() {
  int gasValue = analogRead(gasSensorPin);


  Serial.print("Gas Sensor Value: ");
  Serial.println(gasValue);  // prints the gas sensor value

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gas Sensor Value:");
  lcd.setCursor(0, 1);
  lcd.print(gasValue);
  delay(1000);

  if (gasValue < threshold) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Kuwa Dere Smart");
    lcd.setCursor(0, 1);
    lcd.print("Drive safely");
    delay(1000);
    digitalWrite(greenLedPin, HIGH);  // Green LED ON when below threshold
    digitalWrite(redLedPin, LOW);     // Red LED OFF
      digitalWrite(relayPin, LOW);     // Turn off the car engine
    
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alcohol Detected!!");
    lcd.setCursor(0, 1);
    lcd.print("Alco-stop is on");
    digitalWrite(greenLedPin, LOW);   // Green LED OFF
    digitalWrite(redLedPin, HIGH);    // Red LED ON when above threshold
    digitalWrite(relayPin, HIGH);     // Turn off the car engine
  }

  delay(1000);  // Clear screen after 2 seconds
  lcd.clear();
  //digitalWrite(relayPin, LOW);  // Ensure relay is off for the next loop iteration
  //delay(1000);
}