#include <SoftwareSerial.h>

// Serial Data input pin
#define BLUETOOTH_RX 10
// Serial Data output pin
#define BLUETOOTH_TX 11

SoftwareSerial BluetoothSerial(BLUETOOTH_RX, BLUETOOTH_TX);

//Front Right
//ir_B_LONG
int OrangeRed = A12;
int Black = A13;
int GreenYellow = A14;
int Blue = A15;

void setup() {
// put your setup code here, to run once:
  BluetoothSerial.begin(115200);
  Serial.begin(9600); // start serial communication
}

void loop() {
  BluetoothSerial.print((analogRead(OrangeRed)));
  BluetoothSerial.print(" , ");
  BluetoothSerial.print((analogRead(Black)));
  BluetoothSerial.print(" , ");
  BluetoothSerial.print((analogRead(GreenYellow)));
  BluetoothSerial.print(" , ");
  BluetoothSerial.println((analogRead(Blue)));

  delay(500);
}