#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL2Fpnk9PEt"
#define BLYNK_TEMPLATE_NAME "Air Monitoring"
#define BLYNK_AUTH_TOKEN "S4aDAvYJ7QHXeC6gWDolqKmcVr7KvgW0"


#include <ESP8266WiFi.h>
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>

char auth[] = "BLYNK_AUTH_TOKEN";
char ssid[] = "Novatech";
char pass[] = "Nova@Tech";

SoftwareSerial espSerial(D6,D5);  // RX, TX
BlynkTimer timer;

String str_Temp;
String str_Light;
String str_Oxygen;
String str_Gas;

double str_Temp_Value;
double str_Light_Value;
double str_Oxygen_Value;
double str_Gas_Value;

float  Temprature;
float  light;
float  analog;
float  analog2;

bool parseMessage(String message, float &Temprature, float &light, float &analog, float &analog2)
{
  int indexTemprature = message.indexOf("Temperature=");
  int indexLight = message.indexOf("Light=");
  int indexOxygen = message.indexOf("Oxygen=");
  int indexGas = message.indexOf("Gas=");

  if (indexTemprature != -1 && indexLight != -1 && indexOxygen != -1 && indexGas != -1)
  {
    Temprature = message.substring(indexTemprature + 12, indexLight).toFloat();
    light = message.substring(indexLight + 6, indexOxygen).toFloat();
    analog = message.substring(indexOxygen + 8, indexGas).toFloat();
    analog2 = message.substring(indexGas + 5).toFloat();
    return true;
  }

  return false;
}

void Readings(){
   if (espSerial.available() > 0) {
   String message = espSerial.readStringUntil('\n');
      if (parseMessage (message,Temprature,light,analog,analog2)){

      Blynk.virtualWrite(V0, Temprature);
      Serial.print("The Temprature value is : ");
      Serial.println( Temprature);

      Blynk.virtualWrite(V1, light);
      Serial.print("The Light value is :");
      Serial.println(light);

      Blynk.virtualWrite(V2,analog);
      Serial.print("The Oxygen value is :");
      Serial.println(analog);

      Blynk.virtualWrite(V3, analog2);
      Serial.print("The Gas value is :");
      Serial.println(analog2);

      }
  
   }
  }

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  while (!Serial) {
; // wait for serial port to connect. Needed for native USB port only
}
  timer.setInterval(100L, Readings);
}

void loop() { 
 // Serial.print( Temprature);
  Blynk.run();
  timer.run();
}
