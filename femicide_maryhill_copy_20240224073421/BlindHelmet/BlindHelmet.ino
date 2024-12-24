#define D0 16
#define D1 5 // I2C Bus SCL (clock)
#define D2 4 // I2C Bus SDA (data)
#define D3 0
#define D4 2 // Same as "LED_BUILTIN", but inverted logic
#define D5 14 // SPI Bus SCK (clock)
#define D6 12 // SPI Bus MISO 
#define D7 13 // SPI Bus MOSI
#define D8 15 // SPI Bus SS (CS)
#define D9 3 // RX0 (Serial console)
#define D10 1 // TX0 (Serial console)
#define S3 10
#define S2 9

#define BLYNK_TEMPLATE_ID "TMPL2UMBycI3A"
#define BLYNK_TEMPLATE_NAME "BlindHelmet"
#define BLYNK_AUTH_TOKEN "hlCI64A4u_De9GAron_Gt0dP0J2dqyiJ"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <Wire.h> 
#include <OneWire.h>
#include <DallasTemperature.h>
#define temprature D0


OneWire oneWire( temprature);
DallasTemperature DS18B20(&oneWire);  // Object for the temperature sensor

char ssid[] = "kirwara";
char pass[] = "kirwara123";
BlynkTimer timer; 
float temp;

const int trigPin = D7; 
const int echoPin = D8; 
const int trigPin2 = D1; 
const int echoPin2 = D2; 
#define rightEar D6
#define leftEar D5

#define buttonPin D3
 int buttonState = digitalRead(buttonPin);
long duration;
int distance;
long duration2;
int distance2;
void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
pinMode(rightEar,OUTPUT);
pinMode(leftEar,OUTPUT);
pinMode(temprature,INPUT);
pinMode(buttonPin,INPUT_PULLUP);
Serial.begin(9600); // Starts the serial communication
DS18B20.begin();
Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
 timer.setInterval(1000L, calcDist);
}
void loop() {
  // int buttonState = digitalRead(buttonPin);
   Blynk.run();
  // runs BlynkTimer
   timer.run(); 

calcDist();
if(distance2<20){
  analogWrite(leftEar,100);
//  delay(1000);
  digitalWrite(rightEar,HIGH);
  
  }else{
    digitalWrite(rightEar,LOW);
    }
if(distance<20){
  analogWrite(leftEar,150);
//  delay(1000);
//  digitalWrite(leftEar,LOW);
  
  }else{
    digitalWrite(leftEar,LOW);
    }
  DS18B20.requestTemperatures(); 
  temp = DS18B20.getTempCByIndex(0); // Celsius
  Serial.print("Temperature: ");
  Serial.print(temp);
 
   Blynk.virtualWrite(V0, 35);
  
}
void calcDist(){
  // Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
//Serial.print("Distance: ");
//Serial.println(distance);
//delay(2000);

digitalWrite(trigPin2, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin2, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin2, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration2 = pulseIn(echoPin2, HIGH);

// Calculating the distance
distance2= duration2*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance2: ");
Serial.println(distance2);
Serial.print("Distance: ");
Serial.println(distance);
 Serial.print("buttonState");
  Serial.println(buttonState);
  Blynk.virtualWrite(V1, buttonState );
//delay(2000);

  }