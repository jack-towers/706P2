int sum = 0;
int sumPast1 = 0;
int sumPast2 = 0;
int sumPast3 = 0;
int sumPast4 = 0;
int sumPast5 = 0;
int sumAverage;
int sumAveragePast;

void phototransistorRead() {
  ptLeftDist = analogRead(ptLeft);
  ptMidLeftDist = analogRead(ptMidLeftDist);
  ptMidRightDist = analogRead(ptMidRightDist);
  ptRightDist = analogRead(ptRightDist);
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