
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
  
  if (digitalRead(Level_One) == HIGH) {
    digitalWrite(LEDPin1,HIGH);
    
    lcd.clear();
     lcd.setCursor(0, 0); 
     lcd.print("WATER LEVEL");
    lcd.setCursor(4, 1); 
    lcd.print("25%");
    Blynk.virtualWrite(V3, 25); // Send data to Blynk virtual pin V1
  }

  if (digitalRead(Level_Two) == HIGH) {
     digitalWrite(LEDPin2,HIGH);
    lcd.clear();
     lcd.setCursor(0, 0); 
    lcd.print("WATER LEVEL");
    lcd.setCursor(4, 1); 
    lcd.print("50%");
    Blynk.virtualWrite(V2, 50); // Send data to Blynk virtual pin V1
  }

  if (digitalRead(Level_Three) == HIGH) {
     digitalWrite(LEDPin3,HIGH);
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("WATER LEVEL");
    lcd.setCursor(4, 1); 
    lcd.print("75%");
    Blynk.virtualWrite(V1, 75); // Send data to Blynk virtual pin V1
  } 

  if (digitalRead(Level_One) == HIGH && digitalRead(Level_Three) == HIGH && digitalRead(Level_Two) == HIGH) {
     digitalWrite(LEDPin1,LOW);
     digitalWrite(LEDPin2,LOW);
     digitalWrite(LEDPin3,LOW);
    lcd.clear();
    lcd.setCursor(4, 1); 
    lcd.print("0%");
    Blynk.virtualWrite(V0, 0); // Send data to Blynk virtual pin V1
  }
}
