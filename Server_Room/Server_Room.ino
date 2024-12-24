#define BLYNK_TEMPLATE_ID "TMPL2oF-_cjNk"
#define BLYNK_TEMPLATE_NAME "SmartBeehive"
#define BLYNK_AUTH_TOKEN "RSZ2ugH9t_VjMI733_gfCoKUylk4vlMX"
#define BLYNK_PRINT Serial

#include "DHT.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "EmonLib.h"                   // Include Emon Library

EnergyMonitor emon1;                   // Create an instance

#define CT_pin A0  // CT sensor pin connected to A2 pin of Arduino

// DHT11 Settings ============== start ============

#define DHTPIN D2
#define DHTTYPE DHT11   // DHT 11
#define fan D7
#define LED1 D5
#define LED2 D6
#define LED3 D4
const int touch = D8;


DHT dht(DHTPIN, DHTTYPE);

float tempVal;
float humVal;

double irms;
// DHT11 Settings ============== end ==============


// BLYNK settings ============== start ============

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Novatech";
char pass[] = "Nova@Tech";

BlynkTimer timer; 

// BLYNK settings ============== end =============

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

void sendTemp() 
{
  tempVal = getTemperature();// or dht.readTemperature(true) for Fahrenheit
  Serial.print("Temperature: ");
  Serial.print(tempVal);

  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V0, tempVal);
}

void sendHumidity()
{

  humVal = getHumidity(); // or dht.readTemperature(true) for Fahrenheit
  Serial.print(", Humidity: ");
  Serial.println(humVal);
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, humVal);
}


void sendCurrent(){
  Serial.print("Current: ");
  Serial.print(irms);
  Serial.println(" A");
  Blynk.virtualWrite(V2, irms);
}
void setup()
{
  // Debug console
  Serial.begin(9600);
  dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pinMode(fan, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  emon1.current(CT_pin, 10);  // CT sensor calibration: sensitivity = 10
  

  timer.setInterval(1000L, sendTemp); 
  timer.setInterval(1000L, sendHumidity); 
 
}
void loop(){
  
    if( tempVal >=23){
    digitalWrite(fan,HIGH);
    }
    else  {
    digitalWrite(fan,LOW);
      }

  double irms = emon1.calcIrms(1480); // Measure RMS current
  Serial.print("Current: ");
  Serial.print(irms);
  Serial.println(" A");
   

  Blynk.run();
  // runs BlynkTimer
  timer.run(); 

}

void touched(){
    int touchstate = digitalRead(touch);
    if (touchstate == HIGH) {
      digitalWrite(LED_BUILTIN,HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN,LOW);
      digitalWrite(LED_BUILTIN,HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN,LOW);
      digitalWrite(LED_BUILTIN,HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN,LOW);
      digitalWrite(LED_BUILTIN,HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN,LOW);
  }
}
