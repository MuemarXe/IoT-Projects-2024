const int buttonPin = 5; // Define the pin number for the push button
int buttonState;    // Variable to store the state of the button
int takeInpin =6;
int takeOutpin =2;
void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Set the button pin as input
  Serial.begin(9600);
  pinMode(takeOutpin, OUTPUT);
  pinMode(takeInpin, OUTPUT);// Initialize serial communication
}

void loop() {
  buttonState = digitalRead(buttonPin); // Read the state of the button
  
  // Print the button state to the serial monitor
  Serial.print("Button state: ");
  Serial.println(buttonState);
  if( buttonState==1){
    digitalWrite(takeInpin,HIGH);
    digitalWrite(takeOutpin, LOW);
    
    }
   if( buttonState==0){ 
    digitalWrite(takeOutpin, HIGH);
    digitalWrite(takeInpin,LOW);
    }
  
  delay(100); // Delay for stability
}
