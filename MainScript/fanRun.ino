void fanRun() {
  while (((ptLeftDist < 80) || (ptMidLeftDist < 80) || (ptMidRightDist < 80) || (ptRightDist < 80))) {
    phototransistorRead();
    BluetoothSerial.print(ptLeftDist);
    BluetoothSerial.print(" , ");
    BluetoothSerial.print(ptMidLeftDist);
    BluetoothSerial.print(" , ");
    BluetoothSerial.print(ptMidRightDist);
    BluetoothSerial.print(" , ");
    BluetoothSerial.println(ptRightDist);
    digitalWrite(FAN_PIN, HIGH);
  }
  digitalWrite(FAN_PIN, LOW);
}