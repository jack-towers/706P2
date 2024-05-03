//Front Right
//ir_B_LONG
int OrangeRed = A12;
int Black = A13;
int GreenYellow = A14;
int Blue = A15;

void setup() {
// put your setup code here, to run once:
Serial.begin(9600); // start serial communication
}

void loop() {
  Serial.print((analogRead(OrangeRed)));
  Serial.print(" , ");
  Serial.print((analogRead(Black)));
  Serial.print(" , ");
  Serial.print((analogRead(GreenYellow)));
  Serial.print(" , ");
  Serial.println((analogRead(Blue)));

  delay(500);
}