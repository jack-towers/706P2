float gyroSupplyVoltage = 5;

// supply voltage for gyro
float gyroZeroVoltage = 0;

// the value of voltage when gyro is zero
float gyroSensitivity = 0.007;
float rotationThreshold = 1.5;
float gyroRate = 0;
float currentAngle = 0;

byte serialRead = 0;

int sensorPin = A4;
int T = 100;
int sensorValue = 0;
unsigned long T1 = 0;
unsigned long T2 = 0;


//Function to read angle off gyro sensor
float gyroRead() {
  T1 = millis();
  // convert the 0-1023 signal to 0-5v
  gyroRate = (analogRead(sensorPin) * gyroSupplyVoltage) / 1023;

  // find the voltage offset the value of voltage when gyro is zero (still)
  gyroRate -= (gyroZeroVoltage / 1023 * gyroSupplyVoltage);

  // read out voltage divided the gyro sensitivity to calculate the angular velocity
  float angularVelocity = gyroRate / gyroSensitivity;  // from Data Sheet, gyroSensitivity is 0.007 V/dps

  // if the angular velocity is less than the threshold, ignore it
  if (angularVelocity >= rotationThreshold || angularVelocity <= -rotationThreshold) {
    // we are running a loop in T (of T/1000 second).
    unsigned long T3 = T1 - T2;
    float angleChange = angularVelocity / (1000 / T3);
    currentAngle += angleChange;
  }

  // keep the angle between 0-360
  if (currentAngle < 0) {
    currentAngle += 360;
  } else if (currentAngle > 359) {
    currentAngle -= 360;
  }

  T2 = T1;

  return currentAngle;
}