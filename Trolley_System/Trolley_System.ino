#include <Wire.h> 
// Include LCD display library for I2C
#include <LiquidCrystal_I2C.h>
// Include Keypad library

// Create LCD object
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
 
char input[12];
int count = 0;
const int State =5;
int takeInpin =6;
int takeOutpin =2;
int buzzer =4;

bool itemAdded1 = false; // Flags to track whether item is added
bool itemAdded2 = false;
bool itemAdded3 = false;
 
int ButtonState;
int p1 = 0, p2 = 0, p3 = 0, p4 = 0;
int c1 = 0, c2 = 0, c3 = 0, c4 = 0;
 
double total = 0;
int count_prod = 0;
String uid = "";
 
void setup ()
{
  
  Serial.begin(9600);
  pinMode(State, INPUT_PULLUP);
  pinMode(takeOutpin, OUTPUT);
  pinMode(takeInpin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  SPI.begin();
  mfrc522.PCD_Init();
  lcd.clear();
  lcd.backlight();
  lcd.init(); 
  lcd.begin(16, 2);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME TO");
  delay (2000);
  lcd.setCursor(3, 1);
  lcd.print("SUPER MARKET");
  delay (2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Add Items");
 
 
}
 
 
 
void loop()
{ 
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
 
  if (uid =="a2050e51" && ButtonState == 1  && !itemAdded1)
  {
    Serial.println("Butter Added ");
    lcd.setCursor(0, 0);
    lcd.print("Butter Added       ");
    lcd.setCursor(0, 1);
    lcd.print("Price : 10.00      ");
    itemAdded1 = true; // Set flag to true
    p1++;
    digitalWrite(4, HIGH);
    delay(1000);
    digitalWrite(4, LOW);
    total = total + 10.00;
    count_prod++;
    digitalWrite(4, LOW);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Total Price :");
 
      lcd.setCursor(0, 1);
      lcd.print(total);
 
  }
  else if (uid == "a2050e51" &&  ButtonState == 0 && itemAdded1)
  {
    if (p1 > 0)
    {
      Serial.println("Butter Removed ");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Butter Removed!!!        ");
      digitalWrite(4, HIGH);
      delay(1000);
      digitalWrite(4, LOW);
      itemAdded1 = false; // Set flag to true
      p1--;
      total = total - 10.00;
      count_prod--;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Total Price :");
 
      lcd.setCursor(0, 1);
      lcd.print(total);
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Not in cart!!!        ");
      digitalWrite(4, HIGH);
      delay(2000);
      digitalWrite(4, LOW);
      digitalWrite(4, HIGH);
      delay(1500);
      digitalWrite(4, LOW);
       digitalWrite(4, HIGH);
      delay(1000);
      digitalWrite(4, LOW);

      lcd.clear();
    }
  }
 
 
  if ( uid=="92d1e551" &&  ButtonState == 1  && !itemAdded2)
  { 
    Serial.println ("Milk Added ");
    lcd.setCursor(0, 0);
    lcd.print("Milk Added       ");
    lcd.setCursor(0, 1);
    lcd.print("Price : 20.00      ");
    p2++;
    digitalWrite(4, HIGH);

    delay(1000);
    digitalWrite(4, LOW);
    itemAdded2 = true; // Set flag to true
    total = total + 20.00;
    count_prod++;
     lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Total Bill :");
 
      lcd.setCursor(0, 1);
      lcd.print(total);
  }
 
  else if ( uid=="92d1e551" &&  ButtonState == 0  && itemAdded2)
  {
    if (p2 > 0)
    { Serial.println ("Milk Removed ");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Milk Removed!!!        ");
      digitalWrite(4, HIGH);

      delay(1000);
      digitalWrite(4, LOW);
      itemAdded2 = false; // Set flag to true
      p2--;
      total = total - 20.00;
      count_prod--;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Total Bill :");
      lcd.setCursor(0, 1);
      lcd.print(total);
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Not in cart!!!        ");
      digitalWrite(4, HIGH);

      delay(2000);
      digitalWrite(4, LOW);
      lcd.clear();
    }
  }
 
 
if ( uid == "9285cb51" &&  ButtonState == 1  && !itemAdded3)
  { 
    
    Serial.println("Tea Added ");
    lcd.setCursor(0, 0);
    lcd.print("Tea Added       ");
    lcd.setCursor(0, 1);
    lcd.print("Price : 25.00      ");
    p3++;
    digitalWrite(4, HIGH);

    delay(1000);
    digitalWrite(4, LOW);
    itemAdded3 = true; // Set flag to true
    total = total + 25.00;
    count_prod++;
     lcd.clear();
    

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Total Bill :");
 
      lcd.setCursor(0, 1);
      lcd.print(total);

 
  }
 
  else if ( uid== "9285cb51" && ButtonState == 0  && itemAdded3)
  {
    if (p3 > 0)
    { Serial.println("Tea Removed ");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Tea Removed!!!        ");
      digitalWrite(4, HIGH);

      delay(1000);
       digitalWrite(4, LOW);
       itemAdded3 = false; // Set flag to true

      p3--;
      total = total - 25.00;
      count_prod--;
      lcd.clear();
     
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Total Bill :");
      lcd.setCursor(0, 1);
      lcd.print(total);
      
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Not in cart!!!        ");
      digitalWrite(4, HIGH);
 
      delay(2000);
      digitalWrite(4, LOW);
      lcd.clear();
    }
  }
  uid = ""; // Reset UID after processing
}
