
// Include Arduino Wire library for I2C
#include <Wire.h> 
// Include LCD display library for I2C
#include <LiquidCrystal_I2C.h>
// Include Keypad library
#include <Keypad.h>
 
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
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 11};
 
// Create keypad object
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
 
// Create LCD object
LiquidCrystal_I2C lcd(0x27, 20, 4);
 
void setup(){
  // Setup LCD with backlight and initialize
  lcd.backlight();
  lcd.init(); 
  lcd.begin(20, 4);
  
}
void loop(){
  // Get key value if pressed
  char customKey = customKeypad.getKey();
  
  if (customKey){
    // Clear LCD display
    lcd.clear();
    
    // Check if the pressed key is 'A'
    if (customKey == 'A') {
      lcd.setCursor(0, 0); 
      lcd.print("Well code!");
    } else {
      // If the pressed key is not 'A', display the pressed key
      lcd.setCursor(0, 0); 
      lcd.print(customKey);
    }
  }
}
