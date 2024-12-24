/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on NodeMCU.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right NodeMCU module
  in the Tools -> Board menu!

  For advanced settings please follow ESP examples :
   - ESP8266_Standalone_Manual_IP.ino
   - ESP8266_Standalone_SmartConfig.ino
   - ESP8266_Standalone_SSL.ino

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */

#define BLYNK_TEMPLATE_ID "TMPL2oF-_cjNk"
#define BLYNK_TEMPLATE_NAME "SmartBeehive"
#define BLYNK_AUTH_TOKEN "RSZ2ugH9t_VjMI733_gfCoKUylk4vlMX"
#define BLYNK_PRINT Serial

#include "DHT.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "HX711.h"

// HX711 circuit wiring ============start ===========
const int LOADCELL_DOUT_PIN = 12;//D6
const int LOADCELL_SCK_PIN = 13;//D7

HX711 scale;
// HX711 circuit wiring ====================end =====


// DHT11 Settings ============== start ============

#define DHTPIN D2
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

float tempVal;
float humVal;
long weightVal;
// DHT11 Settings ============== end ==============


// BLYNK settings ============== start ============
char auth[] = "T39q9s6Cj5SLeXgwcymmgniSRl_55xbC";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "maryhill";
char pass[] = "maryhill123";

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

long getWeight(){
  if (scale.is_ready()) {
    long reading = scale.read();
    return map(reading,-165300,700000,0,5000);
  } else {
    Serial.println("HX711 not found.");
    return 0000000;
  }
  
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

void sendWeight()
{

  weightVal = getWeight(); // or dht.readTemperature(true) for Fahrenheit
  Serial.print(", Weight: ");
  Serial.println(weightVal);
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, weightVal);
}
void setup()
{
  // Debug console
  Serial.begin(9600);
  // ESP.wdtDisable();
  // ESP.wdtEnable(WDTO_8S);
  dht.begin();
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  // Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // every 60000ms
  timer.setInterval(1000L, sendTemp); 
  timer.setInterval(1000L, sendHumidity); 
  timer.setInterval(1000L, sendWeight);
}

void loop()
{

  Blynk.run();
  // runs BlynkTimer
  timer.run(); 

}
