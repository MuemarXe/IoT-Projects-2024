#include <Arduino.h>

#include <DHT.h>
#include <Adafruit_Sensor.h>

#define DHTPIN 2
#define DHTTYPE DHT11

#define LDR_PIN A0
#define LIMIT_SWITCH_SHADE 3
#define LIMIT_SWITCH_OUTSIDE 4
#define MOTOR_INA 6
#define MOTOR_INB 5
#define RAIN_SENSOR 7

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);


  pinMode(LDR_PIN, INPUT);
  pinMode(LIMIT_SWITCH_SHADE, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH_OUTSIDE, INPUT_PULLUP);
  pinMode(MOTOR_INA, OUTPUT);
  pinMode(MOTOR_INB, OUTPUT);
  pinMode(RAIN_SENSOR, INPUT_PULLUP);
  dht.begin();

  // Set initial motor direction
  digitalWrite(MOTOR_INA, LOW);
  digitalWrite(MOTOR_INB, LOW);
  Serial.println("Initializing system,system should be static");
  //for(;;){}
}

void loop() {
  float humidity = dht.readHumidity();delay(2500);
  Serial.print("Humidity is: ");Serial.print(humidity);Serial.println();
  int ldrValue = analogRead(LDR_PIN);
  Serial.print("LDR valu is: ");Serial.print(ldrValue);Serial.println();
  int rainSensorValue = !digitalRead(RAIN_SENSOR);
  Serial.print("Is there rain? ");Serial.print(rainSensorValue);Serial.println();

  if (ldrValue < 500 && humidity > 70) {
    // Dark and high humidity condition
    Serial.println("it's dark and humid,clothes should be in");
    while (digitalRead(LIMIT_SWITCH_SHADE)) {
      // Turn motor counter-clockwise
      digitalWrite(MOTOR_INA, HIGH);
      digitalWrite(MOTOR_INB, LOW);
      Serial.println("bringing in clothes");
      //Serial.println("motor moving clockwise");
    }
    // Stop the motor
    digitalWrite(MOTOR_INA, LOW);
    digitalWrite(MOTOR_INB, LOW);
  } else if (ldrValue < 500 && !rainSensorValue) {
    // Dark and not raining condition
    Serial.println("dark and not raining,clothes should be out");
    while (digitalRead(LIMIT_SWITCH_OUTSIDE)) {
      // Turn motor clockwise
      digitalWrite(MOTOR_INA, LOW);
      digitalWrite(MOTOR_INB, HIGH);
       Serial.println("putting out clothes....");
    }
    // Stop the motor
    digitalWrite(MOTOR_INA, LOW);
    digitalWrite(MOTOR_INB, LOW);
  } else if (rainSensorValue) {
    // Rain detected
    Serial.println("it's raining,clothes should be in");
    while (digitalRead(LIMIT_SWITCH_SHADE)) {
      // Turn motor counter-clockwise to retract
      digitalWrite(MOTOR_INA, HIGH);
      digitalWrite(MOTOR_INB, LOW);
      Serial.println("bringing in clothes");
      
    }
    Serial.println("exiting wet loop");
    // Stop the motor
    digitalWrite(MOTOR_INA, LOW);
    digitalWrite(MOTOR_INB, LOW);
  } else {
    Serial.println("Its sunny,clothes should be out");
    // Default condition, no specific criteria met
    // Stop the motor
    // digitalWrite(MOTOR_INA, LOW);
    // digitalWrite(MOTOR_INB, LOW);
    // Serial.println("no criteria met,,system is static");
      while (digitalRead(LIMIT_SWITCH_OUTSIDE)) {
        // Turn motor clockwise to retract
        digitalWrite(MOTOR_INA, LOW);
        digitalWrite(MOTOR_INB, HIGH);
        Serial.println("putting out clothes....");
        
        
      }
      //Serial.println("exiting dry loop");
    // Stop the motor
    digitalWrite(MOTOR_INA, LOW);
    digitalWrite(MOTOR_INB, LOW);
        // for(;;){}

  }

  delay(100); // Adjust delay as needed
}
