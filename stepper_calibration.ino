#include <FlashAsEEPROM.h>

const int stepPin = 9;  // Pin connected to the PWM signal (Step pin)
const int directionPin = 5; // Pin connected to the Direction pin

const int positionAddress = 0;     // EEPROM address to store the stepper position
const int calibrationFlagAddress = 10; // EEPROM address to store the calibration flag

int stepperPosition = 0; // Variable to store the current position of the stepper motor

void setup() {
  // Start serial communication to display the position and status
  Serial.begin(9600);
  
  // Initialize direction and step pins as outputs
  pinMode(directionPin, OUTPUT);
  pinMode(stepPin, OUTPUT);

  // Read position and calibration flag from EEPROM
  //stepperPosition = EEPROM.read(positionAddress);
  Serial.print("Current position: ");
  Serial.println(stepperPosition);
  Serial.println("Use '+' and '-' to adjust position, 'd' to save calibration.");
}

void loop() {
  if (Serial.available()) {
    char input = Serial.read(); // Read user input from serial monitor
    
    // Calibration logic
    if (input == '+') {
      stepperPosition++;
      Serial.print("Position increased: ");
      Serial.println(stepperPosition);
      digitalWrite(directionPin, HIGH);
      oneStep();
    }
    
    if (input == '-') {
      stepperPosition--;
      Serial.print("Position decreased: ");
      Serial.println(stepperPosition);
      digitalWrite(directionPin, LOW);
      oneStep();
    }
    
    if (input == 'd') {
      // Save the position to EEPROM and set the calibration flag
      EEPROM.write(positionAddress, stepperPosition);
      EEPROM.write(calibrationFlagAddress, 1); // Set calibration flag to true
      Serial.println("Calibration saved. Flag set.");
    }
  }
  delay(100);  // Small delay to debounce serial input
}

void oneStep()
{
  // pwm signal high
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(10);        // Hold for 100 microseconds (adjust as needed)
  digitalWrite(stepPin, LOW);    // Step low
}
