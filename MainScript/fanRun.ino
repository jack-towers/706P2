const int FAN_PIN = 42;

fanRun() {
  digitalWrite(FAN_PIN, HIGH);
  delayMicroseconds(5000);
  digitalWrite(FAN_PIN, LOW);
}