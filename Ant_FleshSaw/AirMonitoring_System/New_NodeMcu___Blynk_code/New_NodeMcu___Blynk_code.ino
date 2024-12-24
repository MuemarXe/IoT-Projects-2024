#define BLYNK_TEMPLATE_ID "TMPL24kj0intq"
#define BLYNK_TEMPLATE_NAME "IOT Remote Current Monitoring"
#define BLYNK_AUTH_TOKEN "k_5rTJXkQEVeEK1TDSvVzmKr6rnZMepX"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>


#include <SoftwareSerial.h>

SoftwareSerial espSerial(D6, D5);

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Cups";  // type your wifi name
char pass[] = "letsdrink";  // type your wifi password
BlynkTimer timer;

    // Declare variables to store the received values
    float Irms1, Irms2, Irms3;
    float Vrms1, Vrms2, Vrms3;
    float power1, power2, power3;
    float energy1, energy2, energy3;

int rack1Switch = D7;
int rack2Switch = D1;
int rack3Switch = D2;

bool switch1State = false;
bool switch2State = false;
bool switch3State = false;

void currentMeter()
{
 
  if (espSerial.available() > 0) {
    String message = espSerial.readStringUntil('\n');



    if (parseMessage(message, Irms1, Irms2, Irms3, Vrms1, Vrms2, Vrms3, power1, power2, power3, energy1, energy2, energy3)) {
      Serial.println("Received data:");
      Serial.print("I1: "); Serial.println(Irms1);
      Serial.print("I2: "); Serial.println(Irms2);
      Serial.print("I3: "); Serial.println(Irms3);
      Serial.print("V1: "); Serial.println(Vrms1);
      Serial.print("V2: "); Serial.println(Vrms2);
      Serial.print("V3: "); Serial.println(Vrms3);
      Serial.print("P1: "); Serial.println(power1);
      Serial.print("P2: "); Serial.println(power2);
      Serial.print("P3: "); Serial.println(power3);
      Serial.print("E1: "); Serial.println(energy1);
      Serial.print("E2: "); Serial.println(energy2);
      Serial.print("E3: "); Serial.println(energy3);
      Serial.println("------");

    Blynk.virtualWrite(V0, Irms1);
    Blynk.virtualWrite(V1, Irms2);
    Blynk.virtualWrite(V2, Irms3);
    Blynk.virtualWrite(V3, Vrms1);
    Blynk.virtualWrite(V4, Vrms2);
    Blynk.virtualWrite(V5, Vrms3);
    Blynk.virtualWrite(V6, power1);
    Blynk.virtualWrite(V7, power2);
    Blynk.virtualWrite(V8, power3);
if(Vrms1 > 260 || Vrms2 > 260 || Vrms3 > 260){
    digitalWrite(rack1Switch, LOW);
    digitalWrite(rack2Switch, LOW);
      digitalWrite(rack3Switch, LOW);
  }
  else{
      digitalWrite(rack1Switch, HIGH);
    digitalWrite(rack2Switch, HIGH);
      digitalWrite(rack3Switch, HIGH);
    }
    
    }
  }
   
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  espSerial.begin(115200);
  while (!Serial) {
    // Wait for serial port to connect. Needed for native USB port only
  }
    Blynk.begin(auth, ssid, pass);
  timer.setInterval(100L, currentMeter);

  pinMode(rack1Switch, OUTPUT);
  pinMode(rack3Switch, OUTPUT);
  pinMode(rack2Switch, OUTPUT);

  digitalWrite(rack1Switch, HIGH);
    digitalWrite(rack2Switch, HIGH);
      digitalWrite(rack3Switch, HIGH);
}

void loop() {
if(switch1State==true){
  digitalWrite(rack1Switch, LOW);
}else{
  digitalWrite(rack1Switch, HIGH);
  }

  
if(switch2State==true){
  digitalWrite(rack2Switch, LOW);
}else{
  digitalWrite(rack2Switch, HIGH);
  }

  if(switch3State==true){
  digitalWrite(rack3Switch, LOW);
}else{
  digitalWrite(rack3Switch, HIGH);
  }
  
  Blynk.run();
  timer.run();
}
BLYNK_WRITE(V9) {  // Virtual pin V0 is used to control the relay
  int relayPin1State = param.asInt(); // Get the value from the Blynk app
  if (relayPin1State == HIGH){
    switch1State = true;
   // Set the relay based on the value received (HIGH or LOW)
  
  }
  else if(relayPin1State == LOW) {
    switch1State =false;
    }
    else{}
}

  BLYNK_WRITE(V10) {  // Virtual pin V0 is used to control the relay
  int relayPin2State = param.asInt(); // Get the value from the Blynk app
  if (relayPin2State == HIGH){
    switch2State = true;
   // Set the relay based on the value received (HIGH or LOW)
  
  }
  else if(relayPin2State == LOW) {
    switch2State =false;
    }
    else{}
}
BLYNK_WRITE(V11) {  // Virtual pin V0 is used to control the relay
  int relayPin3State = param.asInt(); // Get the value from the Blynk app
  if (relayPin3State == HIGH){
    switch3State = true;
   // Set the relay based on the value received (HIGH or LOW)
  
  }
  else if(relayPin3State == LOW) {
    switch3State =false;
    }
    else{}
}



bool parseMessage(String message, float &Irms1, float &Irms2, float &Irms3, float &Vrms1, float &Vrms2, float &Vrms3, float &power1, float &power2, float &power3, float &energy1, float &energy2, float &energy3)
{
  int indexI1 = message.indexOf("I1=");
  int indexI2 = message.indexOf("I2=");
  int indexI3 = message.indexOf("I3=");
  int indexV1 = message.indexOf("V1=");
  int indexV2 = message.indexOf("V2=");
  int indexV3 = message.indexOf("V3=");
  int indexP1 = message.indexOf("P1=");
  int indexP2 = message.indexOf("P2=");
  int indexP3 = message.indexOf("P3=");
  int indexE1 = message.indexOf("E1=");
  int indexE2 = message.indexOf("E2=");
  int indexE3 = message.indexOf("E3=");

  if (indexI1 != -1 && indexI2 != -1 && indexI3 != -1 &&
      indexV1 != -1 && indexV2 != -1 && indexV3 != -1 &&
      indexP1 != -1 && indexP2 != -1 && indexP3 != -1 &&
      indexE1 != -1 && indexE2 != -1 && indexE3 != -1) {
    Irms1 = message.substring(indexI1 + 3, indexI2).toFloat();
    Irms2 = message.substring(indexI2 + 3, indexI3).toFloat();
    Irms3 = message.substring(indexI3 + 3, indexV1).toFloat();
    Vrms1 = message.substring(indexV1 + 3, indexV2).toFloat();
    Vrms2 = message.substring(indexV2 + 3, indexV3).toFloat();
    Vrms3 = message.substring(indexV3 + 3, indexP1).toFloat();
    power1 = message.substring(indexP1 + 3, indexP2).toFloat();
    power2 = message.substring(indexP2 + 3, indexP3).toFloat();
    power3 = message.substring(indexP3 + 3, indexE1).toFloat();
    energy1 = message.substring(indexE1 + 3, indexE2).toFloat();
    energy2 = message.substring(indexE2 + 3, indexE3).toFloat();
    energy3 = message.substring(indexE3 + 3).toFloat();
    return true;
  }

  return false;
}
