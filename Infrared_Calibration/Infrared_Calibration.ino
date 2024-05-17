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
    signalADC1 = analogRead(IR_front_left); // the read out is a signal from 0-1023 corresponding to 0-5v
    BluetoothSerial.print("FL: ");
    BluetoothSerial.print(signalADC1);
    BluetoothSerial.print(" , ");
    
    signalADC2 = analogRead(IR_front_right); // the read out is a signal from 0-1023 corresponding to 0-5v
    BluetoothSerial.print("FR: ");
    BluetoothSerial.print(signalADC2);
    BluetoothSerial.print(" , ");

    signalADC3 = analogRead(IR_side_left); // the read out is a signal from 0-1023 corresponding to 0-5v
    BluetoothSerial.print("SL: ");
    BluetoothSerial.print(signalADC3);
    BluetoothSerial.print(" , ");

    signalADC4 = analogRead(IR_side_right); // the read out is a signal from 0-1023 corresponding to 0-5v
    BluetoothSerial.print("SR: ");
    BluetoothSerial.println(signalADC4);

    delay(500);
}