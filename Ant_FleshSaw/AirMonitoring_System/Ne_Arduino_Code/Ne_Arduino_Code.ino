// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
SoftwareSerial espSerial(5, 6);

#include "EmonLib.h"         // Include Emon Library
double Vrms1, Vrms2, Vrms3;
float power1, power2, power3;
double Irms1, Irms2, Irms3;
String message;
int batteryPower;

int relayPin1 = 5;
int relayPin2 = 6;
int relayPin3 = 7;

float energy1 = 0.0, energy2 = 0.0, energy3 = 0.0;

LiquidCrystal_I2C lcd(0x27, 16, 2); 

EnergyMonitor emon1, emon2, emon3;         // Create an instance

void setup() { 
;
Serial.begin(115200);
  espSerial.begin(115200);
  
pinMode(relayPin1, OUTPUT);
pinMode(relayPin2, OUTPUT);
pinMode(relayPin3, OUTPUT);


 
 lcd.init();          
 lcd.backlight();
 lcd.cursor_off();
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Remote Current");
  lcd.setCursor(2,1);
 lcd.print("Monitoring");
 delay(1000);
 emon1.current(A5, 1.100999);      // Current: input pin, calibration.
 emon2.current(A3, 1.100999);      // Current: input pin, calibration.
 emon3.current(A1, 1.100999);      // Current: input pin, calibration.
 emon1.voltage(A4,366, 0);
 emon2.voltage(A2,366, 0);
 emon3.voltage(A0,366, 0);
 lcd.clear();
}

void loop() {
 emon1.calcVI(20, 50);
 emon2.calcVI(20, 50);
 emon3.calcVI(20, 50);
 Irms1 = emon1.calcIrms(1); // Calculate Irms only
 Irms2 = emon2.calcIrms(1); // Calculate Irms only
 Irms3 = emon3.calcIrms(1); // Calculate Irms only

 Vrms1 = emon1.Vrms;
 Vrms2 = emon2.Vrms;
 Vrms3 = emon3.Vrms;
 
 
 power1=(Irms1*Vrms1);
 power2=(Irms2*Vrms2);
 power3=(Irms3*Vrms3);

  energy1 = energy1 + (power1 / 3600); //Wh
    energy2 = energy2 + (power2 / 3600); //Wh
      energy3 = energy3 + (power3 / 3600); //Wh
      
 message = "I1=" + String(Irms1) + " I2=" + String(Irms2) + " I3=" + String(Irms3) + " V1=" + String(Vrms1) + " V2=" + String(Vrms2) + " V3=" + String(Vrms3) + " P1=" + String(power1) + " P2=" + String(power2) + " P3=" + String(power3) + " E1=" + String(energy1) + " E2=" + String(energy2) + " E3=" + String(energy3) + "\n";
  espSerial.println(message);

 //batteryPower = analogRead(25); 
//float batteryVoltage = batteryPower * 12.95/ 4095;

lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("R1:");
 lcd.setCursor(3,0);
 lcd.print(Irms1);lcd.print("A");
 lcd.setCursor(9,0);
 lcd.print("R2:");
 lcd.print(Irms2);lcd.print("A");
 lcd.setCursor(0,1);
 lcd.print("R3:");
 lcd.print(Irms3);lcd.print("A");
// lcd.setCursor(8,1);
// lcd.print("B:");
// lcd.setCursor(10,1);
// lcd.print(batteryVoltage);lcd.print("V");


 Serial.print("Load1 current:");
 Serial.println(Irms1);
 Serial.print("Load2 current:");
 Serial.println(Irms2);
 Serial.print("load3 current:");
 Serial.println(Irms3);
Serial.println("-------------------------------------");
Serial.println("-------------------------------------");
  Serial.print("Load1 Energy Consumtion:");
 Serial.println(energy1);
 Serial.print("Load2 Energy Consumtion:");
 Serial.println(energy2);
 Serial.print("load3 Energy Consumtion:");
 Serial.println(energy3);
 Serial.println("-------------------------------------");
Serial.println("-------------------------------------");
 Serial.print("Load1 Voltage:");
 Serial.println(Vrms1);
 Serial.print("Load2 Voltage:");
 Serial.println(Vrms2);
 Serial.print("load3 Voltage:");
 Serial.println(Vrms3);
 Serial.println("-------------------------------------");
Serial.println("-------------------------------------");
Serial.print("Load1 Power:");
 Serial.println(power1);
 Serial.print("Load2 Power:");
 Serial.println(power2);
 Serial.print("load3 Power:");
  Serial.println(power3);

// Serial.print("Battery Reading: ");
// Serial.println(batteryPower);
// Serial.print("Battery Voltage: ");
// Serial.println(batteryVoltage);

if(power1>100 && power2<100 && power3<100)
{
  digitalWrite(relayPin1, LOW);
  }
  else if(power2>100 && power1<100 && power3<100)
  {
    digitalWrite(relayPin2, LOW);
    }
    else if(power3>100 && power1<100 && power2<100)
    {
      digitalWrite(relayPin3, LOW);
      }
      else if(power1>100 && power2>100 && power3<100)
      {
        digitalWrite(relayPin1, LOW);
        digitalWrite(relayPin2, LOW);
        
        }
        else if(power1>100 && power3>100 && power2<100)
      {
        digitalWrite(relayPin1, LOW);
        digitalWrite(relayPin3, LOW);
        }
        else if(power2>100 && power3>100 && power1<100)
      {
        digitalWrite(relayPin2, LOW);
        digitalWrite(relayPin3, LOW);
        }
        else if(power1>100 && power2>100 && power3>100)
      {
        digitalWrite(relayPin1, LOW);
        digitalWrite(relayPin2, LOW);
        digitalWrite(relayPin3, LOW);
        
        }
        else
        {
          digitalWrite(relayPin1, HIGH);
        digitalWrite(relayPin2, HIGH);
        digitalWrite(relayPin3, HIGH);
          }
        



}
