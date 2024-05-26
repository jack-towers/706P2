int sum = 0;
int sumPast1 = 0;
int sumPast2 = 0;
int sumPast3 = 0;
int sumPast4 = 0;
int sumPast5 = 0;
int sumAverage;
int sumAveragePast;
int stop = 0;

void phototransistorRead() {
  
 

 

  if (analogRead(ptLeft) < 10) {
    ptLeftDist = 180;
  }else{
    ptLeftDist = 393.8*pow(analogRead(ptLeft), -0.448);
  }
  if (analogRead(ptMidLeft) < 10) {
      ptMidLeftDist = 180;
  }else{
    ptMidLeftDist = 398.45*pow(analogRead(ptMidLeftDist),-0.449);
  }
  if (analogRead(ptMidRight) < 10) {
    ptMidRightDist = 180;
  }else{
    ptMidRightDist = 369.85*pow(analogRead(ptMidRightDist),-0.44);
  }
  if (analogRead(ptRight) < 10) {
    ptRightDist = 180;
  }else{
    ptRightDist = 417.25*pow(analogRead(ptRightDist),-0.438);
  }
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
      //delay(500);
      stop();
    } else {
      cw();
     // delay(500);
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