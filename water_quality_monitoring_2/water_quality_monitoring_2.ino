#include <LCD_I2C.h>

LCD_I2C lcd(0x27, 16, 2); 

#include <Arduino.h>
#define RELAY 7
#define TURBIDITY A0

// #include <LiquidCrystal_I2C.h>

// #include <Wire.h>
float voltage =0;


// LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the I2C LCD address and dimensions

// Custom mapping function for float values
 float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
 }

void setup() {
  
  lcd.begin();   // Initialize the LCD
  Serial.begin(9600); // Baud rate: 9600
  Serial.println("starting program...");
  lcd.backlight();
  pinMode(RELAY,OUTPUT);
  digitalWrite(RELAY,LOW);
}

void loop() {
  Serial.println("loop");
  Serial.println(voltage);
  //  int sensorValue = analogRead(A0);  // Read the input on analog pin A0 (TURBIDITY)
  
  for (int i=0;i<800;i++){//smooth out values by accumulation
    voltage+=((float)analogRead(TURBIDITY)/1023)*5;
    //Serial.println(voltage);
  }
  voltage=voltage/800;//average out values
  // voltage = sensorValue * (5.0 / 1024.0);  // Convert analog reading to voltage (0 - 5V)
  Serial.println(voltage);
  // Custom map the voltage to percentage cleanliness (2V = 10%, 4V = 90%)
  float cleanlinessPercentage = mapFloat(voltage,4.0, 0.5, 10.0, 95.0);

  if(cleanlinessPercentage>=99.0){cleanlinessPercentage=99.0;}
  if(cleanlinessPercentage<=0.0){cleanlinessPercentage=0.0;}
  Serial.print("Cleanliness: ");
  Serial.print(cleanlinessPercentage);
  Serial.println("%");

 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("CleanLevel:");
 lcd.print(cleanlinessPercentage,1);
 lcd.print("%");

 lcd.setCursor(0, 1);
// Indicate cleanliness status on the second row
//  if (cleanlinessPercentage > 96.0) {
//  lcd.print("PIPES EMPTY");
//  digitalWrite(RELAY,HIGH);
//  } else 
if (cleanlinessPercentage < 45.0) {
lcd.print("DIRTY,DON'T USE");
 digitalWrite(RELAY,LOW);
 }else if(cleanlinessPercentage >80){
   lcd.print("CLEAN, USABLE");
 digitalWrite(RELAY,HIGH);
 } 
 else {
lcd.print("CLOUDY, UNSAFE");
 digitalWrite(RELAY,LOW);
 }

  delay(500);
}
