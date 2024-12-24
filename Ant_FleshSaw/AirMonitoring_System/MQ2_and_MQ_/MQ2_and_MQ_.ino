/*MQ-3 sensor with Arduino.
  created by the SriTu Tech team.
  Read the code below and use it for any of your creations.
  https://srituhobby.com
*/
#define sensorAnalog A1
#define Gas 6
#define Oxygen 7
#define sensorAnalog2 A2
void setup() {
  pinMode(Gas, OUTPUT);
  pinMode(Oxygen, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  int analog = analogRead(sensorAnalog);
  int analog2 = analogRead(sensorAnalog2);
 
  Serial.print("Analog value : ");
  Serial.print(analog);
  Serial.print("t");
  delay(1000);

  Serial.print("          Analog value2 : ");
  Serial.print(analog2);
  Serial.print("t");
  
  delay(700);
  
  /*if ( analog>300){
    digitalWrite(Oxygen,HIGH);
    Serial.println("LightOnOxygen");
    
    }
    else{
      digitalWrite(Oxygen,LOW);
      Serial.println("LightOffOxygen");
      }
      */
 if ( analog2 >250){
    digitalWrite(Gas,HIGH);
    Serial.println("LightOnGas");
    
    }
 else{
      digitalWrite(Gas,LOW);
      Serial.println("LightOffGas");
      }
}
