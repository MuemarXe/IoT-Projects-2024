#include<Wire.h>
#include <SoftwareSerial.h>
#define FLOAT_SENSOR  8 
//  #define Float_Switch 3
SoftwareSerial mySerial(11, 10); //SIM800L Tx & Rx is connected to Arduino #11 & #10

const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
 
int minVal=265;
int maxVal=402;
double angleX = 215;
double angleY = 100;
double angleZ = 352;
double x;
double y;
double z;
 
void setup(){
Wire.begin();
Wire.beginTransmission(MPU_addr);
Wire.write(0x6B);
Wire.write(0);
Wire.endTransmission(true);
Serial.begin(9600);
mySerial.begin(9600);
 pinMode(FLOAT_SENSOR, INPUT_PULLUP);
}

void loop(){
checkCapsizing();
checkWaterLevel();
Wire.beginTransmission(MPU_addr);
Wire.write(0x3B);
Wire.endTransmission(false);
Wire.requestFrom(MPU_addr,14,true);
AcX=Wire.read()<<8|Wire.read();
AcY=Wire.read()<<8|Wire.read();
AcZ=Wire.read()<<8|Wire.read();
int xAng = map(AcX,minVal,maxVal,-90,90);
int yAng = map(AcY,minVal,maxVal,-90,90);
int zAng = map(AcZ,minVal,maxVal,-90,90);
 
x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
 
Serial.print("AngleX= ");
Serial.println(x);
 
Serial.print("AngleY= ");
Serial.println(y);
 
Serial.print("AngleZ= ");
Serial.println(z);
Serial.println("-----------------------------------------");
delay(1000);
}

void checkCapsizing(){
  int angleChange = (angleX-x, angleY-y, angleZ-z);
  if (angleChange > 50){
    Serial.print("Boat is Capsizing");
    SendMessage1();
  }else{
    Serial.println("You are safe");
  }
}

void checkWaterLevel(){
  if(digitalRead(FLOAT_SENSOR) == HIGH){
    Serial.println("Water Level is critical");
    SendMessage2();
  } 
}
void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
void SendMessage1()
{
    mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
    updateSerial();

    mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
    updateSerial();
    mySerial.println("AT+CMGS=\"+254797560872\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
    updateSerial();
    mySerial.print("Boat is capsizing please help!"); //text content
    updateSerial();
    mySerial.write(26);
}

void SendMessage2()
{
    mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
    updateSerial();

    mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
    updateSerial();
    mySerial.println("AT+CMGS=\"+254797560872\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
    updateSerial();
    mySerial.print("Critical water level. Boat is capsizing please help"); //text content
    updateSerial();
    mySerial.write(26);
}
