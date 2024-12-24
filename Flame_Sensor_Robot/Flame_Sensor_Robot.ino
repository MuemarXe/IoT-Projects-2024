#include <Servo.h>

#define frontFlame 4
#define leftFlame 6
#define rightFlame 5
#define red 3
#define yellow 8

Servo baseServo;  // create servo object to control a servo
Servo topServo;
int pos = 0;    // variable to store the servo position

void setup() {
  baseServo.attach(10);
  topServo.attach(11);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(frontFlame, INPUT);
  pinMode(leftFlame, INPUT);
  pinMode(rightFlame, INPUT);
  topServo.write(0); 
  baseServo.write(90); 
}

void loop(){
  if(digitalRead(frontFlame) == LOW){
    digitalWrite(red, HIGH);
    baseServo.write(90); 
    delay(500);
    topServo.write(55); 
  }

  if(digitalRead(leftFlame) == LOW){
    digitalWrite(red, HIGH);
    baseServo.write(180); 
    delay(500);
    topServo.write(55); 
  }

  if(digitalRead(rightFlame) == LOW){
    baseServo.write(0);
    digitalWrite(red, HIGH);
    delay(500);
    topServo.write(55); 
    
  } 
  if( digitalRead(rightFlame) == HIGH&&digitalRead(leftFlame) == HIGH&&digitalRead(frontFlame) == HIGH){
    digitalWrite(yellow, HIGH);
     digitalWrite(red, LOW);
     topServo.write(0); 
     baseServo.write(90); 
  }
}
