#define BLYNK_TEMPLATE_ID "TMPL2oF-_cjNk"
#define BLYNK_TEMPLATE_NAME "SmartBeehive"
#define BLYNK_AUTH_TOKEN "RSZ2ugH9t_VjMI733_gfCoKUylk4vlMX"
#define BLYNK_PRINT Serial

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h> // Include Blynk library for ESP8266

#define Level_One D6
#define Level_Two D3
#define Level_Three D5

char ssid[] = "Novatech";
char pass[] = "Nova@Tech";
char auth[] = "YourAuthToken"; // Your Blynk auth token

LiquidCrystal_I2C lcd(0x27, 16, 2);
int LEDPin1 = D8;
int LEDPin2 = D7;
int LEDPin3 = D4;

int previousLevel = -1; // Initialize previous level to an invalid value

void setup() {
  Serial.begin(9600);
  pinMode(LEDPin1, OUTPUT);
  pinMode(LEDPin2, OUTPUT);
  pinMode(LEDPin3, OUTPUT);
  
  pinMode(Level_One, INPUT);
  pinMode(Level_Two, INPUT);
  pinMode(Level_Three, INPUT);
  
  lcd.backlight();
  lcd.init(); 
  lcd.begin(16, 2);
  lcd.setCursor(0, 0); 
  lcd.print("WATER LEVEL");
  
   Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run(); // Run Blynk communication
//   timer.run(); 
  
  int currentLevel = -1; // Initialize current level to an invalid value
  
  if (digitalRead(Level_One) == HIGH) {
    digitalWrite(LEDPin1,HIGH);
    currentLevel = 25;
  }

  if (digitalRead(Level_Two) == HIGH) {
     digitalWrite(LEDPin2,HIGH);
    currentLevel = 50;
  }

  if (digitalRead(Level_Three) == HIGH) {
     digitalWrite(LEDPin3,HIGH);
    currentLevel = 75;
  } 

  if (digitalRead(Level_One) == HIGH && digitalRead(Level_Three) == HIGH && digitalRead(Level_Two) == HIGH) {
     digitalWrite(LEDPin1,LOW);
     digitalWrite(LEDPin2,LOW);
     digitalWrite(LEDPin3,LOW);
    currentLevel = 0;
  }
  
  // Check if the water level has changed
  if (currentLevel != previousLevel) {
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("WATER LEVEL");
    lcd.setCursor(4, 1); 
    lcd.print(currentLevel);
    Blynk.virtualWrite(V0, currentLevel); // Send data to Blynk virtual pin V0
    previousLevel = currentLevel; // Update previous level
  }
}
