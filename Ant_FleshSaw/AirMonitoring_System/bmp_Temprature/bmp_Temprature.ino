#include <SFE_BMP180.h>
#include <Wire.h>
int pin =9;

SFE_BMP180 pressure;
void setup(){
  pinMode(pin,OUTPUT);
  Serial.begin(9600);
  Serial.println("BMP180 Temperature Measurements");

  if (pressure.begin())
    Serial.println("BMP180 init success");
  else {
    Serial.println("BMP180 init fail\n\n");
    while (1); // Pause forever.
  }
}
void loop() {
  char status;
  double Temprature;

  // Start a temperature measurement
  status = pressure.startTemperature();

  if (status != 0) {
    // Wait for the measurement to complete
    delay(status);

    // Retrieve the completed temperature measurement
    status = pressure.getTemperature(Temprature);

    if (status != 0) {
      // Print out the measurement
      Serial.print("Temperature: ");
      Serial.print(Temprature, 2);
      Serial.print(" deg C, ");
      Serial.print((9.0 / 5.0) * Temprature + 32.0, 2);
      Serial.println(" deg F");
    } else {
      Serial.println("Error retrieving temperature measurement");
    }
  } else {
    Serial.println("Error starting temperature measurement");
  }

  delay(5000); // Pause for 5 seconds.
  if (Temprature> 27){
    digitalWrite(pin,HIGH);
    Serial.println("LightOn");
    
    }
    else{
      digitalWrite(pin,LOW);
      Serial.println("LightOff");
      }
    
}
