//Default motor control pins
const byte left_front = 46;
const byte left_rear = 47;
const byte right_rear = 50;
const byte right_front = 51;

// connect the selected commands to the robot motions 
void robotMove()
{
switch(motor_input)
{
  case FORWARD:
  forward();
  delay(8);
  break;
  
  case BACKWARD:
  reverse(12);
  break;
  
  case LEFT_TURN:
  ccw();
  // BluetoothSerial.println("Motor Left");
  delay(3);
  break;
 
  case RIGHT_TURN:
  cw();
  // BluetoothSerial.println("Motor Right");
  delay(3);
  break;
 
  case LEFT_ARC:
  strafe_left();
  delay(40);
  break;
 
  case LEFT_ARC_DELAYED:
  strafe_left();
  delay(140);
  break;
 
  case RIGHT_ARC:
  strafe_right();
  delay(40);
  break;

  case RIGHT_ARC_DELAYED:
  strafe_right();
  delay(140);
  break;

  case BACKWARD_LEFT_TURN:
  reverse_ccw();
  delay(40);
  break;

  case BACKWARD_RIGHT_TURN:
  reverse_cw();
  delay(40);
  break;

  case DIAG_LEFT:
  diagonal_left();
  delay(40);
  break;

  case DIAG_RIGHT:
  diagonal_right();
  delay(40);
  break;

  case STOP:
  BluetoothSerial.println("Motor Stop Start");
  stop();
  delay(100);
  BluetoothSerial.println("Motor Stop End");
  fanRun();
  enable_motors();
  reverse(220);
  break;
  }
}

void disable_motors(){                             // function disable all motors, called in  STOPPED STATE
  left_front_motor.detach();
  left_rear_motor.detach();
  right_rear_motor.detach();
  right_front_motor.detach();

  pinMode(left_front,INPUT);                   // set pinMode for next step 
  pinMode(left_rear,INPUT);
  pinMode(right_rear,INPUT);
  pinMode(right_front,INPUT);
}


void enable_motors() {                                //enable all motors, was called in INITIALZING SATE 
  left_front_motor.attach(left_front);
  left_rear_motor.attach(left_rear);
  right_rear_motor.attach(right_rear);
  right_front_motor.attach(right_front);
}

void stop(){                                                                // stop motors 
  left_front_motor.writeMicroseconds(1500);
  left_rear_motor.writeMicroseconds(1500);
  right_rear_motor.writeMicroseconds(1500);
  right_front_motor.writeMicroseconds(1500);

  delay(150);
  disable_motors();
}
void forward(){                                                         // moving forward  
  left_front_motor.writeMicroseconds(1500 + speed_val);
  left_rear_motor.writeMicroseconds(1500 + speed_val);
  right_rear_motor.writeMicroseconds(1500 - speed_val);
  right_front_motor.writeMicroseconds(1500 - speed_val);
}

void reverse(int time){                                                                  // reverse  
  left_front_motor.writeMicroseconds(1500 - speed_val);
  left_rear_motor.writeMicroseconds(1500 - speed_val);
  right_rear_motor.writeMicroseconds(1500 + speed_val);
  right_front_motor.writeMicroseconds(1500 + speed_val);
  delay(time);
}
void strafe_left(){                                                         // straight left  
  left_front_motor.writeMicroseconds(1500 - speed_val);
  left_rear_motor.writeMicroseconds(1500 + speed_val);
  right_rear_motor.writeMicroseconds(1500 + speed_val);
  right_front_motor.writeMicroseconds(1500 - speed_val);
}
void strafe_right(){                                                  //straight right  
  left_front_motor.writeMicroseconds(1500 + speed_val);
  left_rear_motor.writeMicroseconds(1500 - speed_val);
  right_rear_motor.writeMicroseconds(1500 - speed_val);
  right_front_motor.writeMicroseconds(1500 + speed_val);
}
void cw(){                                                                   //clockwise  
  left_front_motor.writeMicroseconds(1500 + speed_val);
  left_rear_motor.writeMicroseconds(1500 + speed_val);
  right_rear_motor.writeMicroseconds(1500 + speed_val);
  right_front_motor.writeMicroseconds(1500 + speed_val);
}
void ccw(){                                                              //anticlockwise  
  left_front_motor.writeMicroseconds(1500 - speed_val);
  left_rear_motor.writeMicroseconds(1500 - speed_val);
  right_rear_motor.writeMicroseconds(1500 - speed_val);
  right_front_motor.writeMicroseconds(1500 - speed_val);
}

void reverse_ccw()
{
  left_front_motor.writeMicroseconds(1500 - speed_val);
  left_rear_motor.writeMicroseconds(1500 - speed_val);
  right_rear_motor.writeMicroseconds(1500 + speed_val);
  right_front_motor.writeMicroseconds(1500 + speed_val);
  delay(220);
  left_front_motor.writeMicroseconds(1500 - speed_val);
  left_rear_motor.writeMicroseconds(1500 - speed_val);
  right_rear_motor.writeMicroseconds(1500 - speed_val);
  right_front_motor.writeMicroseconds(1500 - speed_val);
  delay(220);
}

void reverse_cw()
{
  left_front_motor.writeMicroseconds(1500 - speed_val);
  left_rear_motor.writeMicroseconds(1500 - speed_val);
  right_rear_motor.writeMicroseconds(1500 + speed_val);
  right_front_motor.writeMicroseconds(1500 + speed_val);
  delay(220);
  left_front_motor.writeMicroseconds(1500 + speed_val);
  left_rear_motor.writeMicroseconds(1500 + speed_val);
  right_rear_motor.writeMicroseconds(1500 + speed_val);
  right_front_motor.writeMicroseconds(1500 + speed_val);
  delay(220);
}

void diagonal_left() {
  left_front_motor.writeMicroseconds(1500 - speed_val);
  left_rear_motor.writeMicroseconds(1500 + speed_val);
  right_rear_motor.writeMicroseconds(1500 + speed_val);
  right_front_motor.writeMicroseconds(1500 - speed_val);
  delay(220);
  left_front_motor.writeMicroseconds(1500 - speed_val);
  left_rear_motor.writeMicroseconds(1500 - speed_val);
  right_rear_motor.writeMicroseconds(1500 + speed_val);
  right_front_motor.writeMicroseconds(1500 + speed_val);  
}

void diagonal_right() {
  left_front_motor.writeMicroseconds(1500 + speed_val);
  left_rear_motor.writeMicroseconds(1500 - speed_val);
  right_rear_motor.writeMicroseconds(1500 - speed_val);
  right_front_motor.writeMicroseconds(1500 + speed_val);
  delay(220);
  left_front_motor.writeMicroseconds(1500 - speed_val);
  left_rear_motor.writeMicroseconds(1500 - speed_val);
  right_rear_motor.writeMicroseconds(1500 + speed_val);
  right_front_motor.writeMicroseconds(1500 + speed_val);
}





