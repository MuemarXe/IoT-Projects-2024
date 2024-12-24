#define Door_IR_Pin 12          // IR sensor pin for door opening
#define Person_IR_Pin 9       // IR sensor pin for presence detection

void setup() {
  Serial.begin(9600);
  pinMode(Person_IR_Pin, INPUT);
}

void loop() {
  // Read the state of the door IR sensor
  int doorState = digitalRead(Door_IR_Pin);

  // Read the state of the person IR sensor
  int personState = digitalRead(Person_IR_Pin);

  // Print the states of the IR sensors
//  Serial.print("Door IR Sensor: ");
//  Serial.println(doorState);
//
  Serial.print("Person IR Sensor: ");
  Serial.println(personState);

  delay(1000); // Delay for stability
}
