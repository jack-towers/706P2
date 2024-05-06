void servoMotor() {
  fan_servo_motor.writeMicroseconds(1500);
  delay(1000);
  fan_servo_motor.writeMicroseconds(900);
  delay(1000);
  fan_servo_motor.writeMicroseconds(2100);
  delay(1000);
  fan_servo_motor.writeMicroseconds(900);
  delay(1000);
  fan_servo_motor.writeMicroseconds(2100);
  delay(1000);
  fan_servo_motor.writeMicroseconds(1500);
}

void turnServo() {
  int angleIncrement = 40;//6 degree rotation
  if (turnDirection == 0) {
    ongoingAngle = ongoingAngle + angleIncrement;
  } else {
    ongoingAngle = ongoingAngle - angleIncrement;
  }

  fan_servo_motor.writeMicroseconds(ongoingAngle);

  if (ongoingAngle == 2100) {
    turnDirection = 1;
  } else if (ongoingAngle == 900) {
    turnDirection = 0;
  }  
}

void honeServo() {
  int angleIncrement = 20;//3 degree rotation

  if (turnDirection == 0) {
    ongoingAngle = ongoingAngle + angleIncrement;
  } else {
    ongoingAngle = ongoingAngle - angleIncrement;
  }

  fan_servo_motor.writeMicroseconds(ongoingAngle);

  if ((ongoingAngle == 2100) | (ongoingAngle == 900)) {
    stop = 1;
  }
}