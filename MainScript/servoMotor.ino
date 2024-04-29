const byte fan_servo = A15;

Servo fan_servo_motor; 

void servoMotor() {
  fan_servo_motor.writeMicroseconds(900);
}