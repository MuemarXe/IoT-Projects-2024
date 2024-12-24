
#include "DHT.h"

#define DHTPIN 3
#define DHTTYPE DHT11   // DHT 11

#define sensorPin A0
#define BuzzPin 6
const int SOUND_THRESHOLD = 500;

 const int OUT_PIN = 8;
 const int SAMPLE_TIME = 1000;
 unsigned long millisCurrent;
 unsigned long millisLast = 0;
 unsigned long millisElapsed = 0;
 int sampleBufferValue = 0;

DHT dht(DHTPIN, DHTTYPE);

float tempVal;
float humVal;
#define fan A5
#define motor A4

// DHT11 Settings ============== end ==============


float  getTemperature(){
    
// Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return 0.00;
  }  
  return t;
}


float  getHumidity(){
    
// Read temperature as Celsius (the default)
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return 0.00;
  }
  return h;
}
  

void setup()
{
  // Debug console
  Serial.begin(9600);
  dht.begin();
  pinMode(OUT_PIN, OUTPUT);
   pinMode(motor, OUTPUT);
   pinMode(fan, OUTPUT);
  pinMode(BuzzPin, OUTPUT);
//  digitalWrite(BuzzPin, LOW);

}

void loop(){

  // int soundValue = digitalRead(OUT_PIN);  // Read the analog value from the sound sensor

  // // Check if sound value exceeds the threshold
  // if (soundValue == LOW) {
  //   // If sound detected, turn on motor through relay
  //   analogWrite(motor, 0);
  //   Serial.println("Motor turned ON");
  // } else {
  //   // If no sound detected, turn off motor
  //   analogWrite(motor, 0);
  //   Serial.println("Motor turned OFF");
  // }

  tempVal = getTemperature();// or dht.readTemperature(true) for Fahrenheit
  Serial.print("Temperature: ");
  Serial.println(tempVal);
  
  if( tempVal >=28){
    analogWrite(fan,0);
    }
    else  {
     analogWrite(fan,255);
      }
   
  humVal = getHumidity(); // or dht.readTemperature(true) for Fahrenheit
  Serial.print(" Humidity: ");
  Serial.println(humVal);
  if( humVal >=40){
    digitalWrite(BuzzPin,LOW);
    }
    else  {
      digitalWrite(BuzzPin,LOW);
      }
      
  Serial.print("Analog output: ");

  Serial.println(readSensor());

  delay(500);

}
int readSensor() {

  int sensorValue = analogRead(sensorPin);  // Read the analog value from sensor

  int outputValue = map(sensorValue, 0, 1023, 255, 0); // map the 10-bit data to 8-bit data

  analogWrite(BuzzPin, outputValue); // generate PWM signal

  return outputValue;             // Return analog rain value

}
