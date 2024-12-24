#define BLYNK_TEMPLATE_ID "TMPL2_PxiYPFq"
#define BLYNK_TEMPLATE_NAME "Flood  Monitoring krw"
#define BLYNK_AUTH_TOKEN "h7FjaEXr7VV9Gi4TaGMMV9QHLzyQ2XNl"
#define BLYNK_PRINT Serial
#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h> // Include Blynk library for ESP8266
//#include <BlynkTimer.h> // Include BlynkTimer library


//

//

const int trigger1 = D6;  // Trigger pin for first sensor
const int echo1 = D5;     // Echo pin for first sensor
const int trigger2 = D8;  // Trigger pin for second sensor
const int echo2 = 10;     // Echo pin for second sensor
   // Echo pin for third sensor

Servo gateServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);
char ssid[] = "kirwara";
char pass[] = "kirwara123";
int openButton = D7;
int currentOpenButton;
int lastPressOpenButton= 0;

int waterLevel;
int waterLevelLow = 3;
int waterLevelHigh = 10;
bool gateOpen = false;
int waterVolume;
int downstreamWaterLevel;

BlynkTimer timer;

void setup() {
  Serial.begin(9600);
  pinMode(trigger1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trigger2, OUTPUT);
  pinMode(echo2, INPUT);
  
  lcd.init();   // initializing the LCD
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Kirwara ");
  lcd.setCursor(2,1);
  lcd.print("Highschool");
  delay(3000);
  lcd.clear();
  lcd.print("Conneting....");
  
  gateServo.attach(D4); //D4
  gateServo.write(3); 
  pinMode(openButton,INPUT); 
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  timer.setInterval(1000L, sendSensorData); // Send sensor data every second
}
void loop(){
    Blynk.run(); // Run Blynk communication
    timer.run(); // Run BlynkTimer
}

void sendSensorData() {
  // Pass INC as a parameter to get the distance in inches

  // Measure distance from first sensor
  digitalWrite(trigger1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger1, LOW);
  waterLevel = pulseIn(echo1, HIGH) / 58.0;
  // waterLevel = ultrasonic.read();

  // Measure distance from second sensor
  digitalWrite(trigger2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger2, LOW);
  downstreamWaterLevel = pulseIn(echo2, HIGH) / 58.0;

 
  waterLevel = map(waterLevel,12,0,0,12);
  // downstreamWaterLevel = map(downstreamWaterLevel,12,0,0,12);
  currentOpenButton=digitalRead(openButton);
  waterVolume = map(waterLevel,12,0,100,0);
  lcd.setCursor(0,0);
  lcd.print("Level: ");
  lcd.print(waterLevel);
  lcd.print(",Gate");
  lcd.setCursor(0,1);
  lcd.print("Vol: ");
  lcd.print(waterVolume);
  lcd.print("%");
  if(gateOpen){
    lcd.print(", OPEN");
  }else{
    lcd.print(", CLOSED");
  }
  Serial.print("Water level: ");
  Serial.println(waterLevel);
  Serial.print("downstream WaterLevel: ");
  Serial.println(downstreamWaterLevel);
  Serial.print("Water volume: ");
  Serial.print(waterVolume);
  Serial.println("%");
  if(waterLevel>=waterLevelHigh){
  Serial.println("High water level ...");
    gateOpen = true;   
  }else if(waterLevel<=waterLevelLow){
    //close flood gates 
    Serial.println("Low water level ...");
    gateOpen = false; 
    gateServo.write(3);
  }
  
  if(currentOpenButton==HIGH && lastPressOpenButton ==0){
      Serial.println("Panic button!!!");
      gateOpen = true; 
      lastPressOpenButton = 1;
  }else if(currentOpenButton==LOW){
    Serial.println("NO Panic button!!!");
    lastPressOpenButton= 0;
  }

  if(gateOpen){
  //open flood gates  
    // gateServo.write(map((waterLevel),waterLevelLow,waterLevelHigh,180,0));     
    gateServo.write(160);    
  }
  
  Blynk.virtualWrite(V0, waterVolume);
  Blynk.virtualWrite(V2, gateOpen);
  // delay(500);
}