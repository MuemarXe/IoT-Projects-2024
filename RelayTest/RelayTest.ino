const int  relay = A0;
void setup() {
  // put your setup code here, to run once:
 pinMode(relay, OUTPUT);
 Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 analogWrite(relay,0);
 Serial.println("on relay");
 delay(5000);
//  analogWrite(relay,1023);
// Serial.println("oFF relay");
 
}
