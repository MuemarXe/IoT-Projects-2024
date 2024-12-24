//https://www.youtube.com/c/voidloopRobotechAutomation

#define LDR A0
int pin = 8 ;


void setup() {
  Serial.begin(9600);
  pinMode(8,OUTPUT);
  delay(100);

}

void loop() {
  int light = analogRead(LDR);
  
  Serial.println(light);
  delay(150);
  if ( light>300){
    digitalWrite(pin,HIGH);
    Serial.println("LightOn");
    
    }
    else{
      digitalWrite(pin,LOW);
      Serial.println("LightOff");
      }
  
}
