/**********************************************************************************************************************************************************************************************
AUTHOR:MUEMA
DATE: 6/6/2024
muemarevans@gmail.com
GitHub:MuemaXe 
AUTOMATIC LIQUID DISPENSER WITH RECORD TRACKING

Sensors: Flow Sensor--High Accuracy
        RTC Time Module
        SD Card Module
Actuators: DC Pump
          LCD 20x4 For Display
          KeyPad 4x4
Language:C++
---This code is too big to be uploaded on Arduino Uno or Nano-- Arduino Mega will work
**********************************************************************************************************************************************************************************************/
/*
SD Card Pin	ESP8266 NodeMCU	ESP8266 WeMos D1 Mini
GND	GND	GND
VCC	3V3 or VIN	3V3 or 5V
MISO	D6	D6
MOSI	D7	D7
SCK	D5	D5
CS	D8	D8


SD Card Pin	EPS32 (VSPI interface)	EPS32 (HSPI interface)
GND	GND	GND
VCC	V5 or 3V3	V5 or 3V3
MISO	19	12
MOSI	23	13
SCK	18	14
CS	5	15 

Micro SD | ESP32

MISO  | GPIO 19
MOSI | GPIO 23
SCK | GPIO 18
CS | GPIO 5
VCC | 5V
GND | GND

RTC DS3231 | ESP32
SDA | SDA (GPIO21)
SCL | SCL (GPIO22)
GND | GND
VCC | 3.3 V

Keypad configurations for esp32

byte pin_rows[ROW_NUM]      = {19, 18, 5, 17}; // GPIO19, GPIO18, GPIO5, GPIO17 connect to the row pins
byte pin_column[COLUMN_NUM] = {16, 4, 0, 2};   // GPIO16, GPIO4, GPIO0, GPIO2 connect to the column pins

byte pin_rows[ROW_NUM]      = {21, 19, 18, 5}; /*Initialized ESP32 Pins for Rows
byte pin_column[COLUMN_NUM] = {12, 13, 14, 15};   /*Initialized ESP32 Pins for
//https://linuxhint.com/interface-keypad-esp32-arduino/

byte rowPins[ROWS] = {13, 12, 14, 27}; /* connect to the row pinouts of the keypad 
byte colPins[COLS] = {26, 25, 33, 32}; /* connect to the column pinouts of the keypad 
//https://www.electronicwings.com/esp32/keypad-interfacing-with-esp32

//for Arduino uno

//byte rowPins[ROWS] =  {6,5,4,3}; //connect to the row pinouts of the keypad
//byte colPins[COLS] = {10,9,8,7};//connect to the column pinouts of the keypad
//*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <EEPROM.h>
#include <SD.h>
#include <SPI.h>
#include <RTClib.h>
#define _DISPLAY_TIME_SEC  1000
#define _PIN_FLOW_PULSE_IN  35

#define EEPROM_SIZE 512
const uint8_t  chipSelect = 5; // SD card CS pin

RTC_DS1307 rtc;
File dataFile;
const uint8_t  _RELAY_PIN = 15;
const uint8_t   RelayPump= 15;
//float time;
float Amount;
float price = 230;
float  PULSE;
const  byte ROWS = 4;
const  byte COLS = 4;
const uint8_t  Button_pin = 4;
float  _PULSE_THRESHOLD  = 0;
float dailyTotalLitres = 0;
float dailyTotalAmount = 0;
float TotalAmount = 0.1;
uint8_t  buttonState = LOW ;
char customKey;
uint8_t  calibration_length =6;
uint8_t  calibration;
uint8_t  Litres_Length = 4;
float Milli_Litres;

const uint8_t  debounceDelay = 50; // debounce time in milliseconds
//char currentFileName[30]={0};


volatile uint32_t unpulseCount = 0;   // volatile : RAM Write (Interrupt Fast Variable)
uint32_t undisplayCount = 0;   

uint32_t unlastDisplay = 0;


float totalOilPumped = 0.1;
const  byte Password_Length = 5;
char Data[Password_Length];
char newPassword[Password_Length];
char confirmPassword[Password_Length];
byte newPasswordCount = 0;
byte confirmPasswordCount = 0;
char Master[Password_Length] = "1234";
#define EEPROM_ADDRESS 0
#define EEPROM_PASSWORD_ADDRESS 10
#define EEPROM_TOTAL_OIL_ADDR  100
#define EEPROM_FULL_TOTAL_OIL_ADDR  110
#define EEPROM_PULSE  120
const byte Amount_Length = 4;
char AmountData[Amount_Length];

bool lcdMessageDisplayed;

static byte data_count = 0;
const byte  price_Lenth = 4;
char  priceData [price_Lenth];

static byte  priceCount = 0;

byte smile[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b10001,
  0b01110,
  0b00000,
  0b00000
};

char hexaKeys[ROWS][COLS] = {
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {26, 25, 33, 32}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {13, 12, 14, 27}; // connect to the column pinouts of the keypad

// byte rowPins[ROWS] =  {6,5,4,3}; //connect to the row pinouts of the keypad
// byte colPins[COLS] = {10,9,8,7};//connect to the column pinouts of the keypad
/* byte pin_rows[ROW_NUM]      = {19, 18, 5, 17}; // GPIO19, GPIO18, GPIO5, GPIO17 connect to the row pins
byte pin_column[COLUMN_NUM] = {16, 4, 0, 2};   // GPIO16, GPIO4, GPIO0, GPIO2 connect to the column pins
*/
char getPressedKey();//keypad function
//Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x27, 20, 4);

enum AtmState {
WELCOME,
SETTINGS,
ENTER_AMOUNT,
CALIBRATION,
SET_PRICE,
SET_PASSWORD,
STATISTICS,
PUMPING,
FINISH,
};

AtmState currentState = WELCOME;

void setup() {
Serial.begin(9600);

EEPROM.begin(EEPROM_SIZE);
lcd.backlight();
lcd.init();
lcd.begin(20, 4);
lcd.setCursor(0, 0);
lcd.print("SYSTEM SETUP.....");
lcd.createChar(1, smile);
pinMode(RelayPump, OUTPUT);
digitalWrite(RelayPump,0);
pinMode(_RELAY_PIN, OUTPUT);
digitalWrite(_RELAY_PIN, LOW);

pinMode(RelayPump, OUTPUT);
pinMode(Button_pin, INPUT_PULLUP);
digitalWrite(RelayPump, LOW);
pinMode(_PIN_FLOW_PULSE_IN, INPUT);
attachInterrupt(digitalPinToInterrupt(_PIN_FLOW_PULSE_IN), detectInterruptSignal, FALLING);
EEPROM.get(EEPROM_ADDRESS, price);
EEPROM.get(EEPROM_PASSWORD_ADDRESS, Master);

// TotalAmount = 0.001;
// EEPROM.put(EEPROM_FULL_TOTAL_OIL_ADDR, TotalAmount);
// PULSE = 14500;
// EEPROM.put( EEPROM_PULSE  , PULSE);
EEPROM.get(EEPROM_TOTAL_OIL_ADDR, totalOilPumped);
EEPROM.get(EEPROM_FULL_TOTAL_OIL_ADDR, TotalAmount);
EEPROM.get( EEPROM_PULSE  , PULSE);
//    // Initialize RTC
if (!rtc.begin()) {
   Serial.println("Couldn't find RTC");
 // while (1);
}
if (!rtc.isrunning()) {
  // Serial.println("RTC is NOT running!");
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}
  if (rtc.lostPower()) {
  Serial.println("RTC lost power, let's set the time!");
  // Following line sets the RTC to the date & time this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

// Print the current date and time
DateTime now = rtc.now();
Serial.print("Current Date and Time: ");
Serial.print(now.year(), DEC);
Serial.print('/');
Serial.print(now.month(), DEC);
Serial.print('/');
Serial.print(now.day(), DEC);
Serial.print(" ");
Serial.print(now.hour(), DEC);
Serial.print(':');
Serial.print(now.minute(), DEC);
Serial.print(':');
Serial.print(now.second(), DEC);
Serial.println();
Serial.print("Total Oil Pumped: ");
Serial.println(totalOilPumped);
Serial.println(TotalAmount);

if (!SD.begin(chipSelect)) {
  // Serial.println("SD card initialization failed!");
  return;
}
Serial.println("SD card is ready.");
  Create a file
File dataFile = SD.open("logs.txt", FILE_WRITE);

// Check if the file opened successfully
if (dataFile) {
  Serial.println("File opened successfully.");
  dataFile.close();  // Close the file
} else {
  Serial.println("Error opening file.");
}

  Serial.print("Total Oil Pumped: ");
  Serial.println(totalOilPumped);
}

void clearData() {
while (data_count != 0) {
  Data[--data_count] = 0;
}
data_count = 0;
// Clear data count for new password entry
  uint8_t newPasswordCount = 0;

// Clear data count for confirming new password entry
  uint8_t  confirmPasswordCount = 0;
uint8_t undisplayCount = 0;
// Clear data arrays for password entry
for (uint8_t  i = 0; i < Password_Length; i++) {
  Data[i] = ' ';
  newPassword[i] = ' ';
  confirmPassword[i] = ' ';
}

// Clear data array for price entry
for (uint8_t  i = 0; i < price_Lenth; i++) {
  priceData[i] = ' ';
}
// Reset price count
memset(priceData, 0, sizeof(priceData));
priceCount = 0;
}
void displayPulse() {
Serial.print("PULSE:");
Serial.println(undisplayCount);
Serial.print(", ");
Serial.println("Liter:");

  float fliter;
fliter = undisplayCount * 1/PULSE; // = * 0.00641
Serial.print(fliter, 3);
}

void detectInterruptSignal() {
unpulseCount++;
}

void detectinterruptReset() {
unpulseCount = 0;
undisplayCount = 0;
digitalWrite(_RELAY_PIN, LOW); // Ensure the relay is off when reset (assuming LOW deactivates the relay)
}
// String formatNumber(int number) {
//   if (number < 10) {
//     return "0" + String(number);
//   } else {
//     return String(number);
//   }

//}
// void pushbutton(){
//     // Read the state of the push button
//   buttonState = digitalRead(Button_pin);

//   // Print the button state to the serial monitor
//   if (buttonState == HIGH) {
//     Serial.println("Button Pressed");
//     currentState= ENTER_AMOUNT;
//     lcdMessageDisplayed = false;
//   } 
//  else {
//     Serial.println("Button Released");
//    }

//   // Small delay to debounce the button
//   delay(50);

// }
char getPressedKey() {
while(true){
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


void loop() {
    if (isEndOfDay()) {
      logEndOfDayTotals();
      dailyTotalLitres = 0; // Reset daily totals
      dailyTotalAmount = 0; // Reset daily totals
      buttonState = digitalRead(Button_pin);
      
      // Update currentFileName for new day
      //currentFileName = String(rtc.now().year()) + "-" + formatNumber(rtc.now().month()) + "-" + formatNumber(rtc.now().day()) + ".txt";
      //currentFileName = String(now.year()) + "-" + formatNumber(now.month()) + "-" + formatNumber(now.day()) + ".txt";
        //Serial.println("Current file name: " + currentFileName);
  }

//       String logMessage = String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + " " +
//                       String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()) + "\n";

//   // Open the file to write the log
//   File logFile = SD.open("logs.txt", FILE_WRITE);
//   if (logFile) {
//     logFile.print(logMessage);
//     logFile.close();
//     Serial.println("Log written to logs.txt");
//   } else {
//     Serial.println("Failed to open logs.txt for writing");
//   }
// }
// char customKey;
switch (currentState) {
  case WELCOME:
  {
    if (!lcdMessageDisplayed) {
      //  Serial.println("State Welcome");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PATA MAFUTA HAPA");
      lcd.setCursor(0, 1);
      lcd.print("1 litre =");
      lcd.print(price);
      lcd.print(" Ksh");
    lcd.setCursor(12, 3);
    lcd.print("Next-'#'");

      // lcd.setCursor(0, 3);
      // lcd.print("* -> Settings");


      lcdMessageDisplayed = true;
    }
  //  pushbutton();
//buttonState = digitalRead(Button_pin);

// Print the button state to the serial monitor
if (buttonState == HIGH) {
  // Serial.println("Button Pressed");
  currentState= ENTER_AMOUNT;
  lcdMessageDisplayed = false;
} 


// Print the button state to the serial monitor
  
      customKey = getPressedKey();
    if (customKey =='*')
    { 
      lcdMessageDisplayed = false;
      currentState = SETTINGS;
    }
    if (customKey =='#')
    { 
    lcdMessageDisplayed = false;
      currentState= ENTER_AMOUNT;
    }

  };
  case SETTINGS:
  {   
      customKey = getPressedKey();

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("A-> Calibration");
      lcd.setCursor(0, 1);
      lcd.print("B-> Set Price");
      lcd.setCursor(0, 2);
      lcd.print("C-> Set Password");
      lcd.setCursor(0, 3);
      lcd.print("D-> View Statitics");
      

      if(customKey =='A'){
        currentState = CALIBRATION;
      }

      if(customKey =='B'){
        currentState = SET_PRICE;
      }
      if(customKey =='C'){
        currentState = SET_PASSWORD;
      }
      if(customKey =='D'){
        currentState = STATISTICS;
      }
    
    break;

  }
  case CALIBRATION:
  {
    Calibration_Input();
    currentState = WELCOME;
    break;
  };
  
    case SET_PRICE:
    {
    // Serial.print("Setting Price");
    data_count = 0;
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Price Settings");
    lcd.setCursor(2, 1);
    lcd.print(" Enter Password ");
    lcd.setCursor(0, 2);
    lcd.print(" Password:");
    lcd.setCursor(0, 3);
    lcd.print("Delete-'*'");
    lcd.setCursor(12, 3);
    lcd.print("Done-'#'");
    
    while (currentState == SET_PRICE) {
      customKey = getPressedKey();
      if (customKey) {
        if (customKey == '*' && data_count > 0) {
          data_count--;
          Data[data_count] = ' ';
        } else if (customKey >= '0' && customKey <= '9' && data_count < Password_Length - 1) {
          Data[data_count] = customKey;
          data_count++;
        } else if (customKey == '#' && data_count == Password_Length-1) {
            // if(){}
          Data[data_count] = '\0';
          bool strcmp_flag = 0;
          for (int a=0;;a++) {
              if(a>=Password_Length){
            strcmp_flag=1;
            // Serial.println("match");
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
            strcmp_flag=0;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Proceed To Set");
            delay(2000);
            Set_Price();
            // Serial.print(price);
            // lcd.setCursor(0, 1);
            // lcd.print("Price Set:");
            // lcd.setCursor(0, 3);
            // lcd.print(price);
            delay(2000);
            clearData();
            lcd.clear();
            currentState = WELCOME;
          } else {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Password Incorrect");
            delay(3000);
            clearData();
            lcd.clear();
            lcd.setCursor(3, 0);
            lcd.print("Price Settings");
            lcd.setCursor(2, 1);
            lcd.print(" Enter Password ");
            lcd.setCursor(0, 2);
            lcd.print(" Password:");
            lcd.setCursor(0, 3);
            lcd.print("Delete-'*'");
            lcd.setCursor(12, 3);
            lcd.print("Done-'#'");
          }
          customKey = '\0'; // Reset customKey to avoid multiple reads
        }
        lcd.setCursor(10, 2);
        lcd.print(Data);
        // Serial.println(Data);
      }
    }
    break;

    };
case SET_PASSWORD:{
  //  Serial.print("Setting Password");
    lcd.clear();
      lcd.setCursor(1, 0);
    lcd.print(" Setting Password");
    delay(2000);
    clearData(); 
    changePassword();
    lcdMessageDisplayed = false;
    currentState = WELCOME;
    break;
};
case STATISTICS:{
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("STATISTICS");
    lcd.setCursor(0, 1);
    lcd.print("OIL RESERVE:");
    lcd.setCursor(13, 1);
    lcd.print(dailyTotalLitres);
    lcd.setCursor(18, 1);
    lcd.print("L");
    lcd.setCursor(0, 2);
    lcd.print("SALES:");

          // Retrieve the total oil pumped from EEPROM
    //float totalOilPumped = 0.0;
    EEPROM.get(EEPROM_TOTAL_OIL_ADDR, totalOilPumped);
    lcd.setCursor(7, 2);
    // lcd.print(totalOilPumped);
    lcd.print(TotalAmount);
    lcd.setCursor(11, 3);
    lcd.print("# -> EXIT");
    customKey = getPressedKey();
    if (customKey=='#'){
        currentState = WELCOME;

      }
    
break;
};
case ENTER_AMOUNT:{

// Serial.println("ENTER_AMOUNT");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ENTER AMOUNT (KSH)");
  lcd.setCursor(0, 1);
  lcd.print("AMOUNT :");
  lcd.setCursor(0, 3);
  lcd.print("Delete-'*'");
  lcd.setCursor(12, 3);
  lcd.print("Done-'#'");

while (currentState == ENTER_AMOUNT) {
  customKey = getPressedKey();
  if (customKey) {
    if (customKey >= '0' && customKey <= '9' && data_count < Amount_Length - 1) {
      Serial.println("INPUT KEY");
      Data[data_count] = customKey;
      // lcd.setCursor(9, 1);
      // lcd.print(Data[data_count]);
      data_count++;
    } else if (customKey == '*' && data_count > 0) {
      data_count--;
      Data[data_count] = ' ';
      // lcd.setCursor(data_count, 1);
      // lcd.print(Data[data_count]);
    }
      lcd.setCursor(8, 1);
      lcd.print(Data);
  }

  if (data_count >= 2 && data_count <= 4) {
    // lcd.setCursor(0, 0);
    // lcd.print("ENTER AMOUNT");
    // lcd.setCursor(data_count, 1);
    // lcd.print(Data[data_count - 1]); // Print the last entered digit        // if (data_count == Amount_Length - 1) {
    if (customKey == '#') {
      Amount = atoi(Data); // Convert char array to integer
      Serial.print("Amount: ");
      Serial.println(Amount);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Amount Input: ");
      lcd.setCursor(13, 0);
      lcd.print(Amount);
      lcd.setCursor(17, 0);
      lcd.print("Ksh");
      delay(2000);
      clearData();
      lcdMessageDisplayed = false;
      currentState = PUMPING;
    }
  }
}
break;
};
case PUMPING:{
  float litres= (Amount*1/price);
  float pumpedAmount = Amount; // Assume Amount is the current session's pumped amount

  // float totalOilPumped = 0.0;


while(currentState == PUMPING){


if (!lcdMessageDisplayed) {
      // Serial.println("Pumping Oil....");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pumping :");
      lcd.setCursor(9, 0);
      lcd.print(litres);
      lcd.setCursor(14, 0);
      lcd.print("Litres");

      lcdMessageDisplayed=true;
}

  float Cashing;

  if (millis() - unlastDisplay >= _DISPLAY_TIME_SEC) {
  unlastDisplay += _DISPLAY_TIME_SEC;

  undisplayCount = unpulseCount;
  //Cashing = unpulseCount++/76.9;///This is where the problrm was


  //flow display
    displayPulse();
      lcd.setCursor(0, 1);
      lcd.print("Amount: ");
      lcd.setCursor(8, 1);
    //  lcd.print(Cashing);
      lcd.setCursor(15, 1);
      lcd.print("Ksh");
        lcd.setCursor(0, 3);
      lcd.print("Pulse : ");
        lcd.setCursor(9, 3);
      lcd.print(PULSE);
      lcd.setCursor(14, 3);
        lcd.print(_PULSE_THRESHOLD);

  _PULSE_THRESHOLD = (PULSE * Amount/price);
  // Check if pulse count has reached the exact threshold
  if (undisplayCount < _PULSE_THRESHOLD) {
    digitalWrite(_RELAY_PIN, HIGH); // Turn off the relay (assuming LOW deactivates the relay)
  }
  if (undisplayCount >=_PULSE_THRESHOLD) {
    
  
    digitalWrite(_RELAY_PIN, LOW); // Turn on the relay (assuming HIGH activates the relay)
  //  currentState = FINISH;
  
    
    delay(5000); // Relay stays on for 5 seconds
    detectinterruptReset();
    EEPROM.get(EEPROM_TOTAL_OIL_ADDR, totalOilPumped);
    totalOilPumped += pumpedAmount;
    EEPROM.put(EEPROM_TOTAL_OIL_ADDR, totalOilPumped);
    EEPROM.commit();
    // Serial.println("Done Pumping Oil....");
    // logPumpingData(Amount, litres);
    Serial.print(TotalAmount);
      currentState = FINISH;
      lcdMessageDisplayed = false;
    }
  }

}
      

    break;
};
  case FINISH:
  
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   THANK YOU ");
    lcd.setCursor(2, 1);
    lcd.print("WELCOME BACK");
    lcd.setCursor(0, 2);
    for (int i = 0; i < 20; i++) {
      lcd.write(1);
    }
    delay(5000);
    clearData();
    currentState = WELCOME;
    break;
}
}


void Set_Price() {
lcd.clear();
lcd.setCursor(3, 0);
lcd.print("Price Settings");
lcd.setCursor(0, 1);
lcd.print("Enter new Oil Price");
lcd.setCursor(0, 2);
lcd.print("New Price:");
lcd.setCursor(0, 3);
lcd.print("Delete-'*'");
lcd.setCursor(12, 3);
lcd.print("Done-'#'");

while (true) {
  char Key = getPressedKey();
  if (Key) {
    if (Key >= '0' && Key <= '9' && priceCount < price_Lenth - 1) {
      // Serial.println("INPUT KEY");
      priceData[priceCount] = Key;
      // lcd.setCursor(priceCount, 1);
      // lcd.print(priceData[priceCount]);
      priceCount++;
    } else if (Key == '*' && priceCount > 0) {
      priceCount--;
      priceData[priceCount] = ' ';
      // lcd.setCursor(10, 1);
      lcd.print(' ');
    } else if (Key == '#' && priceCount >= 2 && priceCount <= 4) {
      priceData[priceCount] = '\0';
      price = atoi(priceData);
      // EEPROM.put(EEPROM_ADDRESS, price);
      // EEPROM.commit();

      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Price Settings");
      lcd.setCursor(0, 1);
      lcd.print("Price Set:");
      lcd.setCursor(12, 1);
      lcd.print(price);
      lcd.setCursor(1, 3);
      lcd.print("New Price Updated");
      delay(3000);
      break; // Exit the while loop
    }
    lcd.setCursor(10, 2);
    lcd.print(priceData);
  }
}
clearData(); // Clear data after setting the price
}

void changePassword() {
char newPassword[Password_Length];
char confirmPassword[Password_Length];
byte newPasswordCount = 0;
byte confirmPasswordCount = 0;
bool passwordMatched = false;
  data_count = 0;
lcd.clear();
lcd.setCursor(1, 0);
lcd.print("Setting Password");
lcd.setCursor(0, 1);
lcd.print("Enter Old Password:");
lcd.setCursor(0, 2);
lcd.print("Password:");
lcd.setCursor(0, 3);
lcd.print("Delete-'*'  Done-'#'");


// Serial.println("Starting password change process."); // Debug statement

while (true) {
  char key = getPressedKey();

  if (key) {
    // Serial.print("Key pressed: "); // Debug statement
    // Serial.println(key);

    if (key == '*' && data_count > 0) {
      data_count--;
      Data[data_count] = ' ';
      // Serial.print("Data after backspace: "); // Debug statement
      // Serial.println(Data);
    } else if (key >= '0' && key <= '9' && data_count < Password_Length - 1) {
      Data[data_count] = key;
      data_count++;
      // Serial.print("Data after key input: "); // Debug statement
      // Serial.println(Data);
    }

    lcd.setCursor(10, 2);
    lcd.print(Data);

    if (data_count == Password_Length - 1 && key == '#') {
      Data[data_count] = '\0';
      // Serial.print("Entered current password: "); // Debug statement
      // Serial.println(Data);

      if (strcmp(Data, Master) == 0) {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Correct  Password");
        // Serial.println("Current password correct."); // Debug statement
        delay(1500);
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Setting Password");
        lcd.setCursor(0, 1);
        lcd.print("Enter New Passord:");
        lcd.setCursor(0, 2);
        lcd.print("Password:");
        lcd.setCursor(0, 3);
        lcd.print("Delete-'*'  Done-'#'");
        clearData();
        data_count = 0;
        memset(newPassword, 0, Password_Length); // Clear newPassword array

        // Enter new password
        while (true) {
          key = getPressedKey();
          if (key) {
            // Serial.print("Key pressed for new password: "); // Debug statement
            // Serial.println(key);

            if (key == '*' && newPasswordCount > 0) {
              newPasswordCount--;
              newPassword[newPasswordCount] = ' ';
              // Serial.print("New password after backspace: "); // Debug statement
              // Serial.println(newPassword);
            } else if (key >= '0' && key <= '9' && newPasswordCount < Password_Length - 1) {
              newPassword[newPasswordCount] = key;
              newPasswordCount++;
              // Serial.print("New password after key input: "); // Debug statement
              // Serial.println(newPassword);
            }

            lcd.setCursor(10, 2);
            lcd.print(newPassword);

            if (newPasswordCount == Password_Length - 1 && key == '#') {
              // Serial.println("New password entry complete."); // Debug statement
              break;
            }
          }
        }

        newPassword[newPasswordCount] = '\0';

        while (true) {  // Loop for confirming password until matched
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Setting Password");
          lcd.setCursor(0, 1);
          lcd.print("Confirm New Password:");
          lcd.setCursor(0, 2);
          lcd.print("Password:");
          lcd.setCursor(0, 3);
          lcd.print("Delete-'*'  Done-'#'");
          clearData();
          confirmPasswordCount = 0;
          memset(confirmPassword, 0, Password_Length); // Clear confirmPassword array

          while (true) {
            key = getPressedKey();
            if (key) {
              // Serial.print("Key pressed for confirm password: "); // Debug statement
              // Serial.println(key);

              if (key == '*' && confirmPasswordCount > 0) {
                confirmPasswordCount--;
                confirmPassword[confirmPasswordCount] = ' ';
                // Serial.print("Confirm password after backspace: "); // Debug statement
                // Serial.println(confirmPassword);
              } else if (key >= '0' && key <= '9' && confirmPasswordCount < Password_Length - 1) {
                confirmPassword[confirmPasswordCount] = key;
                confirmPasswordCount++;
                // Serial.print("Confirm password after key input: "); // Debug statement
                // Serial.println(confirmPassword);
              }

              lcd.setCursor(10, 2);
              lcd.print(confirmPassword);

              if (confirmPasswordCount == Password_Length - 1 && key == '#') {
                // Serial.println("Confirm password entry complete."); // Debug statement
                break;
              }
            }
          }

          confirmPassword[confirmPasswordCount] = '\0';

          // Serial.print("New Password: "); // Debug statement
          // Serial.println(newPassword);
          // Serial.print("Confirm Password: "); // Debug statement
          // Serial.println(confirmPassword);

          if (strcmp(newPassword, confirmPassword) == 0) {
            passwordMatched = true;
            // Serial.println("Passwords match."); // Debug statement
            break;
          } else {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Passwords Mismatch!");
            // Serial.println("Passwords mismatch!"); // Debug statement
            delay(2000);
            // Loop back to ask for confirm password again
          }
        }

        break; // Exit the outer while loop once passwords are confirmed
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Wrong Password");
        // Serial.println("Wrong current password."); // Debug statement
        delay(2000);
        clearData();
        return;
      }
    }
  }
}

if (passwordMatched) {
  // for (byte i = 0; i < Password_Length - 1; i++) {
  //   Master[i] = newPassword[i];
  // }
  strcpy(Master, newPassword);
  EEPROM.put(EEPROM_PASSWORD_ADDRESS, Master);
 // EEPROM.put(EEPROM_PASSWORD_ADDRESS, Master);
  EEPROM.commit();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Password Changed");
  // Serial.println("Password changed successfully."); // Debug statement
  delay(2000);
}
clearData();
}
void logPumpingData(float amount, float litres) {
  File dataFile = SD.open("logs.txt", FILE_WRITE);
  if (dataFile) {
      // dataFile.print("Date/Time: ");
      // dataFile.print(rtc.now().year());
      // dataFile.print("-");
      // dataFile.print(formatNumber(rtc.now().month()));
      // dataFile.print("-");
      // dataFile.print(formatNumber(rtc.now().day()));
      // dataFile.print(" ");
      // dataFile.print(formatNumber(rtc.now().hour()));
      // dataFile.print(":");
      // dataFile.print(formatNumber(rtc.now().minute()));
      // dataFile.print(":");
      // dataFile.print(formatNumber(rtc.now().second()));
      // dataFile.print(", Amount: ");
      // dataFile.print(amount);
      // dataFile.print(", Litres: ");
      // dataFile.println(litres);
      dataFile.close();
  } else {
      // Serial.println("Error opening file for writing.");
  }
}

void logEndOfDayTotals() {
  File dataFile = SD.open("logs.txt", FILE_WRITE);
  if (dataFile) {
      // dataFile.println("End of Day Totals:");
      // dataFile.print("Total Amount: ");
      // dataFile.println(dailyTotalAmount);
      // dataFile.print("Total Litres: ");
      // dataFile.println(dailyTotalLitres);
      dataFile.close();
  } else {
      // Serial.println("Error opening file for writing.");
  }
  
  // Print on serial monitor
  // Serial.println("End of Day Totals:");
  // Serial.print("Total Amount: ");
  // Serial.println(dailyTotalAmount);
  // Serial.print("Total Litres: ");
  // Serial.println(dailyTotalLitres);
}
bool isEndOfDay() {
  DateTime now = rtc.now();
  if (now.hour() == 23 && now.minute() == 59) {
      return true;
  }
  return false;
}
void Calibration_Input(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ENTER PULSE");
  lcd.setCursor(0, 1);
  lcd.print("5000-10000");
  lcd.setCursor(0, 3);
  lcd.print("Delete-'*'");
  lcd.setCursor(12, 3);
  lcd.print("Done-'#'");

while (1) {
  customKey = getPressedKey();
  if (customKey) {
    if (customKey >= '0' && customKey <= '9' && data_count < calibration_length - 1) {
      // Serial.println("INPUT KEY");
      Data[data_count] = customKey;
      // lcd.setCursor(9, 1);
      // lcd.print(Data[data_count]);
      data_count++;
    } else if (customKey == '*' && data_count > 0) {
      data_count--;
      Data[data_count] = ' ';
      // lcd.setCursor(data_count, 1);
      // lcd.print(Data[data_count]);
    }
      lcd.setCursor(0, 2);
      lcd.print(Data);
  }

  if (data_count >= 3 && data_count <= 5) {
    // lcd.setCursor(0, 0);
    // lcd.print("ENTER AMOUNT");
    // lcd.setCursor(data_count, 1);
    // lcd.print(Data[data_count - 1]); // Print the last entered digit        // if (data_count == Amount_Length - 1) {
    if (customKey == '#') {
      calibration = atoi(Data); // Convert char array to integer
      // Serial.print("calibration: ");
      // Serial.println(calibration);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pulse Input: ");
      lcd.setCursor(13, 0);
      lcd.print(calibration);

      delay(2000);
      clearData();
      Calibration_Pump();

    }
  }
}
}


void Calibration_Pump(){

  while(1){
  if (millis() - unlastDisplay >= _DISPLAY_TIME_SEC) {
  unlastDisplay += _DISPLAY_TIME_SEC;

  undisplayCount = unpulseCount;

  // Check if pulse count has reached the exact threshold
  if (undisplayCount < calibration) {
    digitalWrite(_RELAY_PIN, HIGH); // Turn off the relay (assuming LOW deactivates the relay)
  }
  if (undisplayCount >=calibration) {
    
    // lcd.setCursor(0, 1);
    //   lcd.print("Amount: ");
    //   lcd.setCursor(8, 1);
    //   lcd.print(Cashing);
    digitalWrite(_RELAY_PIN, LOW); // Turn on the relay (assuming HIGH activates the relay)
  //  currentState = FINISH;
    delay(5000); // Relay stays on for 5 seconds
    //undisplayCount = 0; // Reset pulse count
    //  Serial.println("Done Pumping Oil....");
      detectinterruptReset();
      Litres_Measured();
    }
  }
  }
}
void Litres_Measured(){
    lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ENTER Millilitres  ");
  lcd.setCursor(5, 1);
  lcd.print("  Measured ");
  lcd.setCursor(0, 3);
  lcd.print("Delete-'*'");
  lcd.setCursor(12, 3);
  lcd.print("Done-'#'");

  while (1) {
  customKey = getPressedKey();
  if (customKey) {
    if (customKey >= '0' && customKey <= '9' && data_count < Litres_Length - 1) {
      // Serial.println("INPUT KEY");
      Data[data_count] = customKey;
      // lcd.setCursor(9, 1);
      // lcd.print(Data[data_count]);
      data_count++;
    } else if (customKey == '*' && data_count > 0) {
      data_count--;
      Data[data_count] = ' ';
      // lcd.setCursor(data_count, 1);
      // lcd.print(Data[data_count]);
    }
      lcd.setCursor(0, 2);
      lcd.print(Data);
  }

  if (data_count >= 2 && data_count <= 5 ){
    // lcd.setCursor(0, 0);
    // lcd.print("ENTER AMOUNT");
    // lcd.setCursor(data_count, 1);
    // lcd.print(Data[data_count - 1]); // Print the last entered digit        // if (data_count == Amount_Length - 1) {
    if (customKey == '#') {
      Milli_Litres = atoi(Data); // Convert char array to integer
      // Serial.print("Milli_Litres: ");
      // Serial.println(Milli_Litres);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Milli_Litres Input: ");
      lcd.setCursor(13, 1);
      lcd.print(Milli_Litres);

      delay(2000);
      clearData();
      calculate();

    }
  }

}
}
void calculate(){
lcd.clear();
PULSE = 1000*calibration/Milli_Litres;
      lcd.setCursor(0, 0);
      lcd.print("Pulse : ");
        lcd.setCursor(9, 0);
      lcd.print(PULSE);
      
      EEPROM.put(EEPROM_PULSE, PULSE);
      EEPROM.commit();
      lcdMessageDisplayed = false;
      currentState = WELCOME;

}

