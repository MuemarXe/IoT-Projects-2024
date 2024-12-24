#include <Servo.h>
#define buzzer D3
Servo servo1,servo2;
const int touch = D2;
const int relay = D4;
bool off = 0;

int pos;

int LEDState=0;
int LEDPin1=12;
int LEDPin2=13;
int buttonPin=14;
int buttonNew;
int buttonOld=1;
int dt=100;


void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(touch, INPUT);
  pinMode(relay, OUTPUT);
  servo1.attach(5);
  servo1.write(180);
  servo2.attach(15);
  servo2.write(180);

  pinMode(LEDPin1, OUTPUT);
  pinMode(LEDPin2, OUTPUT);
  pinMode(buttonPin, INPUT);

  digitalWrite(relay,HIGH);

}

void loop() {
  int touchstate = digitalRead(touch);
buttonNew=digitalRead(buttonPin);
  if (off == 0) {
    if (touchstate == HIGH) {
      digitalWrite(relay, HIGH);
      digitalWrite(LEDPin2,HIGH);
      digitalWrite(buzzer,HIGH);
      digitalWrite(LEDPin1,LOW);
      off = 1;
      Serial.print("Turn off");
     servo1.write(70);
     servo2.write(70);
    }
    else {


if(buttonOld==0 && buttonNew==1){
  if (LEDState==0){
    digitalWrite(LEDPin1,HIGH);
      digitalWrite(relay, LOW);
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
