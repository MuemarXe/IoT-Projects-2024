#include <LiquidCrystal_I2C.h>
#define Mode_Button 5
#define IR1 6
#define IR2 7
#define ControlLed 8
#define timeOut 500

LiquidCrystal_I2C lcd(0x27, 16, 2);
 int counter =0;
long int cTime = 0;
int buzzer=4;

void Init_LCD(){
  lcd.init();       // Initialize the lcd
  lcd.backlight();  // Turn on LCD's led
  lcd.clear();      // Clear LCD screen
}

void Init_IR(){
  pinMode(IR1,INPUT_PULLUP); // Using internal resistor PULLUP
  pinMode(IR2,INPUT_PULLUP); // Using internal resistor PULLUP
  pinMode(buzzer, OUTPUT);
}


void LCD_display(){ 
  // Display information

   
  lcd.setCursor(0,0);
  lcd.print("People inside:");
  lcd.print(counter);  
}


void setup() {
  Serial.begin(9600); 

  // Init the system
  Init_LCD();
  Init_IR();
  cTime = millis();

}

void loop() {
  LCD_display();
  Serial.println(counter);
cTime = millis();
if(digitalRead(IR1) == 0){
  while(1)
  {
    if(digitalRead(IR2) == 0)
    {
      counter++;
      break; 
    }
    if (millis() - cTime >= timeOut)
    {
      break;
    }
  }
    lcd.clear();
  LCD_display();
}
else if(digitalRead(IR2) == 0){
 while(1)
  {
    if(digitalRead(IR1) == 0)
    {
         if(counter != 0)
            {
              counter--;  
            }
      break; 
    }
    if (millis() - cTime >= timeOut)
    {
      break;
    }
  
  }
  lcd.clear();
  LCD_display(); 
} 
while( digitalRead(IR1) == 0 && digitalRead(IR2) == 0);
while(counter>=14){
 // lcd.clear();
 digitalWrite(buzzer,HIGH);
 delay(3000);
 pinMode(buzzer,LOW);
 LCD_display();
  lcd.setCursor(0,1);
  lcd.print("matatu full");
  break;
  }
  

}
