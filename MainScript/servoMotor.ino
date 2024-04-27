const byte fan_servo = A15;

Servo fan_servo; 

void servoMotor() {
  fan_servo.writeMicroseconds(900);
}