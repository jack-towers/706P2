//Front Right
//ir_B_LONG
int PhotoTransistor = A15;

void setup() {
// put your setup code here, to run once:
Serial.begin(9600); // start serial communication
}

void loop() {
  Serial.println((analogRead(PhotoTransistor)));

  delay(500);
}