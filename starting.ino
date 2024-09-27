#include <Arduino.h>
#include <LiquidCrystal.h>

#define IN1  10
#define IN2  9
#define IN3  8
#define IN4  7
const int ACS712Pin = A0;
const int LDRPin = A2;
const int VoltageSensorPin = A1; // Pin for the voltage sensor

const float referenceVoltage = 5.0;

int stepIndex = 0;
boolean direction = true;
unsigned long lastTime = 0;
unsigned long currentTime = 0;
unsigned long elapsedTime = 0;
int remainingSteps = 4095;
// Initialize the LCD (RS, EN, D4, D5, D6, D7)
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);




void setup()
{
    Serial.begin(115200);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    lcd.begin(20, 4); // Set up the LCD's number of columns and rows
   // lcd.backlight();   // Enable backlight if available
}

void loop()
{
    // Light intensity measurement
    int ldrValue = analogRead(LDRPin);
    float lightIntensity = (ldrValue / 1023.0) * referenceVoltage;

    // Voltage measurement
    int voltageSensorValue = analogRead(VoltageSensorPin);
    float measuredVoltage = (voltageSensorValue / 1023.0) * referenceVoltage * 5; // Adjust multiplier based on sensor specs

    
    // Read the current from ACS712
    int sensorValue = analogRead(ACS712Pin);
    float current = (sensorValue / 1023.0) * referenceVoltage * 5;

    // Position calculation
    int position = 4095 - remainingSteps;

    // Display values on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Volt: ");
    lcd.print(measuredVoltage);
    lcd.print("V");

    lcd.setCursor(0, 1);
    lcd.print("Current: ");
    lcd.print(current);
    lcd.print("mA");

    lcd.setCursor(0, 2);
    lcd.print("Position: ");
    lcd.print(position);
    
    lcd.setCursor(0, 3);
    lcd.print("Light: ");
    lcd.print(lightIntensity);
    lcd.print(" V");

    // Log values to Serial Monitor
    Serial.print("Position: ");
    Serial.println(position);
    Serial.print("Light Intensity: ");
    Serial.print(lightIntensity);
    Serial.println(" V");
    Serial.print("Measured Voltage: ");
    Serial.println(measuredVoltage);
    Serial.println(" V");

    delay(2000); // Display for 2 seconds

    direction = !direction;
    remainingSteps = 4095;

    while (remainingSteps > 0)
    {
        currentTime = micros();
        if (currentTime - lastTime >= 1000)
        {
            stepper(1);
            elapsedTime += micros() - lastTime;
            lastTime = micros();
            remainingSteps--;
        }
    }
}


void stepper(int stepCount)
{
    for (int x = 0; x < stepCount; x++)
    {
        switch (stepIndex)
        {
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

void updateDirection() 
{
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
