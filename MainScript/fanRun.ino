
bool fanStart = false;
void fanRun() {
  //while (gotBlown != 2){
  while (((ptLeftDist < 50) || (ptMidLeftDist < 50) || (ptMidRightDist < 50) || (ptRightDist < 50))) {
    phototransistorRead();
    
    // BluetoothSerial.print(ptLeftDist);
    // BluetoothSerial.print(" , ");
    // BluetoothSerial.print(ptMidLeftDist);
    // BluetoothSerial.print(" , ");
    // BluetoothSerial.print(ptMidRightDist);
    // BluetoothSerial.print(" , ");
    // BluetoothSerial.println(ptRightDist);
    digitalWrite(FAN_PIN, HIGH);

  }
 
  
  digitalWrite(FAN_PIN, LOW);
  

  if(gotBlown == 2){
     BluetoothSerial.print("Finished?? ");
   delay(1000000000);
 }
}
//}