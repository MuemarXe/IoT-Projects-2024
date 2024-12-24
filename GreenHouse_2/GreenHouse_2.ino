#include"DHT.h"
#include <LiquidCrystal.h>

#define DHTPIN 3       // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 22  (AM2302)
#define SOIL_MOISTURE_PIN 7// Digital pin connected to the soil moisture sensor
#define RAIN_SENSOR_PIN 4   // Digital pin connected to the rain sensor
#define FAN_RELAY_PIN A2     // Digital pin connected to the fan relay
#define PUMP_RELAY_PIN A3    // Digital pin connected to the pump relay
#define MOTOR_foward 9 
#define  MOTOR_backward 10  
#define  RelayledPin  8
const int ldrPin = 12;
bool elseStatementHasRun = false;



DHT dht(DHTPIN, DHTTYPE);
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
int tempVal;
float humVal;

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

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);  // Initialize the LCD display with 16 columns and 2 rows
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(ldrPin, INPUT_PULLUP);
    pinMode(MOTOR_foward, OUTPUT);
    pinMode(MOTOR_backward, OUTPUT);
    dht.begin();
   lcd.backlight();
  lcd.init(); 
  lcd.begin(16, 2);
  lcd.setCursor(0, 0); 
  lcd.print("SMART GREENHOUSE");
  pinMode(RelayledPin, OUTPUT); // Set the LED pin as output
}

void loop() {
  // Read temperature and humidity from DHT sensor
  float temperature = getTemperature();
  float humidity = getHumidity();


  // Read soil moisture level
  int soilMoisture = digitalRead(SOIL_MOISTURE_PIN);

  // Read rain sensor
  int rainDetected = digitalRead(RAIN_SENSOR_PIN);
  

  int ldrValue = digitalRead(ldrPin); // Read the value from the LDR
  Serial.print("LDR Value: ");
  Serial.println(ldrValue); // Print the value to the serial monitor

  if (ldrValue == 1) {
    digitalWrite(RelayledPin ,HIGH); // Turn on the LED if the LDR value exceeds the threshold
  } else {
    digitalWrite(RelayledPin, LOW); // Turn off the LED otherwise
  }
    lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temperature);
  //lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(humidity);
  lcd.print("%");

  // Check temperature and control fan
  if (temperature > 25) { // Adjust threshold as needed
    analogWrite(FAN_RELAY_PIN, 0);// Turn on the fan
  } else {
    analogWrite(FAN_RELAY_PIN, 255); // Turn off the fan

  }
  
  //  if ( temperature == 29) {
  //   digitalWrite(MOTOR_foward, LOW);
  //   delay(2000);
  //  digitalWrite(MOTOR_foward, HIGH);
  // }  else {
  //   // Check if the else statement has not run before
  //   if (!elseStatementHasRun) {
  //     digitalWrite(MOTOR_backward, LOW);
  //     delay(2000);
  //     digitalWrite(MOTOR_backward, HIGH);
      
  //     // Set the flag to indicate that the else statement has run
  //     elseStatementHasRun = true;
  //   }
  // }

  // Check soil moisture and control pump
  if (soilMoisture == HIGH) { // Adjust threshold as needed
    analogWrite(PUMP_RELAY_PIN, 0); // Turn on the pump
     lcd.print("ON");
  } else {
    analogWrite(PUMP_RELAY_PIN ,255);  // Turn off the pump
    lcd.print("OFF");
  }

  // Print sensor readings on LCD

  
 // Print status of pump and fan
  lcd.setCursor(9, 0);
  lcd.print("Fan:");

  if (digitalRead(FAN_RELAY_PIN) == 0) {
    lcd.print("ON");
  } else {
    lcd.print("OFF");
  }

  lcd.setCursor(8, 1);
  lcd.print("Pump:");
  if (digitalRead(PUMP_RELAY_PIN) == 0) {
    lcd.print("ON ");
  } else {
    lcd.print("OFF");
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
