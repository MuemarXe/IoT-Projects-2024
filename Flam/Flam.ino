
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
Servo myservo2;
int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(9);
  myservo2.attach(10);// attaches the servo on pin 9 to the servo object
}
e
