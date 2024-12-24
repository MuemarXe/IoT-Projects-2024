#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance


#define TEMP_WIRE_BUS 2
#define ldrPin A0
#define IRpin 3
#define red 5
#define yellow 6
#define green 7
#define buzz 4
#define waterPin A1
float Irms=0;
int CT_pin = A2; //  CT sensor pin connected to A5 pin of Arduino

int readValue;  
int maxValue = 0;
int minValue = 1024;
int voltage = 230;
float Vpp;
float Vrms;
float power;
int waterVal;

LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire oneWire(TEMP_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);  //object for the temp sensor

float temp;

void setup()
{
  pinMode(buzz,OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(waterPin, INPUT);
  pinMode(ldrPin, INPUT);
  Serial.begin(9600);
  DS18B20.begin();
  emon1.current(CT_pin,10);
  lcd.init();
  lcd.backlight(); 
}
void printTemp(){
  lcd.clear();
   lcd.setCursor(0,0);
  lcd.print("T=");
  lcd.setCursor(2,0);
  lcd.print(temp); 
  }

void loop()

{
 //lcd.clear();
 double Irms = emon1.calcIrms(1480); 
 waterVal=analogRead(waterPin);
 int ldrValue = analogRead(ldrPin);
 Serial.print("ldrValue ");
 Serial.println(ldrValue);
 DS18B20.requestTemperatures(); 
 temp = DS18B20.getTempCByIndex(0); // Celcius
 Serial.println(temp);
 //printTemp();
   if (waterVal>300 && ldrValue>800 && temp<30 && Irms<2.0) //
   {
   lcd.clear();
   lcd.setCursor(0,0);
  lcd.print("T=");
  lcd.setCursor(2,0);
  lcd.print(temp); 
  lcd.print("C");
    lcd.setCursor(9,0);
    lcd.print("I=");
    lcd.print(Irms);
    lcd.print("A");
    lcd.setCursor(0,1);
    lcd.print("Normal Operation");
    digitalWrite(red,LOW);
    digitalWrite(green,HIGH);
    digitalWrite(yellow,LOW);
    } 
   if(waterVal<300 && ldrValue<800 && temp>30 && Irms>2.0)
   {
    lcd.clear();
    digitalWrite(green,LOW);
    digitalWrite(yellow,LOW);
    digitalWrite(red,HIGH);
    lcd.setCursor(0,0);
    lcd.print("EMERGENCY!!");
    digitalWrite(buzz,HIGH);
    delay(2000);
    lcd.setCursor(0,1);
    lcd.print("T=");
    lcd.print(temp);
    } 
     if(waterVal<300){
      lcd.clear();
        lcd.setCursor(0,1);
    lcd.print("check leakage");
    digitalWrite(yellow,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,LOW);
    Serial.print("water: ");
    Serial.println(waterVal);
    buzzer();
      }
      if(Irms>2.0){
        lcd.clear();
        lcd.setCursor(0,0);
    lcd.print("Overcurrent!");
    digitalWrite(yellow,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,LOW);
    lcd.setCursor(0,1);
    lcd.print("I=");
    lcd.print(Irms);
    lcd.print("A");
    Serial.print("I=");
    Serial.println(Irms);
    buzzer(); 
        
        }
   if(temp>30 || ldrValue<800 )
   {
     printTemp();
    if(temp>30){
      lcd.clear();
       lcd.setCursor(0,0);
    lcd.print("overheating");
    lcd.setCursor(0,1);
    lcd.print("temp=");
    lcd.print(temp);
     digitalWrite(yellow,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,LOW);
    buzzer();
      }
      
if(ldrValue<800)
  { 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("misalignment");
     digitalWrite(yellow,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,LOW);
    buzzer();
     if(waterVal<300){
        lcd.setCursor(0,1);
    lcd.print("water leakage");
    digitalWrite(yellow,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,LOW);
      }
if(waterVal>300)
     {
    lcd.setCursor(0,1);
    lcd.print("no leakage");
     }
  } 
    }
    
}
void buzzer(){
  digitalWrite(buzz,HIGH);
    delay(500);
    digitalWrite(buzz,LOW);
    delay(2000);
  }

void blinkYellow(){
     digitalWrite(yellow,HIGH);
     delay(500);
     digitalWrite(yellow,LOW);
     delay(500);
  }
void blinkRed(){
     digitalWrite(red,HIGH);
     delay(500);
     digitalWrite(red,LOW);
     delay(500);
  }
