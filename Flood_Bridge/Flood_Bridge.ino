#define BLYNK_TEMPLATE_ID "TMPL2G1-Upgb5"
#define BLYNK_TEMPLATE_NAME "Flood Monitoring"
#define BLYNK_AUTH_TOKEN "7Pj_XYPCFpnhMWd6xV2bnO7dthvgicJy"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <BlynkSimpleEsp8266.h> // Include Blynk library for ESP8266
char ssid[] = "Novatech";
char pass[] = "Nova@Tech";

const int trigPin = D3; // Ultrasonic sensor trigger pin
const int echoPin = D8; // Ultrasonic sensor echo pin
const int servoPin = D7; // Servo control pin
const int redLightPin = D5; // Red traffic light pin
const int greenLightPin = D6; // Green traffic light pin
int vibrationValue = analogRead(vibrationPin);
int waterLevel;
int waterVolume;
int waterLevelPercentage;
bool gateOpen = false;
int waterLevelHigh = 10;
int waterLevelLow = 3;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Address for 16x2 LCD
const int vibrationPin = A0;

Servo servo;

BlynkTimer timer;
void setup() {
  Serial.begin(9600);
  lcd.init();                      // Initialize the LCD
  lcd.backlight();
   lcd.setCursor(0, 0); 
  lcd.print("Bridge Safety");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLightPin, OUTPUT);
  pinMode(greenLightPin, OUTPUT);
  pinMode(vibrationPin, INPUT);
  // pinMode(buzzerPin, OUTPUT);
  servo.attach(servoPin);
   Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
   timer.setInterval(1000L, sendSensorData); // Send sensor data every second
}

void loop(){
  Blynk.run(); // Run Blynk communication
  timer.run(); // Run BlynkTimer
}
void sendSensorData() {
  // Send ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  waterLevel = pulseIn(echoPin, HIGH) / 58.0;

  // Map distance to water level percentage
    //  waterLevel = map(waterLevel,0, 30, 100, 0);
    // waterVolume = map(waterVolume, 0, 100);

    waterLevel = map(waterLevel,12,0,0,12);
    waterVolume = map(waterLevel,12,0,100,0);
  // Display water level percentage on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Water Level:");
  lcd.setCursor(0, 1);
  lcd.print(waterVolume);
  lcd.print("%");


  // Control servo and traffic light based on water level
  if (waterLevel >=10) {
      servo.write(170);
      gateOpen = false;
         // Open the bridge
  digitalWrite(redLightPin, HIGH); // Turn off red light
  digitalWrite(greenLightPin, LOW); // Turn on green light
  } else {
    servo.write(10);  
    gateOpen = true;   // Open the bridge
  digitalWrite(redLightPin, LOW); // Turn off red light
  digitalWrite(greenLightPin, HIGH); // Turn on green light
  }


   Serial.println("vibrationValue");
  Serial.println(vibrationValue);

    Blynk.virtualWrite(V0, waterVolume);
    Blynk.virtualWrite(V1, gateOpen);
    Blynk.virtualWrite(V2, vibrationValue);
  //delay(1000); // Adjust delay as needed
}
