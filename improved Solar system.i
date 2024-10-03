#include <Arduino.h>
#include <LiquidCrystal.h>

#define IN1  10
#define IN2  9
#define IN3  8
#define IN4  7
const int ACS712Pin = A7;
const int LDRPin = A2;
const int VoltageSensorPin = A1;
const float referenceVoltage = 5.0;

const int ledgreen = 14;
const int ledred = 15;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int stepIndex = 0;
boolean direction = true;
int stepsToRotate = 1024;
const int threshold = 500;
unsigned long previousMillis = 0;
const long interval = 1000; // Interval for LED blinking
bool ledState = false;
int messagePosition = 0;

String message1 = "Welcome! Charge control solar system! Sunshine is less. The motor tries to change the position of the solar panel!";
String message2 = "Welcome! Charge control solar system in CHARGING MODE!";
bool isScrolling = false;

void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ledgreen, OUTPUT);
  pinMode(ledred, OUTPUT);
  lcd.begin(20, 4);
}

void loop() {
  int ldrValue = analogRead(LDRPin);
  Serial.println(ldrValue);

  // Display values on LCD
  lcd.clear();
  displaySensorValues();

  if (ldrValue < threshold) {
    if (!isScrolling) {
      rotateMotor(stepsToRotate);
      isScrolling = false;
    }
    

//String message = "Welcome! Charge control solar system! Sunshine is less. The motor tries to change the position of the solar panel!";
    
    
  
  digitalWrite(ledgreen, LOW);
    // Scroll message
    scrollMessage();
 digitalWrite(ledred, HIGH);
    delay(700);
    digitalWrite(ledred, LOW);
    // Blink green LED
    //blinkLED(ledgreen);
  } else {
    
     digitalWrite(ledred, LOW);
      scrollMessage1();
    digitalWrite(ledgreen, HIGH);
    delay(700);
    digitalWrite(ledgreen, LOW);
   
  }

  // Check current and display charging message
  int voltageSensorValue = analogRead(VoltageSensorPin);
  float measuredVoltage = (voltageSensorValue / 1023.0) * referenceVoltage * 5;
  if (measuredVoltage > 4) {
    blinkLED(ledred);
  }

 
  
  
 // if (current > 0) {
   // scrollMessage();
 // }
}

void displaySensorValues() {
  int voltageSensorValue = analogRead(VoltageSensorPin);
  float measuredVoltage = (voltageSensorValue / 1023.0) * referenceVoltage * 5;
  int sensorValue = analogRead(ACS712Pin);
  float current = (sensorValue / 1023.0) * referenceVoltage  ;
    float current1 = current-0.31;
Serial.print(sensorValue);
  lcd.setCursor(0, 0);
  lcd.print("Line Volt: ");
  lcd.print(measuredVoltage);
  lcd.print("V");

  lcd.setCursor(0, 1);
  lcd.print("Motor Current:");
  lcd.print(current1);
  lcd.print("A");

  float lightIntensity = (analogRead(LDRPin) / 1023.0) * referenceVoltage;
  lcd.setCursor(0, 2);
  lcd.print("Light Int: ");
  lcd.print(lightIntensity);
  lcd.print(" cd");
}

void scrollMessage() {
  lcd.setCursor(0, 3);
    lcd.print("LOW LIGHT,TURN PANNEL");
    
}

void scrollMessage1() {
  lcd.setCursor(0, 3);
    lcd.print("CHARGING MODE");
}
void blinkLED(int ledPin) {
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    ledState = !ledState; // Toggle LED state
    digitalWrite(ledPin, ledState ? HIGH : LOW);
  }
}

void rotateMotor(int steps) {
  direction = true;
  for (int i = 0; i < steps; i++) {
    stepper(1);
    delay(2); // Adjust delay for smoother rotation
  }
}

void stepper(int stepCount) {
  for (int x = 0; x < stepCount; x++) {
    switch (stepIndex) {
    case 0: digitalWrite(IN1, LOW); digitalWrite(IN2, LOW); digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); break;
            case 1: digitalWrite(IN1, LOW); digitalWrite(IN2, LOW); digitalWrite(IN3, HIGH); digitalWrite(IN4, HIGH); break;
            case 2: digitalWrite(IN1, LOW); digitalWrite(IN2, LOW); digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); break;
            case 3: digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); break;
            case 4: digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); digitalWrite(IN3, LOW); digitalWrite(IN4, LOW); break;
            case 5: digitalWrite(IN1, HIGH); digitalWrite(IN2, HIGH); digitalWrite(IN3, LOW); digitalWrite(IN4, LOW); break;
            case 6: digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); digitalWrite(IN3, LOW); digitalWrite(IN4, LOW); break;
            case 7: digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); break;
            default: digitalWrite(IN1, LOW); digitalWrite(IN2, LOW); digitalWrite(IN3, LOW); digitalWrite(IN4, LOW); break;
            
    }
    updateDirection();
  }
}

void updateDirection() {
  if (direction) {
    stepIndex++;
    
  } else {
    stepIndex--;
  }

  if (stepIndex > 7) {
    stepIndex = 0;
  }
  if (stepIndex < 0) {
    stepIndex = 7;
  }
}
