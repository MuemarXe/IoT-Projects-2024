//#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL2Fpnk9PEt"
#define BLYNK_TEMPLATE_NAME "Air Monitoring"
#define BLYNK_AUTH_TOKEN "S4aDAvYJ7QHXeC6gWDolqKmcVr7KvgW0"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//char auth[] = "BLYNK_AUTH_TOKEN";
char ssid[] = "Muema";
char pass[] = "EVANS101A";

//SoftwareSerial espSerial(5, 6);  // RX, TX

String str_Temp;
String str_Light;
String str_Oxygen;
String str_Gas;

double  str_Temp_Value;
double str_Light_Value;
double str_Oxygen_Value;
double str_Gas_Value;


void setup() {
  Serial.begin(115200);
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  while (!Serial) {
; // wait for serial port to connect. Needed for native USB port only
}
}

void loop() {
  while (Serial.available() > 0) {
    char incomingChar = Serial.read();
    
    if (incomingChar != '\n') {
      str_Temp_Value += incomingChar;
    } else {
      Blynk.virtualWrite(V0, str_Temp_Value);
      Serial.println("The Temprature value is : ");
      Serial.print(str_Temp_Value);
      str_Temp = "";
    }
  }

  while (Serial.available() > 0) {
    char incomingChar = Serial.read();
    
    if (incomingChar != '\n') {
      str_Light_Value += incomingChar;
    } else {
      Blynk.virtualWrite(V1, str_Light_Value);
      Serial.println("The Light value is :");
      Serial.print(str_Light_Value);
      str_Light = "";
    }
  }

  while (Serial.available() > 0) {
    char incomingChar = Serial.read();
    
    if (incomingChar != '\n') {
     str_Oxygen_Value += incomingChar;
    } else {
      Blynk.virtualWrite(V2, str_Oxygen_Value);
      Serial.println("The Oxygen value is :");
      Serial.print(str_Oxygen_Value);
      str_Oxygen = "";
    }
  }

  while (Serial.available() > 0) {
    char incomingChar = Serial.read();
    
    if (incomingChar != '\n') {
      str_Gas_Value += incomingChar;
    } else {
      Blynk.virtualWrite(V3, str_Gas_Value);
      Serial.println("The Gas value is :");
      Serial.print(str_Gas_Value);
      str_Gas= "";
    }
  }

  Blynk.run();
}
