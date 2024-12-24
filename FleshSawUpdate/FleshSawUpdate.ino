#include <Servo.h>

Servo servo1;//D1
const int touch = D2;
const int relay = D8;

int pos;

int LEDPin1=12;//D6
int LEDPin2=13;//D7
int buttonPin=14;//D5
#define buzzer D3
int buttonState = digitalRead(buttonPin);
int touchstate = digitalRead(touch);

  enum SawState {
  WAITING_TO_START,
  PERSON_DETECTED,
  STOPPING,
};

SawState currentState = WAITING_TO_START;
void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(touch, INPUT);
  pinMode(relay, OUTPUT);
  servo1.attach(5);
  servo1.write(180);

  pinMode(LEDPin1, OUTPUT);
  pinMode(LEDPin2, OUTPUT);
  pinMode(buttonPin,INPUT_PULLUP);
  digitalWrite(relay, HIGH);
  
}

void loop() {
int buttonState = digitalRead(buttonPin);
int touchstate = digitalRead(touch);
    switch (currentState) {
    case WAITING_TO_START:

    if (buttonState== LOW){
       digitalWrite(LEDPin1,HIGH);
       digitalWrite(relay, LOW);
     currentState = PERSON_DETECTED;
    }
      break;

    case PERSON_DETECTED:

      digitalWrite(relay, LOW);
      digitalWrite(LEDPin2,LOW);
      digitalWrite(LEDPin1,HIGH);
      
      if(touchstate==HIGH){
        digitalWrite(relay, HIGH);
      digitalWrite(LEDPin2,HIGH);
      digitalWrite(buzzer,HIGH);
      digitalWrite(LEDPin1,LOW);
      servo1.write(70);
      delay(2000);
      digitalWrite(buzzer,LOW);
      
      
      currentState = STOPPING;
      }
       
      break;

    case STOPPING:
        digitalWrite(LEDPin2,LOW);
      digitalWrite(LEDPin1,LOW);
         servo1.write(180);
         delay(2000);
        currentState = WAITING_TO_START;
      
      break;

  }

  delay(500); // Delay for stability
}

