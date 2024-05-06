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

void turnServo () {

}