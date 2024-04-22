#include <Servo.h>  //Need for Servo pulse output
#include <SoftwareSerial.h>

Servo turret_motor;

//Default ultrasonic ranging sensor pins, these pins are defined my the Shield
const int TRIG_PIN = 4;
const int ECHO_PIN = 5;

const int FAN_PIN = 42;

int sensorPin = A4;
int sensorValue = 0;
float gyroRate = 0;
// supply voltage for gyro
float gyroZeroVoltage = 0;

//Serial Pointer
HardwareSerial *SerialCom;

// Serial Data input pin
#define BLUETOOTH_RX 10
// Serial Data output pin
#define BLUETOOTH_TX 11

// USB Serial Port
#define OUTPUTMONITOR 0
#define OUTPUTPLOTTER 0

// Bluetooth Serial Port
#define OUTPUTBLUETOOTHMONITOR 1

SoftwareSerial BluetoothSerial(BLUETOOTH_RX, BLUETOOTH_TX);

int pos = 0;

//Global position variables:
float X = 0;
float Y = 0;
float PHI = 0;


void setup() {
  turret_motor.attach(11);
  pinMode(LED_BUILTIN, OUTPUT);

  BluetoothSerial.begin(115200);

  pinMode(FAN_PIN, OUTPUT);

  // The Trigger pin will tell the sensor to range find
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);

  // Setup the Serial port and pointer, the pointer allows switching the debug info through the USB port(Serial) or Bluetooth port(Serial1) with ease.
  SerialCom = &Serial1;
  SerialCom->begin(115200);
  SerialCom->println("MECHENG706_Base_Code_25/01/2018");
  delay(1000);
  SerialCom->println("Setup....");

  // this section is initialize the sensor, find the value of voltage when gyro is zero
  int i;
  float sum = 0;
  pinMode(sensorPin, INPUT);
  Serial.println("please keep the sensor still for calibration");
  Serial.println("get the gyro zero voltage");
  for (i = 0; i < 100; i++)  // read 100 values of voltage when gyro is at still, to calculate the zero-drift.
  {
    sensorValue = analogRead(sensorPin);
    sum += sensorValue;
    delay(5);
  }
  gyroZeroVoltage = sum / 100;  // average the sum as the zero drifting

  delay(1000);  //settling time but no really needed

}

void loop() {
  // put your main code here, to run repeatedly:

fanRun();
}
