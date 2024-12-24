#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
String uid ="";

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Scan your RFID card...");
}

void loop() {
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

    delay(500);  // Wait for a second before scanning for the next card
    
    delay(5);

  }
}


