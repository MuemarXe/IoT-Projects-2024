/*#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  Wire.begin();
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 2);
  display.print("Hello, DroneBot!");
  display.display();
}

void loop() {
  // Your loop code here (if needed)
  delay(1000); // Add a delay if necessary
}


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  Wire.begin();
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("Hello, DroneBot!");
  display.display();
}

void loop() {
  display.clearDisplay();
  
  // Display sentence 1
  display.setCursor(0, 0);
  display.print("Sentence 1");

  // Display sentence 2
  display.setCursor(0, 10);
  display.print("Sentence 2");

  // Display sentence 3
  display.setCursor(0, 20);
  display.print("Sentence 3");

  // Display sentence 4
  display.setCursor(0, 30);
  display.print("Sentence 4");

  display.display();
  
  delay(2000); // Add a delay to control refresh rate
}*/


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

const char sentences[][20] = {"Sentence 1", "Sentence 2", "Sentence 3", "Sentence 4"};
const int sentenceCount = 4;
const int fontSize = 2;
const int scrollDelay = 500; // Adjust scrolling speed

void setup() {
  Wire.begin();
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(fontSize);
  display.setCursor(0, 0);
  display.print("Hello, DroneBot!");
  display.display();
}

void loop() {
  for (int i = 0; i < sentenceCount; ++i) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(sentences[i]);
    display.display();
    delay(scrollDelay);
  }
}
