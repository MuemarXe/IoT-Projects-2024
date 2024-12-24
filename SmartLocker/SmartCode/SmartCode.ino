#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Include LCD display library for I2C
#include <Keypad.h> // Include Keypad library
#include <Servo.h>
const int relay = A1; 
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
String uid = "";
String Stored_uid = "";
#define Password_Length 4
// Character to hold password input
char Data[Password_Length];
// Password
char Master[Password_Length] = "147";
 
byte data_count = 0; // Counter for character entries
 
char customKey; // Character to hold key input
 
// Constants for row and column sizes
const byte ROWS = 4;
const byte COLS = 4;
 
// Array to represent keys on keypad
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
 
// Connections to Arduino
//byte rowPins[ROWS] = {11, 8, 7, 6};
//byte colPins[COLS] = {5, 4, 3, 2};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 11};
 
 
// Create keypad object
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
 
// Create LCD object
LiquidCrystal_I2C lcd(0x27, 20, 4);

enum LockerState {
  SCAN_RFID,
  COMPARE_UID,
  CHECK_PAYMENT_INFO,
  COMPARE_UID2,
};
LockerState currentState = SCAN_RFID;
bool lcdMessageDisplayed = false;
void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  lcd.backlight();
  lcd.init();
  lcd.begin(20, 4);
  myservo.attach(12);
  pinMode(12,OUTPUT);
    myservo.write(100);   


  pinMode(relay, OUTPUT);
  analogWrite(relay, 255);
}
  void clearData() {
  // Go through array and clear data
  while (data_count != 0) {
    Data[data_count--] = 0;
  }
  return;
  }
void loop() {
 switch (currentState) {
    case  SCAN_RFID:{
      while (currentState == SCAN_RFID) {
      //digitalWrite(12,LOW);
      //myservo.write(pos);  
    // Serial.println("Scan RFID card to get UID...");
   if (!lcdMessageDisplayed) {
        Serial.println("Scan RFID card to get UID...");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Scan  Card to");
        lcd.setCursor(0, 1);
        lcd.print("Open Locker...");
        lcdMessageDisplayed = true;
      }

     if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Read the UID number
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }

    Serial.println("UID tag is : " + uid);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("UID: ");
        lcd.setCursor(7, 0);
        lcd.print(uid);
        lcd.setCursor(0, 1);
        lcd.print("Welcome...");
        delay(500);
        analogWrite(relay, 0);  
        delay(4000);  
        analogWrite(relay, 255);     

    
    mfrc522.PICC_HaltA();  // Halt card
    mfrc522.PCD_StopCrypto1(); // Stop encryption on the card

    delay(50);  // Wait for a second before scanning for the next card
    Stored_uid = uid;
    currentState = COMPARE_UID ;
    lcd.clear();
  }
}   
    break;
 }
    case COMPARE_UID:{
      while (currentState == COMPARE_UID){
        analogWrite(relay, 255); 
        Serial.println("Next state....COMPARE_UID .");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Locker Occupied..");
        lcd.setCursor(0, 1);
        lcd.print("Scan To Open ");
        //delay(500);
        

    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Read the UID number
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }

    Serial.println("UID tag is : " + uid);
    
    
    mfrc522.PICC_HaltA();  // Halt card
    mfrc522.PCD_StopCrypto1(); // Stop encryption on the card
    delay(250);  // Wait for a second before scanning for the next card

    if (uid == Stored_uid){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Correct Card..");
        delay(500);
    currentState = CHECK_PAYMENT_INFO;
    
    lcd.clear();
    }
    else{
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Invalid Card..");
        delay(500);
      }
  }
 }
      break;
    }
 case CHECK_PAYMENT_INFO:{
      while (currentState == CHECK_PAYMENT_INFO) {
        Serial.println("Next state....CHECK_PAYMENT_INFO ");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Ent Payment det:");
        customKey = customKeypad.getKey();
        if (customKey) {
          Data[data_count] = customKey;
          lcd.setCursor(data_count, 1);
          lcd.print(Data[data_count]);
          data_count++;
        }
        if (data_count == Password_Length - 1) {
          lcd.clear();
          if (!strcmp(Data, Master)) {
            lcd.print("Payment Verified");
            delay(1000);
            lcd.clear();
            lcd.print("Open Locker");
            analogWrite(relay, 0);
            delay(4000);
            analogWrite(relay, 255);
            currentState = COMPARE_UID2;
            
          } else {
            lcd.print("Payment Not ");
            lcd.setCursor(0, 1);
            lcd.print("Verified ");
            delay(3000);
          }
          lcd.clear();
          clearData();
        }
       //break; 
      }
      //currentState = SCAN_RFID; // Reset state to SCAN_RFID
      break;
    }
       case COMPARE_UID2:{
      
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("   THANK YOU ");
        lcd.setCursor(2, 1);
        lcd.print("WELCOME BACK");
        delay(3000);
       currentState == SCAN_RFID;
         break;
 }
    
    default:
      currentState == SCAN_RFID;
    
  }
}