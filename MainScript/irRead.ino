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

float irRead() {

  int previous_front_right = 0;
  int previous_back_right = 0;
  int previous_front_left = 0;
  int previous_back_left = 0;

  
  current_front_right = 2227.4 * pow(analogRead(IR_front_right), -0.981);
  current_back_right = 62843.2 * pow(analogRead(IR_back_right), -1.111);
  current_front_left = 6336.7 * pow(analogRead(IR_front_left), -1.065);
  current_back_left = 5259.9 * pow(analogRead(IR_back_left), -0.978);

  if (abs(current_front_right - previous_front_right) < 5) {
    previousIR_cm = current_front_right;
  } 

  if (abs(current_back_right - previous_back_right) < 5) {
    previousIR_cm = current_back_right;
  } 

  if (abs(current_front_left - previous_front_left) < 5) {
    previousIR_cm = current_front_left;
  } 

  if (abs(current_back_left - previous_back_left) < 5) {
    previousIR_cm = current_back_left;
  } 
}
