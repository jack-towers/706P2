int IR = A9; //sensor is attached on pinA0
int C = A11;


byte serialRead = 0; //for control serial communication
int signalADC = 0; // the read out signal in 0-1023 corresponding to 0-5v

void setup() {
// put your setup code here, to run once:
BluetoothSerial.begin(115200);
Serial.begin(9600); // start serial communication
}

void loop() {
// put your main code here, to run repeatedly:
if (Serial.available()) // Check for input from terminal
{
  // signalADC = analogRead(W); // the read out is a signal from 0-1023 corresponding to 0-5v
  // signalADC = 6336.7*pow(analogRead(C),-1.065 );
  // Serial.print("C: ");
  // Serial.println(signalADC);

  signalADC = analogRead(IR); // the read out is a signal from 0-1023 corresponding to 0-5v
  Serial.print("Y: ");
  Serial.println(signalADC);

  delay(2000);
}
