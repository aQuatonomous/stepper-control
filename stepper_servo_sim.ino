#include <EEPROM.h>

// Pins
const int pwmIn = 10;
const int stepPin = 9;
const int directionPin = 5;

// Constants
const int pwmToSteps = 1;
const int positionAddress = 0;     // EEPROM address to store the stepper position
const int calibrationFlagAddress = 10; // EEPROM address to store the calibration flag

// Global Variables
int stepperPosition = 0; // Variable to store the current position of the stepper motor
bool calibrationFlag = false; // Calibration flag
int desiredPosition = 0;

void setup() {
  Serial.begin(9600);

  // put your setup code here, to run once:
  pinMode(stepPin, OUTPUT);
  pinMode(pwmIn, INPUT);

  // Read position and calibration flag from EEPROM
  stepperPosition = EEPROM.read(positionAddress);
  calibrationFlag = EEPROM.read(calibrationFlagAddress);

  // Check if calibration has occurred
  if (calibrationFlag) {
    // If calibrated, set position to 50 and reset the flag
    stepperPosition = 50; 
    EEPROM.write(positionAddress, stepperPosition); // Save new position
    EEPROM.write(calibrationFlagAddress, 0); // Clear calibration flag
    Serial.println("Calibration detected. Setting position to 50.");
  } else {
    Serial.print("Position read from EEPROM: ");
    Serial.println(stepperPosition);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int desiredPosition = getDutyCycle();

  // print out the value you read:
  Serial.println(desiredPosition);
  // delay in between reads for stability
  delay(10);

  // ratio is 1:1 with duty cycle
  int numberOfSteps;
  int turnDirection = 1;
  if(currentPosition >= desiredPosition)
  {
    turnDirection = -1;
    numberOfsteps = currentPosition - desiredPosition;
    digitalWrite(directionPin, LOW);
  }
  else
  {
    turnDirection = 1;
    numberOfSteps = desiredPosition - currentPosition;
    digitalWrite(directionPin, HIGH);
  }

  for(int i=0;i<numberOfSteps;i++)
  {
    oneStep();
    stepperPosition += turnDirection; // either -1 or +1
    EEPROM.write(positionAddress, stepperPosition); // Save updated position to EEPROM
    Serial.print("Stepper Position: ");
    Serial.println(stepperPosition);
  }
}

unsigned long getDutyCycle()
{
  unsigned long highTime = pulseIn(pwmPin, HIGH);  // Time the signal is HIGH
  unsigned long lowTime = pulseIn(pwmPin, LOW);    // Time the signal is LOW

  // Calculate the total period
  unsigned long totalTime = highTime + lowTime;

  // Check if the total time is greater than 0 to avoid division by zero
  if (totalTime > 0) {
    // Calculate duty cycle as a percentage
    return (float)highTime / totalTime * 100;
  }

  return 0;
}

void oneStep()
{
  // pwm signal high
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(100);        // Hold for 100 microseconds (adjust as needed)
  digitalWrite(stepPin, LOW);    // Step low
}