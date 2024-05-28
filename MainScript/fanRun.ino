void fanRun() {
  while (((ptLeftDist < 50) || (ptMidLeftDist < 50) || (ptMidRightDist < 50) || (ptRightDist < 50))) {
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