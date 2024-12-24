#define BLYNK_TEMPLATE_ID "TMPL2L9aai7qJ"
#define BLYNK_TEMPLATE_NAME "Smart Brooder"
#define BLYNK_AUTH_TOKEN "nt0I7E7LuwCIsdrheyO9R2FXR6TXtSSh"
#define BLYNK_PRINT Serial

#include <Servo.h>
#include <LiquidCrystal_I2C.h>
// #include <SoftwareSerial.h>
#include <BlynkSimpleShieldEsp8266.h>
#define EspSerial Serial1
#include <ESP8266_Lib.h>

// SoftwareSerial EspSerial(11, 10); // RX, TX

#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "kianyaga";
char pass[] = "kianyaga123";

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo1, servo2;

#include <DHT.h>
DHT dht(2, DHT11); //(sensor pin,sensor type)

#define trig1 7
#define echo1 6
#define trig2 5
#define echo2  4

#define heatRelay 12
#define fanRelay  3
#define shakeRelay 13

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034

long duration1, duration2;
float distance1, distance2;

int maxFood = 100;
float maxContainerSpace = 22;
float minContainerSpace = 4;
int sensorValue;

float t, h;
BlynkTimer timer;

void setup() {

  Serial.begin(9600); // Starts the serial communication
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(heatRelay, OUTPUT);
  pinMode(fanRelay, OUTPUT);
  pinMode(shakeRelay, OUTPUT);

  digitalWrite(fanRelay, HIGH);
  digitalWrite(heatRelay, HIGH);
  digitalWrite(shakeRelay, HIGH);

  lcd.init();
  lcd.clear();
  lcd.backlight(); // Make sure backlight is on

  // Print a message on both lines of the LCD.
  lcd.setCursor(1, 0); //Set cursor to character 2 on line 0
  lcd.print("SMART CHICKEN");

  lcd.setCursor(3, 1); //Move cursor to character 2 on line 1
  lcd.print("BROODER");

  dht.begin();

  servo1.attach(9);
  servo1.write(0);
  servo2.attach(8);
  servo2.write(0);
  delay(2000);

  int sensorValue = analogRead(A0);
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
//  Blynk.begin(auth, ssid, pass);
Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass, "blynk.cloud", 80);
  // Timer setup
  timer.setInterval(1000L, sendSensorData); // Send sensor data every second
}

void sendSensorData() {

  delay(20);
  h = dht.readHumidity();
  t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Clears the trigPin1
  digitalWrite(trig1, LOW);
  delayMicroseconds(2);
  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);

  // Calculate the distance
  duration1 = pulseIn(echo1, HIGH);
  distance1 = duration1 * SOUND_VELOCITY / 2;
  float availableContainerSpace = ((distance1 - minContainerSpace) * 100) / maxContainerSpace;
  maxFood = 100 - availableContainerSpace;
  digitalWrite(trig2, LOW);
  delayMicroseconds(2);
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);

  duration2 = pulseIn(echo2, HIGH);
  distance2 = duration2 * SOUND_VELOCITY / 2;

  lcd.clear();
  lcd.setCursor(0, 0); //Set cursor to character 2 on line 0
  lcd.print("Available: ");
  lcd.print(maxFood);
  lcd.print("%");
  lcd.setCursor(0, 1); //Set cursor to character 2 on line 0
  lcd.print("T:");
  lcd.print(t);
  lcd.setCursor(8, 1); //Set cursor to character 2 on line 0
  lcd.print("H:");
  lcd.print(h);
  lcd.print("%");

  if (maxFood <= 20) {
    lcd.clear();
    lcd.setCursor(0, 0); //Set cursor to character 2 on line 0
    lcd.print("LOW FOOD LEVEL");
    lcd.setCursor(0, 1); //Set cursor to character 2 on line 0
    lcd.print("Restock Food!");

    delay(1000);
  } else if (distance2 >= 6.0) {
    lcd.clear();
    lcd.setCursor(0, 0); //Set cursor to character 2 on line 0
    lcd.print("FEED PAN LOW");
    lcd.setCursor(0, 1); //Set cursor to character 2 on line 0
    lcd.print("ADDING & SHAKING");
    servo1.write(90);

    digitalWrite(shakeRelay, LOW);
    delay(4000);

    for (int i = 0; i <= 20; i++) {
      servo2.write(90);
      delay(100);
      servo2.write(0);
      delay(100);
    }
  } else {
    servo1.write(0);
  }

  if (t <= 22) {
    lcd.clear();
    lcd.setCursor(0, 0); //Set cursor to character 2 on line 0
    lcd.print("BROODER TEMP LOW");
    lcd.setCursor(0, 1); //Set cursor to character 2 on line 0
    lcd.print("TURNING ON HEAT");
    digitalWrite(heatRelay, LOW);

    delay(100);
  } else if (t >= 28) {
    lcd.clear();
    lcd.setCursor(0, 0); //Set cursor to character 2 on line 0
    lcd.print("BROODER TEMP HIGH");
    lcd.setCursor(0, 1); //Set cursor to character 2 on line 0
    lcd.print("TURNING ON FAN");
    digitalWrite(fanRelay, LOW);

    delay(100);
  } else {
    digitalWrite(fanRelay, HIGH);
    digitalWrite(heatRelay, HIGH);
    digitalWrite(shakeRelay, HIGH);
  }

  // Serial.print("Distance1: ");
  // Serial.println(distance1);
  // Serial.print("Distance2: ");
  // Serial.println(distance2);

  delay(100);
   Blynk.virtualWrite(V0, t); // Write temperature data to Blynk virtual pin V5
  Blynk.virtualWrite(V1, h); // Write humidity data to Blynk virtual pin V6
  Blynk.virtualWrite(V2, maxFood); // Write food level data to Blynk virtual pin V7
}

void loop() {
   Blynk.run(); // Run Blynk communication
  timer.run(); // Run BlynkTimer
}