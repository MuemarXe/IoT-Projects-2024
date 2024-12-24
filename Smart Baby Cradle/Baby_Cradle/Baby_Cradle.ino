
#include "DHT.h"

#define DHTPIN 3
#define DHTTYPE DHT11   // DHT 11

#define sensorPin A0
#define BuzzPin 6

 const int microphonePin = A1;
const int threshold =900;

DHT dht(DHTPIN, DHTTYPE);
float outputValue;
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
   pinMode(motor, OUTPUT);
   pinMode(fan, OUTPUT);
  pinMode(BuzzPin, OUTPUT);
//  digitalWrite(BuzzPin, LOW);

}

void loop(){
  int mn = 1024;
  int mx = 0;

  for (int i = 0; i < 10000; ++i) {

    int val = analogRead(microphonePin);
    
    mn = min(mn, val);
    mx = max(mx, val);
  }

  int delta = mx - mn;

  Serial.print("Min=");
  Serial.print(mn);
  Serial.print(" Max=");
  Serial.print(mx);
  Serial.print(" Delta=");
  Serial.println(delta);

if (mx > 900){
  analogWrite(motor,0);

}else{
  analogWrite(motor,255);
}
   
  tempVal = getTemperature();// or dht.readTemperature(true) for Fahrenheit
  Serial.print("Temperature: ");
  Serial.println(tempVal);
  
  if( tempVal >=23){
    analogWrite(fan,0);
    }
    else  {
     analogWrite(fan,255);
      }
  humVal = getHumidity(); // or dht.readTemperature(true) for Fahrenheit
  Serial.print(" Humidity: ");
  Serial.println(humVal);

  Serial.print("Analog output: ");

  Serial.println(readSensor());

  delay(500);
 
}
int readSensor() {
  int sensorValue = analogRead(sensorPin);  // Read the analog value from the rain sensor
    Serial.println("sensorValue");
   Serial.println(sensorValue);
  // Assuming a threshold of 500, adjust as necessary
  if (sensorValue <= 700) {
    digitalWrite(BuzzPin, HIGH);  // Turn the buzzer on if sensor value is above the threshold
  } else {
    digitalWrite(BuzzPin, LOW);   // Turn the buzzer off if sensor value is below the threshold
  }

  return sensorValue;             // Return analog rain value
}
