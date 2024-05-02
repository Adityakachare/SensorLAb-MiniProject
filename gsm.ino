const int buttonPin = 2;     
int buttonState = 0;
int lastButtonState = 0;

#include <SoftwareSerial.h>

#define rxPin 2
#define txPin 3
SoftwareSerial sim800L(rxPin, txPin); 

void setup() {
  // Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  // Begin serial communication with Arduino and SIM800L
  sim800L.begin(9600);

  Serial.println("Initializing...");
  pinMode(buttonPin, INPUT);

  // Initialize the last known state of the button
  lastButtonState = digitalRead(buttonPin);
}

void send_sms() {
  sim800L.println("Sending Message please wait....");
  sim800L.println("AT+CMGF=1");
  sim800L.print("AT+CMGS=\"+919224303446\"\r");
  waitForResponse();
  
  sim800L.print("SOS. PLEASE SEND HELP !!!");
  sim800L.write(0x1A);  // End AT command with a Ctrl-Z
  waitForResponse();
  Serial.println("Message sent successfully");
}

void make_call() {
  sim800L.println("ATD+919224303446;");
  waitForResponse();
}

void waitForResponse() {
  delay(1000);
  while (sim800L.available()) {
    Serial.println(sim800L.readString());
  }
  sim800L.read(); // Clear the buffer after reading
}

void loop() {
  buttonState = digitalRead(buttonPin);
  
  // Check if the button state changed from not pressed to pressed
  if (buttonState == HIGH && lastButtonState == LOW) {
    send_sms();
    Serial.println("MESSAGE SENT SUCCESSFULLY.");
    delay(500);  // Debounce delay
  } else if (buttonState == LOW) {
    Serial.println("Waiting for button press...");
  }

  // Update the last button state
  lastButtonState = buttonState;
}
