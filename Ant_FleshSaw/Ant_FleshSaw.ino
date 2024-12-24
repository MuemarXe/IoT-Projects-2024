#include <Servo.h>

Servo servo1;//D1
const int touch = D2;
const int relay = D8;
bool off = 0;

int pos;

int LEDState=0;
int LEDPin1=12;//D6
int LEDPin2=13;//D7
int buttonPin=14;//D5
int buttonNew;
int buttonOld=1;
int dt=100;

#define buzzer D3
void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(touch, INPUT);
  pinMode(relay, OUTPUT);
  servo1.attach(5);
  servo1.write(180);

  pinMode(LEDPin1, OUTPUT);
  pinMode(LEDPin2, OUTPUT);
  pinMode(buttonPin, INPUT);

  digitalWrite(relay, LOW);

}

void loop() {
  int touchstate = digitalRead(touch);
buttonNew=digitalRead(buttonPin);
  if (off == 0) {
    if (touchstate == HIGH) {
      digitalWrite(relay, LOW);
      digitalWrite(LEDPin2,HIGH);
      digitalWrite(buzzer,HIGH);
      digitalWrite(LEDPin1,LOW);
      off = 1;
      Serial.print("Turn off");
     servo1.write(70);
    }
    else {


if(buttonNew==1){
  if (LEDState==0){
    digitalWrite(LEDPin1,HIGH);
      digitalWrite(relay, HIGH);
      Serial.println("M/c running");
    
    LEDState=1;
  }
  
if(buttonOld==0 && buttonNew==1){
  if (LEDState==0){
    digitalWrite(LEDPin1,HIGH);
      digitalWrite(relay, HIGH);
      Serial.println("M/c running");
    
    LEDState=1;
  }
  else{
    digitalWrite(LEDPin1, LOW);
    digitalWrite(LEDPin2, LOW);
    digitalWrite(LEDPin1,LOW);
    digitalWrite(relay, LOW);
    LEDState=0;
  }
}
buttonOld=buttonNew;
delay(dt);

    }
  }

}
