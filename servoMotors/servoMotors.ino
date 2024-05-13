#include <Servo.h>  //Need for Servo pulse output
#include <SoftwareSerial.h>

//servoMotor for fan
const byte fan_servo = 45;
Servo fan_servo_motor; 

void setup() {
  // put your setup code here, to run once:
  fan_servo_motor.attach(45);
}

void loop() {
  // put your main code here, to run repeatedly:
  fan_servo_motor.writeMicroseconds(1500);
  delay(1000);
  // fan_servo_motor.writeMicroseconds(900);
  // delay(1000);
}

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