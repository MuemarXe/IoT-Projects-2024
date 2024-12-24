#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL2Fpnk9PEt"
#define BLYNK_TEMPLATE_NAME "Air Monitoring"
#define BLYNK_AUTH_TOKEN "S4aDAvYJ7QHXeC6gWDolqKmcVr7KvgW0"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "BLYNK_AUTH_TOKEN";
char ssid[] = "Novatech";
char pass[] = "Nova@Tech";

BlynkTimer timer;

 
String str_Temp;
String str_Light;
String str_Oxygen;
String str_Gas;

double str_Temp_Value;
double str_Light_Value;
double str_Oxygen_Value;
double str_Gas_Value;

float Temperature;
float light;
float analog;
float analog2;

void Readings() {
  
  if (Serial.available() > 0) {
    String message = Serial.readStringUntil('\n');
    Serial.println("Received message: " + message); 
    if (parseMessage(message, Temperature, light, analog, analog2)) {
      Blynk.virtualWrite(V0, Temperature);
      Serial.print("Temperature: ");
      Serial.println(Temperature);

      Blynk.virtualWrite(V1, light);
      Serial.print("Light: ");
      Serial.println(light);

      Blynk.virtualWrite(V2, analog);
      Serial.print("Oxygen: ");
      Serial.println(analog);

      Blynk.virtualWrite(V3, analog2);
      Serial.print("Gas: ");
      Serial.println(analog2);
    }
  }
}

void setup() {
  Serial.begin(115200);  // Use hardware serial for communication with ESP8266
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  timer.setInterval(100L, Readings);
}

void loop() {
  Blynk.run();
  timer.run();
}

bool parseMessage(String message, float &Temperature, float &light, float &analog, float &analog2) {
  int indexTemperature = message.indexOf("Temperature=");
  int indexLight = message.indexOf("Light=");
  int indexOxygen = message.indexOf("Oxygen=");
  int indexGas = message.indexOf("Gas=");

  if (indexTemperature != -1 && indexLight != -1 && indexOxygen != -1 && indexGas != -1) {
    Temperature = message.substring(indexTemperature + 12, indexLight).toFloat();
    light = message.substring(indexLight + 6, indexOxygen).toFloat();
    analog = message.substring(indexOxygen + 8, indexGas).toFloat();
    analog2 = message.substring(indexGas + 4).toFloat();
    return true;
  }

  return false;
}
