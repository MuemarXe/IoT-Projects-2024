#include <DHT.h>

#define DHTPIN 4        // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define SOIL_MOISTURE_PIN 2 // Digital pin connected to the soil moisture sensor
#define RAIN_SENSOR_PIN 4   // Digital pin connected to the rain sensor
#define FAN_RELAY_PIN A2    // Digital pin connected to the fan relay
#define PUMP_RELAY_PIN A3   // Digital pin connected to the pump relay

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(FAN_RELAY_PIN, OUTPUT);
  pinMode(PUMP_RELAY_PIN, OUTPUT);
}

void loop() {
  // Read temperature and humidity from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read soil moisture level
  int soilMoisture = digitalRead(SOIL_MOISTURE_PIN);

  // Read rain sensor
  int rainDetected = digitalRead(RAIN_SENSOR_PIN);

  // Check temperature and control fan
  if (temperature > 28) { // Adjust threshold as needed
    analogWrite(FAN_RELAY_PIN, 255); // Turn on the fan
  } else {
    analogWrite(FAN_RELAY_PIN, 0);  // Turn off the fan
  }

  // Check soil moisture and control pump
  if (soilMoisture == LOW) { // Adjust threshold as needed
     analogWrite(PUMP_RELAY_PIN, 255); // Turn on the pump
  } else {
    analogWrite(PUMP_RELAY_PIN, 0);  // Turn off the pump
  }

  // Check rain sensor and take action if rain is detected
  if (rainDetected == HIGH) {
    // You can implement actions like closing windows, stopping irrigation, etc.
    // For simplicity, let's just print a message here.
    Serial.println("Rain detected! Taking necessary actions...");
  }

  // Print sensor readings for monitoring
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Soil Moisture: ");
  Serial.print(soilMoisture);
  Serial.print(", Rain Detected: ");
  Serial.println(rainDetected);

  delay(2000); // Adjust delay as needed for your application
}
