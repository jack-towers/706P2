#include <SoftwareSerial.h>

// Serial Data input pin
#define BLUETOOTH_RX 8
// Serial Data output pin
#define BLUETOOTH_TX 9

SoftwareSerial BluetoothSerial(BLUETOOTH_RX, BLUETOOTH_TX);

//Front Right
//ir_B_LONG
int ptLeft= A12;
int ptMidLeft = A13;
int ptMidRight = A14;
int ptRight = A15;

void setup() {
// put your setup code here, to run once:
  BluetoothSerial.begin(115200);
  Serial.begin(9600); // start serial communication
}

void loop() {

  BluetoothSerial.print(393.8*pow(analogRead(ptLeft), -0.448));
  BluetoothSerial.print(" , ");
  BluetoothSerial.print(398.45*pow(analogRead(ptMidLeft),-0.449));
  BluetoothSerial.print(" , ");
  BluetoothSerial.print(369.85*pow(analogRead(ptMidRight),-0.44));
  BluetoothSerial.print(" , ");
  BluetoothSerial.println(417.25*pow(analogRead(ptRight),-0.438));

  delay(500);
}