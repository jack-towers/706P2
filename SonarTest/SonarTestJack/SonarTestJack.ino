/* Example code for HC-SR04 ultrasonic distance sensor with Arduino. 
   No library required. More info: https://www.makerguides.com */
   
#include <SoftwareSerial.h>

// Define Trig and Echo pin:
#define trigPin A3
#define echoPin A1

// Serial Data input pin
#define BLUETOOTH_RX 8
// Serial Data output pin
#define BLUETOOTH_TX 9

SoftwareSerial BluetoothSerial(BLUETOOTH_RX, BLUETOOTH_TX);

// Define variables:
long duration;
int distance;

void setup() {
  // Define inputs and outputs:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Begin Serial communication at a baudrate of 9600:
  BluetoothSerial.begin(115200);
}

void loop() {
  // Clear the trigPin by setting it LOW:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echoPin, pulseIn() returns the duration (length of the pulse) in microseconds:
  duration = pulseIn(echoPin, HIGH);
  // Calculate the distance:
  distance = duration * 0.034 / 2;

  // Print the distance on the Serial Monitor (Ctrl+Shift+M):
  BluetoothSerial.print("Distance = ");
  BluetoothSerial.print(distance);
  BluetoothSerial.println(" cm");

  delay(50);
}