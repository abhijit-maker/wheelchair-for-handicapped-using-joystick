/*
author abhijit-maker
*/

#include <L298N.h>

// Define joystick pins
const int joyXPin = A0; // Analog pin for X axis
const int joyYPin = A1; // Analog pin for Y axis
const int joySwPin = 2; // Digital pin for joystick switch (optional)

// Define motor control pins
const int enA = 10; // Enable pin for Motor A
const int in1 = 9;  // Control pin 1 for Motor A
const int in2 = 8;  // Control pin 2 for Motor A
const int enB = 5;  // Enable pin for Motor B
const int in3 = 7;  // Control pin 1 for Motor B 
const int in4 = 6;  // Control pin 2 for Motor B

void setup() {
  // Initialize motor control pins
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial.begin(9600); // Initialize serial communication
}

void loop() {
  // Read joystick values
  int joyXValue = analogRead(joyXPin);
  int joyYValue = analogRead(joyYPin);
  int joySwValue = digitalRead(joySwPin);

  // Map joystick values to motor speeds (adjust as needed)
  int motorSpeedA = map(joyYValue, 0, 1023, -255, 255);
  int motorSpeedB = map(joyYValue, 0, 1023, -255, 255);

  // Map joystick X axis to control direction (adjust as needed)
  if (joyXValue < 500) {
    // Turn left
    motorSpeedA = -motorSpeedA;
  } else if (joyXValue > 550) {
    // Turn right
    motorSpeedB = -motorSpeedB;
  }

  // Send motor speeds to second Arduino via Serial
  Serial.print(motorSpeedA);
  Serial.print(",");
  Serial.println(motorSpeedB);

  delay(100); // Adjust delay as needed for responsiveness
   if (Serial.available() > 0) {
    // Read the incoming string until newline character
    String data = Serial.readStringUntil('\n');

    // Split the data into two parts (motor A speed and motor B speed)
    int commaIndex = data.indexOf(',');
    String motorASpeedStr = data.substring(0, commaIndex);
    String motorBSpeedStr = data.substring(commaIndex + 1);

    // Convert string to integer values
    int motorASpeed = motorASpeedStr.toInt();
    int motorBSpeed = motorBSpeedStr.toInt();

    // Control motor A
    if (motorASpeed >= 0) {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    } else {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    }
    analogWrite(enA, abs(motorASpeed));

    // Control motor B
    if (motorBSpeed >= 0) {
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    } else {
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }
    analogWrite(enB, abs(motorBSpeed));
  }
}

