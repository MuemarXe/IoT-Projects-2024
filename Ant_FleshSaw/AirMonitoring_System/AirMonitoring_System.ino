#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SFE_BMP180.h>
#include <SoftwareSerial.h>

SFE_BMP180 pressure;
SoftwareSerial espSerial(6, 5);

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

String str_Temp;
String str_Light;
String str_Oxygen;
String str_Gas;
String message;

double str_Temp_Value;
double str_Light_Value;
double str_Oxygen_Value;
double str_Gas_Value;

float Temperature;
float light;
float analog;
float analog2;

#define LDR A0        // LDR is connected to analog pin A0
#define Oxygen 7      // digital pin 7 for the Oxygen control
#define Gas 10         // digital pin 10 for the Gas control
#define Temp 8         // digital pin 8 for the Temperature control
#define Light 9        // digital pin 9 for the light control
#define sensorAnalog A1 // MQ3 connected to A1
#define sensorAnalog2 A2 // MQ2 connected to A2

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);
  Serial.println("BMP180 and LDR Measurements");

  if (pressure.begin())
    Serial.println("BMP180 init success");
  else {
    Serial.println("BMP180 init fail\n\n");
    while (1)
      ; // Pause forever.
  }
  Wire.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1.5);
  display.setCursor(5, 2);
//  display.print("Hello, Muema!");
//  display.setCursor(0, 10);
//  display.print("Welcome To the ");
//  display.setCursor(0, 20);
//  display.print("Air Monitoring System");
  delay(1500);
  display.display();

  pinMode(Light, OUTPUT);
  pinMode(Temp, OUTPUT);
  pinMode(Oxygen, OUTPUT);
  pinMode(Gas, OUTPUT);
}

void loop() {
//  display.setCursor(5, 2);
//  display.print("Hello, Muema!");
//  display.setCursor(0, 10);
//  display.print("Welcome To the ");
//  display.setCursor(0, 20);
//  display.print("Air Monitoring System");
//  delay(1000);
//  
  display.clearDisplay();
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
      str_Temp = String("Temperature: ") + String(Temperature, 2) + " deg C, " + String((9.0 / 5.0) * Temperature + 32.0, 2) + " deg F";
      str_Temp_Value = Temperature;

       // Display the temperature on OLED
    display.setCursor(0, 0);
    display.print("Temperature: ");
    display.setCursor(0, 10);
    display.print(Temperature, 2);
    display.print(" deg C");

      // Control the light based on temperature
      if (Temperature > 28) {
        digitalWrite(Temp, HIGH);
        Serial.println("Light On Temperature");
      } else {
        digitalWrite(Temp, LOW);
        Serial.println("Light Off Temperature");
      }
      display.display();
    } else {
      Serial.println("Error retrieving temperature measurement");
    }
  } else {
    Serial.println("Error starting temperature measurement");
  }
  delay(550);
  
  display.clearDisplay();
  // Read light intensity from LDR
  int light = analogRead(LDR);
  Serial.println("Light Intensity: " + String(light));
  str_Light = String("Light Intensity: ") + String(light);
  str_Light_Value = light;
  display.setCursor(0, 0);
  display.print("Light Intensity: ");
  display.setCursor(0, 10);
  display.print(light);
  display.print(" L");
  display.display();



  // Control the light based on light intensity
  if (light > 300) {
    digitalWrite(Light, HIGH);
    Serial.println("Light On LDR");
  } else {
    digitalWrite(Light, LOW);
    Serial.println("Light Off LDR");
  }

  delay(550);
  
  display.clearDisplay();
  int analog = analogRead(sensorAnalog);
  int analog2 = analogRead(sensorAnalog2);

  Serial.print("Oxygen value : ");
  Serial.println(analog);
  str_Oxygen = String("Oxygen value : ") + String(analog);
  str_Oxygen_Value = analog;
  display.setCursor(0, 0);
  display.print("Oxygen Intensity: ");
  display.setCursor(0, 10);
  display.print(analog);
  display.print(" t");
  display.display();


  if (analog > 200) {
    digitalWrite(Oxygen, HIGH);
    Serial.println("Light On Oxygen");

  } else {
    digitalWrite(Oxygen, LOW);
    Serial.println("Light Off Oxygen");
  }

  delay(550);
  display.clearDisplay();
  Serial.print("Gas value : ");
  Serial.println(analog2);
  str_Gas = String("Gas value : ") + String(analog2);
  str_Gas_Value = analog2;
  
  display.setCursor(0, 0);
  display.print("Gas Intensity: ");
  display.setCursor(0, 10);
  display.print(analog2);
  display.print(" t");
  display.display();


  delay(550);

  if (analog2 > 250) {
    digitalWrite(Gas, HIGH);
    Serial.println("Light On Gas");

  } else {
    digitalWrite(Gas, LOW);
    Serial.println("Light Off Gas");
  }
  
message = "Temperature=" + String(Temperature) + " Light=" + String(light) + " Oxygen=" + String(analog) + " Gas=" + String(analog2);

espSerial.println(message);
Serial.println(message);
Serial.println("After message");
delay(550);
}
