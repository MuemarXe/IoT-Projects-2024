String uid = ""; // Declare uid variable outside the loop
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
const int State =5;
int takeInpin =6;
int takeOutpin =2;
int ButtonState;
 void setup ()
{
  Serial.begin(9600);
   pinMode(State, INPUT_PULLUP);
    pinMode(takeOutpin, OUTPUT);
  pinMode(takeInpin, OUTPUT);
  SPI.begin();
  mfrc522.PCD_Init();
}
  
void loop() {
  // put your main code here, to run repeatedly:
  
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Read the UID number
    uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }

    Serial.println("UID tag is : " + uid);
    
    mfrc522.PICC_HaltA();  // Halt card
    mfrc522.PCD_StopCrypto1(); // Stop encryption on the card

    delay(50);  // Wait for a second before scanning for the next card
  }
     ButtonState = digitalRead(State);
  Serial.print("Button state: ");
  Serial.println(ButtonState);
   if( ButtonState==1){
    digitalWrite(takeInpin,HIGH);
    digitalWrite(takeOutpin, LOW);
    
    }
   if( ButtonState==0){ 
    digitalWrite(takeOutpin,HIGH);
    digitalWrite(takeInpin,LOW);
    }

  if (uid == "9285cb51" && ButtonState == 1 ) {
    Serial.println("Butter Added ");
  }
}
