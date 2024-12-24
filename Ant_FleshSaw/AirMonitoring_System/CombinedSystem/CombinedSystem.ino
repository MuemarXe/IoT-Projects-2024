#include <SFE_BMP180.h>
#include <Wire.h>

SFE_BMP180 pressure;

#define ALTITUDE 1655.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters
#define LDR A0          //  LDR is connected to analog pin A1
#define Oxygen 7           //  digital pin 13 for the Oxygen control
#define Gas 10           // digital pin 13 for the Gas control
#define Temp 8           // digital pin 13 for the Temprature control
#define Light 9           // digital pin 13 for the light control
#define sensorAnalog A1   // MQ3 connected to A1
#define sensorAnalog2 A2  // MQ2 connected to A2

void setup() {
  Serial.begin(9600);
  Serial.println("BMP180 and LDR Measurements");

  if (pressure.begin())
    Serial.println("BMP180 init success");
  else {
    Serial.println("BMP180 init fail\n\n");
    while (1); // Pause forever.
  }

  pinMode(Light, OUTPUT);
  pinMode(Temp, OUTPUT);
  pinMode(Oxygen, OUTPUT);
  pinMode(Gas, OUTPUT);
}

void loop() {
  char status;
  double Temperature;
  
  // Start a temperature measurement
  status = pressure.startTemperature();

  if (status != 0) {
    // Wait for the measurement to complete
    delay(status);

    // Retrieve the completed temperature measurement
    status = pressure.getTemperature(Temperature);

    if (status != 0) {
      // Print out the measurement
      Serial.print("Temperature: ");
      Serial.print(Temperature, 2);
      Serial.print(" deg C, ");
      Serial.print((9.0 / 5.0) * Temperature + 32.0, 2);
      Serial.println(" deg F");

      // Control the light based on temperature
      if (Temperature > 28) {
        digitalWrite(Temp, HIGH);
        Serial.println("Light On Temprature");
      } else {
        digitalWrite(Temp, LOW);
        Serial.println("Light Off Temprature");
      }
    } else {
      Serial.println("Error retrieving temperature measurement");
    }
  } else {
    Serial.println("Error starting temperature measurement");
  }
  delay(150);

  // Read light intensity from LDR
  int light = analogRead(LDR);
  Serial.println("Light Intensity: " + String(light));
  delay(150);

  // Control the light based on light intensity
  if (light > 300) {
    digitalWrite(Light, HIGH);
    Serial.println("Light On LDR");
  } else {
    digitalWrite(Light, LOW);
    Serial.println("Light Off LDR");
  }

  delay(150);

int analog = analogRead(sensorAnalog);
int analog2 = analogRead(sensorAnalog2);
 
  Serial.print("Analog value : ");
  Serial.print(analog);
  Serial.print("t");
  delay(1000);

  Serial.print("          Analog value2 : ");
  Serial.print(analog2);
  Serial.print("t");
  Serial.println("");
  
  delay(700);
  
 if ( analog>200){
    digitalWrite(Oxygen,HIGH);
    Serial.println("LightOnOxygen");
    
    }
 else{
      digitalWrite(Oxygen,LOW);
      Serial.println("LightOffOxygen");
      }
      
 if ( analog2 >250){
    digitalWrite(Gas,HIGH);
    Serial.println("LightOnGas");
    
    }
 else{
      digitalWrite(Gas,LOW);
      Serial.println("LightOffGas");
      }
}
