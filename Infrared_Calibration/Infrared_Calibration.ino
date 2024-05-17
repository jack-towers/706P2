#include <SoftwareSerial.h>

// Serial Data input pin
#define BLUETOOTH_RX 10
// Serial Data output pin
#define BLUETOOTH_TX 11

SoftwareSerial BluetoothSerial(BLUETOOTH_RX, BLUETOOTH_TX);

//Front Right
//ir_A_LONG
int IR_front_right = A10;

//Front Left
//ir_C_Long
int IR_front_left = A8;

//Side Right
//ir_W_Short
int IR_side_right = A11;

//Side left
//ir_Y_Short
int IR_side_left = A9;

byte serialRead = 0; //for control serial communication
int signalADC1 = 0; // the read out signal in 0-1023 corresponding to 0-5v
int signalADC2 = 0; // the read out signal in 0-1023 corresponding to 0-5v
int signalADC3 = 0; // the read out signal in 0-1023 corresponding to 0-5v
int signalADC4 = 0; // the read out signal in 0-1023 corresponding to 0-5v


void setup() {
// put your setup code here, to run once:
  BluetoothSerial.begin(115200);
  Serial.begin(9600); // start serial communication
}

void loop() {
// put your main code here, to run repeatedly:
  if (Serial.available()) { // Check for input from terminal 
    signalADC1 = analogRead(IR_front_left); // the read out is a signal from 0-1023 corresponding to 0-5v
    Serial.print("FL: ");
    Serial.println(signalADC1);
    
    signalADC2 = analogRead(IR_front_right); // the read out is a signal from 0-1023 corresponding to 0-5v
    Serial.print("FR: ");
    Serial.println(signalADC2);

    signalADC3 = analogRead(IR_side_left); // the read out is a signal from 0-1023 corresponding to 0-5v
    Serial.print("SL: ");
    Serial.println(signalADC3);

    signalADC4 = analogRead(IR_side_right); // the read out is a signal from 0-1023 corresponding to 0-5v
    Serial.print("SR: ");
    Serial.println(signalADC4);

    delay(500);
  }
}