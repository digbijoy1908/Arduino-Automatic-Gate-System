/*
  Automatic Gate System
  ---------------------
  An ultrasonic sensor (HC-SR04) measures the distance to an object.
  If an object is closer than 15 cm, the servo opens the gate.
  Otherwise, the gate stays closed.

  LED status colors:
    Green -> Gate open  (object detected)
    Red   -> Gate closed (no object nearby)
    Blue  -> No valid sensor reading (nothing in range / sensor error)

  Board: Arduino Uno (or compatible)
*/

#include <Servo.h>

Servo myServo;  // Servo object that controls the gate

// ---------- Pin definitions ----------
const int redPin   = 3;   // Red LED   (gate closed)
const int greenPin = 4;   // Green LED (gate open)
const int bluePin  = 7;   // Blue LED  (no reading)

const int trigPin  = 9;   // Ultrasonic sensor TRIG pin
const int echoPin  = 10;  // Ultrasonic sensor ECHO pin

// (Servo signal wire is connected to pin 5)

// ---------- Variables ----------
long duration;    // Time (in microseconds) for the echo to return
float distance;   // Calculated distance in centimeters

void setup() {
  // LED pins as outputs
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Attach the servo to pin 5 and start at the closed position
  myServo.attach(5);
  myServo.write(90);

  // Start serial monitor for debugging (9600 baud)
  Serial.begin(9600);
}

void loop() {
  // ----- Measure distance -----
  // Make sure the trigger pin is LOW first
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send a 10 microsecond pulse to start the measurement
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure how long the echo pin stays HIGH (timeout after 30 ms)
  duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    // ----- No valid measurement (timeout) -----
    // Only the blue LED turns on
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, HIGH);

    Serial.println("No reading");

  } else {
    // Convert echo time to distance:
    // speed of sound = 0.0343 cm/us, divide by 2 (sound goes there and back)
    distance = duration * 0.0343 / 2;

    // Print distance to the serial monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance <= 15) {
      // ----- Object detected: open the gate -----
      myServo.write(10);            // Open position

      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH); // Green = open
      digitalWrite(bluePin, LOW);

    } else {
      // ----- Object far away: close the gate -----
      myServo.write(90);            // Closed position

      digitalWrite(redPin, HIGH);   // Red = closed
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, LOW);
    }
  }

  // Small delay between measurements
  delay(200);
}
