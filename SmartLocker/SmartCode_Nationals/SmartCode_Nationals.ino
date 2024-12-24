#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Include LCD display library for I2C
// #include <Keypad.h> // Include Keypad library

const int relayLocker1 = A1; 
const int relayLocker2 = A3; 
const int buttonPin = A0;

// String uid = "";
String Stored_uid = "";
static int selectedLocker =0; 
unsigned long startTime = 0;
int bill;
const byte Password_Length =4;
const byte  Password_Length_2 =10;
// Character to hold password input
char Data[Password_Length];
// Password
char Master[Password_Length] = "147";

char Data2[Password_Length_2];
// Password
char Master2[Password_Length_2] = " ";
static byte data_count = 0; // Counter for character entries
 
//char customKey; // Character to hold key input

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
 

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 11};
 
char getPressedKey();//keypad function

// Create LCD object
LiquidCrystal_I2C lcd(0x27, 20, 4);

enum LockerState {
  WELCOME,
  SCAN_RFID,
  LOCKER_SIZE,
  COMPARE_UID,
  ENTER_NUMBER,
  CHECK_PAYMENT_INFO,
  FINISH,
};
LockerState currentState = WELCOME;
bool lcdMessageDisplayed = false;
bool lcdMessageDisplayed2 = false;
bool lcdMessageDisplayed3 = false;
static bool rfidFlag=0 ;
void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  lcd.backlight();
  lcd.init();
  lcd.begin(20,4);

  pinMode(relayLocker1, OUTPUT);
  pinMode(relayLocker2, OUTPUT);
  analogWrite(relayLocker1,0);
  analogWrite(relayLocker2,0);
   pinMode(buttonPin, INPUT);
}
  void clearData() {
  // Go through array and clear data
  while (data_count != 0) {
    Data[data_count--] = 0;

  }
  data_count=0;
  return;
  }
void loop() {
    int buttonState = analogRead(buttonPin);
    char customKey;
 switch (currentState) {
  case  WELCOME :
     if (!lcdMessageDisplayed) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Welcome to Smart  ");
        lcd.setCursor(3, 1);
        lcd.print(" Lugguage Locker ");
        lcd.setCursor(1, 2);
        lcd.print("Press Push Button ");
        lcd.setCursor(0, 3);
        lcd.print("  To access Locker ");
        lcdMessageDisplayed = true;
        
      }

  delay(1000); // Delay before restarting the scrolling
   Serial.print("Button State: ");
  Serial.println(buttonState);
if(buttonState < 100){
  startTime = millis(); // Start time when entering SCAN_RFID state
  lcdMessageDisplayed = false;
  currentState = SCAN_RFID;

}

    
    break;
      case  SCAN_RFID:
      while (currentState == SCAN_RFID) {

   if (!lcdMessageDisplayed) {
        Serial.println("Scan RFID card to get UID...");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Scan  Card to");
        lcd.setCursor(0, 1);
        lcd.print("Open Locker...");
        lcdMessageDisplayed = true;
        
      }

     if (mfrc522.PICC_IsNewCardPresent()) {
     // Serial.print("rfid flag:");Serial.print(rfidFlag);Serial.println();
    // Read the UID number
    if (mfrc522.PICC_ReadCardSerial()) {
          String uid = "";
          for (byte i = 0; i < mfrc522.uid.size; i++) {
            uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
            uid += String(mfrc522.uid.uidByte[i], HEX);
          }
          mfrc522.PICC_HaltA();  // Halt card
          Serial.println("UID tag is : " + uid);
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("UID: ");
              lcd.setCursor(7, 0);
              lcd.print(uid);
              lcd.setCursor(0, 1);
              lcd.print("Welcome...");
              lcd.setCursor(0, 2);
              lcd.print("              ");
              lcd.setCursor(0, 3);
              lcd.print("              ");
              delay(3000);  

          
         
          // rfidFlag=0;
        //  mfrc522.PCD_StopCrypto1(); // Stop encryption on the card

          delay(50);  // Wait for a second before scanning for the next card
          Stored_uid = uid;
          currentState = LOCKER_SIZE;
          lcd.clear();
        }
        else {
                Serial.println("Error reading card serial.");
            }
     }else{Serial.println("no new card detected");}
}

    break;

// Serial.print("rfid flag:");Serial.print(rfidFlag);Serial.println();
case LOCKER_SIZE:
  while (currentState == LOCKER_SIZE) {
    selectedLocker = 0;
    // Stored_uid = uid;
    if (!lcdMessageDisplayed2) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pick Locker Size ");
      lcd.setCursor(0, 2);
      lcd.print("Select \"1\" for Big");
      lcd.setCursor(0, 3);
      lcd.print("Select \"2\" for Small");
      lcdMessageDisplayed2 = true;
    }
    // delay(5000);//this delay is just for debugging,,remove it later
    char customKey = getPressedKey();
    if (customKey) {
      // Print the pressed key to Serial Monitor
      Serial.print("Pressed Key: ");
      Serial.println(customKey);
      
      // Handle keypad input
      if (customKey == '1') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Locker Selected:");
        lcd.print(customKey);
        selectedLocker = relayLocker1;
        lcd.setCursor(4, 1);
        lcd.print(" Big Locker ");
        lcd.setCursor(5, 2);
        lcd.print(" Selected");
        analogWrite(selectedLocker, 255); // Activate relay for locker A
        delay(4000);
        analogWrite(selectedLocker, 0);
         delay(3000);
        // Stored_uid = uid;
        currentState = COMPARE_UID;
      } else if (customKey == '2') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Locker Selected:");
        lcd.print(customKey);
        selectedLocker = relayLocker2;
        lcd.setCursor(5, 1);
        lcd.print("Small Locker ");
        lcd.setCursor(5, 2);
        lcd.print(" Selected");
        analogWrite(selectedLocker, 255); // Activate relay for locker B
        delay(4000);
        analogWrite(selectedLocker, 0);
        delay(3000);
        // Stored_uid = uid;
        currentState = COMPARE_UID;
      }
    }
  }
  break;

   
case COMPARE_UID: {
while(currentState== COMPARE_UID){
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Locker Occupied..");
            lcd.setCursor(0, 1);
            lcd.print("Scan To Open ");
            // currentState = ENTER_NUMBER;
            // lcdMessageDisplayed3 = true;
            delay(500);
        
    unsigned long endTime = millis(); // End time when leaving COMPARE_UID state
    unsigned long timeTaken = endTime - startTime;
    Serial.print("Time taken: ");
    Serial.println(timeTaken);
    bill = timeTaken/100;
    //currentState = ENTER_NUMBER;

        if(mfrc522.PICC_IsNewCardPresent()){
             if (mfrc522.PICC_ReadCardSerial()) {
                String uid = "";
                for (byte i = 0; i < mfrc522.uid.size; i++) {
                    uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
                    uid += String(mfrc522.uid.uidByte[i], HEX);
                }

                Serial.println("Scanned UID: " + uid);
                mfrc522.PICC_HaltA();  // Halt card
                // mfrc522.PCD_StopCrypto1(); // Stop encryption on the card
                delay(250);  // Wait before scanning for the next card
                
                // Compare scanned UID with stored UID
                if (uid == Stored_uid) {
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Correct Card..");
                    delay(500);
                    currentState = ENTER_NUMBER;
                } else {
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Invalid Card..");
                    delay(500);
                }
            } else {
                Serial.println("Error reading card serial.");
            }
        } else {
            Serial.println("No new card present.");
        }
        
        // mfrc522.PICC_HaltA();  // Halt card
        // rfidFlag=0;
        // mfrc522.PCD_StopCrypto1(); // Stop encryption on the card
        // delay(250);  // Wait before scanning for the next card
  
}
    break;
}
case ENTER_NUMBER:
      // analogWrite(selectedLocker, 255);
      // delay(1000);
      // analogWrite(selectedLocker, 0);
    if (!lcdMessageDisplayed3) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Your bill is:");
        lcd.print(bill); // Print the bill amount directly
        lcd.setCursor(0, 1);
        lcd.print("Enter Mpesa Number");
        delay(5000);
        lcdMessageDisplayed3 = true;
        delay(500);
    }
    // lcd.setCursor(1, 2);
    // Loop for entering Mpesa number
    while (currentState == ENTER_NUMBER) {

        customKey =getPressedKey();//customKeypad.getKey();
        if (customKey) {
            if (customKey=='*'&&data_count>0){data_count--;Data2[data_count] =' ';}//use * key as backspace
            else{Data2[data_count] = customKey;}
             lcd.setCursor(data_count, 2);
            lcd.print(Data2[data_count]);
          //  lcd.print(" "); // Add a space to separate entered digits
           // lcd.print(customKey); // Print the current key pressed
            if (customKey!='*'){data_count++;}
            // Check if the Mpesa number entry is complete
            if (data_count > Password_Length_2-1) {
                // Clear the LCD and display the confirmation message
                delay(3000);
                lcd.clear();
                // if (!strcmp(Data2, Master2)) {
                  
                    lcd.setCursor(0, 0);
                    lcd.print("Confirm Payment");
                    lcd.setCursor(0, 1);
                    lcd.print("Prompt...");
                    delay(3000);
                    clearData();
                    // Transition to the next state after confirmation
                    currentState = CHECK_PAYMENT_INFO;
                // }else{
                //     lcd.setCursor(0, 0);
                //     lcd.print("what next?");
                //     delay(3000);//ni ndirangu amead hii else statement..ater this point inafaa kufanya?
                // }
            }
        }
    }
    break;

 case CHECK_PAYMENT_INFO:
       data_count=0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Ent Payment det:");
      while (currentState == CHECK_PAYMENT_INFO) {
        // selectedLocker = ' ';
        Serial.println("Next state....CHECK_PAYMENT_INFO ");
        // lcd.clear();
        // lcd.setCursor(0, 0);
        // lcd.print("Ent Payment det:");
        Serial.print("datacount:");
        Serial.println(data_count);
        customKey =getPressedKey();//customKeypad.getKey();
        // Serial.print("Password_Length:");
        Serial.println("pass1");
        if (customKey) {
           Serial.println("pass2");
            if (customKey=='*'&&data_count>0){data_count--;Data[data_count] =' ';}//use * key as backspace
            else{
               Serial.println("passelse");
              Data[data_count] = customKey;
              Serial.println("passelse2");}
            lcd.setCursor(data_count, 2);
            Serial.println("passelse3");
            lcd.print(Data[data_count]);
             Serial.println("pass3");
           // lcd.print(" "); // Add a space to separate entered digits
            //lcd.print(customKey); // Print the current key pressed
            if (customKey!='*'){
               Serial.println("pass4");
              data_count++;
              Serial.println("passelse5555");}
            // Check if the Mpesa number entry is complete
        if (data_count == Password_Length -1) {
           Serial.print("datacount:");
        Serial.println(data_count);
         Serial.print("Password_Length:");
        Serial.println(Password_Length);
          lcd.clear();
          bool strcmp_flag=0;
          for(int a=0;;a++){
            if(a>=Password_Length){
              strcmp_flag=1;
              Serial.println("match");
              break;
            }
            if(Data[a]==Master[a]){
              continue;
            }else{
              Serial.println("mismatch!");
              strcmp_flag=0;
              break;
            }
            
          }
          if (strcmp_flag) {
             lcd.setCursor(0, 0);
            lcd.print("Payment Verified");
            delay(2000);
            // lcd.clear();
            lcd.setCursor(3, 1);
            lcd.print("Open Locker");
            analogWrite(selectedLocker, 255);
            delay(4000);
            analogWrite(selectedLocker, 0);
            currentState = FINISH;
            
          }
           else {
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
    
      // break;
    }
    break;
    case FINISH:
    {  
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("   THANK YOU ");
        lcd.setCursor(2, 1);
        lcd.print("WELCOME BACK");
        delay(3000);
       lcdMessageDisplayed = false;
       lcdMessageDisplayed2= false;
       lcdMessageDisplayed3 = false;

       clearData();
       currentState = WELCOME;
       break;
    }
    
    // default:
    //   currentState = SCAN_RFID;
    
  }
}

char getPressedKey() {
  while(1){
    for (byte row = 0; row < ROWS; row++) {
      pinMode(rowPins[row], OUTPUT);
      digitalWrite(rowPins[row], LOW);
      
      for (byte col = 0; col < COLS; col++) {
        pinMode(colPins[col], INPUT_PULLUP);
        
        if (digitalRead(colPins[col]) == LOW) {
          delay(50); // debounce delay
          if (digitalRead(colPins[col]) == LOW) {
            pinMode(rowPins[row], INPUT); // restore row pin state
            delay(200);//debounce time
            return hexaKeys[row][col];
          }
        }
        
        pinMode(colPins[col], OUTPUT);
        digitalWrite(colPins[col], HIGH);
      }
      
      pinMode(rowPins[row], INPUT); // restore row pin state
    }
    
  }
}