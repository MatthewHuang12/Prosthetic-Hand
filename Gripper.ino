#include <Servo.h>

const int servoPin = 9;
const int buttonPin = 2;
const int ledPin = 13;
const int emgPin = A0;

Servo handServo;
const int OPEN_POSITION = 0;
const int CLOSED_POSITION = 180;
int currentPosition = OPEN_POSITION;


bool handClosed = false;           
bool lastButtonState = HIGH;      
bool currentButtonState = HIGH;    
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; 

void setup() {
  Serial.begin(9600);
  
  handServo.attach(servoPin);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  
  handServo.write(OPEN_POSITION);
  handClosed = false;
  digitalWrite(ledPin, LOW);
  
  Serial.println("Toggle Button Servo Controller Ready");
  Serial.println("Click button to toggle hand open/closed");
  Serial.println("Hand starts in OPEN position");
}

void loop() {
  int buttonReading = digitalRead(buttonPin);
  
  if (buttonReading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    
    if (buttonReading != currentButtonState) {
      currentButtonState = buttonReading;
      
 
      if (currentButtonState == LOW) {
        toggleHandState();  
      }
    }
  }
  
  lastButtonState = buttonReading;

  int emgValue = analogRead(emgPin);
  if (emgValue > 512) {
    if (!handClosed) {
      closeHand();
    }
  } else {
    if (handClosed) {
      openHand();
    }
  }
  
  digitalWrite(ledPin, handClosed);
  
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 500) {
    Serial.print("Hand State: ");
    Serial.print(handClosed ? "CLOSED" : "OPEN");
    Serial.print(" | Position: ");
    Serial.print(currentPosition);
    Serial.print("degrees |");
    Serial.print(" EMG Value: ");
    Serial.println(emgValue);
    lastPrint = millis();
  }
  
  delay(10); 
}

void toggleHandState() {
  if (handClosed) {
    openHand();
  } else {
    closeHand();
  }
}

void closeHand() {
  Serial.println("Button pressed - CLOSING hand...");
  
  while (currentPosition < CLOSED_POSITION) {
    currentPosition += 5;
    if (currentPosition > CLOSED_POSITION) {
      currentPosition = CLOSED_POSITION;
    }
    handServo.write(currentPosition);
    delay(30); 
  }
  
  handClosed = true;
  digitalWrite(ledPin, HIGH);
  Serial.println("Hand is now CLOSED");
}

void openHand() {
  Serial.println("Button pressed - OPENING hand...");

  while (currentPosition > OPEN_POSITION) {
    currentPosition -= 5;
    if (currentPosition < OPEN_POSITION) {
      currentPosition = OPEN_POSITION;
    }
    handServo.write(currentPosition);
    delay(30);
  }
  
  handClosed = false;
  digitalWrite(ledPin, LOW);
  Serial.println("Hand is now OPEN");
}