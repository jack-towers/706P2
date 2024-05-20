int sum = 0;
int sumPast1 = 0;
int sumPast2 = 0;
int sumPast3 = 0;
int sumPast4 = 0;
int sumPast5 = 0;
int sumAverage;
int sumAveragePast;

void phototransistorRead() {
  // ptLeftDist = analogRead(ptLeft);
  // ptMidLeftDist = analogRead(ptMidLeft);
  // ptMidRightDist = analogRead(ptMidRight);
  // ptRightDist = analogRead(ptRight);
  ptLeftDist = 100 - (393.8*pow(analogRead(ptLeft)+1, -0.448))/180 * 100;
  ptMidLeftDist = 398.45*pow(analogRead(ptMidLeftDist)+1,-0.449);
  ptMidRightDist = 100 - (369.85*pow(analogRead(ptMidRightDist)+1,-0.44))/180 * 100;
  ptRightDist = 100 - (417.25*pow(analogRead(ptRightDist)+1,-0.438))/180 * 100;
  // BluetoothSerial.println(ptLeftDist);
  BluetoothSerial.println(ptMidLeftDist);
  delay(100);
  // BluetoothSerial.println(ptMidRightDist);
  // BluetoothSerial.println(ptRightDist);
}

float phototransistorHone() {
  if (ptLeftDist > ptRightDist) {
    turnDirection = 1;
  } else {
    turnDirection = 0;
  }

  sum = (ptMidLeftDist + ptMidRightDist) / 2;
  sumAverage = sum + sumPast1 + sumPast2 + sumPast3 + sumPast4;
  sumAveragePast = sumPast1 + sumPast2 + sumPast3 + sumPast4 + sumPast5;

  while (sumAverage > sumAveragePast) {

    if (stop == 0) {
      honeServo();
    }

    if (turnDirection = 1) {
      ccw();
      delay(500);
      stop();
    } else {
      cw();
      delay(500);
      stop();
    }

    sumPast1 = sum;
    sumPast2 = sumPast1;
    sumPast3 = sumPast2;
    sumPast4 = sumPast3;
    sumPast5 = sumPast4;    
    sum = (ptMidLeftDist + ptMidRightDist) / 2;
    sumAverage = sum + sumPast1 + sumPast2 + sumPast3 + sumPast4;
    sumAveragePast = sumPast1 + sumPast2 + sumPast3 + sumPast4 + sumPast5;
  }
}