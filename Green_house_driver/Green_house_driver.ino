#include <DHT.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define SOIL_MOISTURE_PIN 3PgetHumidity()






#define RAIN_SENSOR_PIN 4
#define FAN_RELAY_PIN A2

#define RightMotorForward 8
#define RightMotorBackward 9
#define LeftMotorForward 10
#define LeftMotorBackward 11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(FAN_RELAY_PIN, OUTPUT); // Assign pin mode for the fan relay
  dht.begin();
  lcd.backlight();
  lcd.init();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("SMART GREENHOUSE");
  pinMode(RightMotorForward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
}

float getTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return 0.00;
  }
  return t;
}

float getHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return 0.00;
  }
  return h;
}

void loop() {
  float temperature = getTemperature();
  float humidity = getHumidity();
  int soilMoisture = digitalRead(SOIL_MOISTURE_PIN);
  int rainDetected = digitalRead(RAIN_SENSOR_PIN);

  if (temperature > 28) {
    digitalWrite(FAN_RELAY_PIN, HIGH);
  } else {
    digitalWrite(FAN_RELAY_PIN, LOW);
  }

  if (temperature < 28) {
    moveBackwardVent();
    delay(2000);
    moveStop();
  } else {
    moveForwardVent();
    delay(2000);
    moveStop();
  }

  if (soilMoisture == LOW) {
    moveForwardPump();
  } else {
    moveStop();
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");

  lcd.setCursor(10, 0);
  lcd.print("Fan: ");
  if (digitalRead(FAN_RELAY_PIN) == HIGH) {
    lcd.print("ON ");
  } else {
    lcd.print("OFF");
  }
  lcd.setCursor(10, 1);
  lcd.print("Pump: ");
  if (soilMoisture == LOW) {
    lcd.print("ON ");
  } else {
    lcd.print("OFF");
  }

  if (rainDetected == HIGH) {
    Serial.println("Rain detected! Taking necessary actions...");
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Soil Moisture: ");
  Serial.print(soilMoisture);
  Serial.print(", Rain Detected: ");
  Serial.println(rainDetected);

  delay(2000);
}

void moveForwardPump() {
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);
}

void moveForwardVent() {
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);
}

void moveBackwardVent() {
  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);
}

void moveStop() {
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
}
