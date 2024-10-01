#include <Arduino.h>
#include <LiquidCrystal.h>
// Define motor control pins
const int motorPin1 = 10; // Pin 1 connected to IN1 on motor driver
const int motorPin2 = 9; // Pin 2 connected to IN2 on motor driver
const int motorPin3 = 8; // Pin 3 connected to IN3 on motor driver
const int motorPin4 = 7; // Pin 4 connected to IN4 on motor driver

const int ACS712Pin = A0;
const int LDRPin = A2;
const int VoltageSensorPin = A1; // Pin for the voltage sensor

const float referenceVoltage = 5.0;
const int threshold = 500; // Threshold for light intensity
// Initialize the LCD (RS, EN, D4, D5, D6, D7)
const int ledgreen = 14;
const int ledred =15;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // initialize lcd screen pin out
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void setup() {
  lcd.begin(20, 4);
  // Set motor control pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(ledgreen, OUTPUT);
  pinMode(ledred, OUTPUT);
  Serial.begin(9600); // Start serial communication for debugging
}

void loop() {
  int ldrValue = analogRead(LDRPin); // Read the value from the LDR
  Serial.println(ldrValue); // Print the LDR value for debugging

  if (ldrValue < threshold) {

    
    // Rotate the stepper motor clockwise
    stepMotor(1); // Step the motor
    digitalWrite(ledred, HIGH);
    delay(200);
    digitalWrite(ledred, LOW);
  } 

  delay(200); // Wait before the next reading

 
    float lightIntensity = (ldrValue / 1023.0) * referenceVoltage;

    // Voltage measurement
    int voltageSensorValue = analogRead(VoltageSensorPin);
    float measuredVoltage = (voltageSensorValue / 1023.0) * referenceVoltage * 5; // Adjust multiplier based on sensor specs

    
    // Read the current from ACS712
    int sensorValue = analogRead(ACS712Pin);
    float current = (sensorValue / 1023.0) * referenceVoltage * 5;

    

    // Display values on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Line Volt: ");
    lcd.print(measuredVoltage);
    lcd.print("V");

    lcd.setCursor(0, 1);
    lcd.print("Ch Current: ");
    lcd.print(current);
    lcd.print("mA");

   
    
    lcd.setCursor(0, 2);
    lcd.print("Light Int: ");
    lcd.print(lightIntensity);
    lcd.print(" V");

   
    Serial.print("Light Int ");
    Serial.print(lightIntensity);
    Serial.println(" V");
    Serial.print("Measured Voltage: ");
    Serial.println(measuredVoltage);
    Serial.println(" V");

    delay(2000); // Display for 2 seconds
if (current > 0){
  digitalWrite(ledgreen, HIGH);
  delay(1000);
  digitalWrite(ledgreen, LOW);

  // The message to be displayed
  String message = "Welcome! Charge control solar system in CHARGING MODE!";

  // Clear the LCD before starting
  

  // Scroll the message across the LCD
  for (int position = 0; position < message.length() + 20; position++) {
    // Print the message with offset
     lcd.setCursor(0,3); // Start at the beginning of the first row
    lcd.print(message.substring(position, position + 20)); // Display 10 characters

    delay(250); // Adjust delay for speed of scrolling
  }
  }
  if (measuredVoltage > 4){
  digitalWrite(ledred, HIGH);
  delay(1000);
  digitalWrite(ledred, LOW);
  
  }
}


// Function to control the stepper motor
void stepMotor(int direction) {
  // Define the sequence for stepping
  if (direction == 1) { // Clockwise
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, LOW);
    delay(5); // Wait for the motor to step

    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    delay(5);

    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, HIGH);
    delay(5);

    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, HIGH);
    delay(5);

    digitalWrite(motorPin4, LOW);
  } else if (direction == -1) { // Counterclockwise
    digitalWrite(motorPin4, HIGH);
    delay(5);

    digitalWrite(motorPin4, LOW);
    digitalWrite(motorPin3, HIGH);
    delay(5);

    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin2, HIGH);
    delay(5);

    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin1, HIGH);
    delay(5);

    digitalWrite(motorPin1, LOW);
  }
  
}
