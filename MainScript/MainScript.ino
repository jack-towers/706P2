#include <Servo.h>  //Need for Servo pulse output
#include <SoftwareSerial.h>

Servo turret_motor;

//Default ultrasonic ranging sensor pins, these pins are defined my the Shield
const int TRIG_PIN = 4;
const int ECHO_PIN = 5;

const int FAN_PIN = 42;

int sensorPin = A4;
int sensorValue = 0;
float gyroRate = 0;
// supply voltage for gyro
float gyroZeroVoltage = 0;

//Serial Pointer
HardwareSerial *SerialCom;

// Serial Data input pin
#define BLUETOOTH_RX 10
// Serial Data output pin
#define BLUETOOTH_TX 11

// USB Serial Port
#define OUTPUTMONITOR 0
#define OUTPUTPLOTTER 0

// Bluetooth Serial Port
#define OUTPUTBLUETOOTHMONITOR 1

SoftwareSerial BluetoothSerial(BLUETOOTH_RX, BLUETOOTH_TX);

int pos = 0;

//Global position variables:
float X = 0;
float Y = 0;
float PHI = 0;


// three machine states 
enum STATE {
  INITIALISING,
  RUNNING, 
  STOPPED
};

// define motions states
enum MOTION{
FORWARD,
BACKWARD,
LEFT_TURN,
RIGHT_TURN,
LEFT_ARC,
RIGHT_ARC,
BACKWARD_LEFT_TURN,
};

// declare function output and function flag
MOTION cruise_command;
int cruise_output_flag;
MOTION follow_command;
int follow_output_flag;
MOTION avoid_command;
int avoid_output_flag;
MOTION escape_command;
int escape_output_flag;
MOTION motor_input;


void setup() {
  turret_motor.attach(11);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  BluetoothSerial.begin(115200);
  
  // Setup the Serial port and pointer, the pointer allows switching the debug info through the USB port(Serial) or Bluetooth port(Serial1) with ease.
  SerialCom = &Serial1;
  SerialCom->begin(115200);
  SerialCom->println("MECHENG70_WE_ONNNNN");
  delay(1000);
  SerialCom->println("Setup....");

  // this section is initialize the sensor, find the value of voltage when gyro is zero
  int i;
  float sum = 0;
  pinMode(sensorPin, INPUT);
  Serial.println("please keep the sensor still for calibration");
  Serial.println("get the gyro zero voltage");
  for (i = 0; i < 100; i++)  // read 100 values of voltage when gyro is at still, to calculate the zero-drift.
  {
  sensorValue = analogRead(sensorPin);
  sum += sensorValue;
  delay(5);
  }
  gyroZeroVoltage = sum / 100;  // average the sum as the zero drifting

  delay(1000);  //settling time but no really needed
  /////////

}

void loop() {
  // put your main code here, to run repeatedly:
  static STATE machine_state = INITIALISING; // start from the sate
  INITIALIING
  switch (machine_state)
  {
    case INITIALISING:
      machine_state = initialising();
    break;
    case RUNNING:
      machine_state = running();
    break;
    case STOPPED:
      machine_state = stopped();
    break;
  }
  fanRun();
}

STATE initialising(){
  enable_motors();                                 // enable motors 
  Serial.println("INITIALISING");        // print the current stage 
  return RUNNING;                               // return to RUNING STATE DIRECTLY 
}

STATE running(){
  //read_serial_command();                      // read command from serial communication
  speed_change_smooth();                 //function to speed up and slow down smoothly 
  // this is just for test functions to read simulative                       sensor reading from monitor
  serial_read_conditions();  
  // four function 
  cruise(); 
  follow(); 
  avoid(); 
  escape();
  // select the output command based on the function priority 
  arbitrate();
  photo_left = 0; 
  photo_right = 0;
  ir_detect = 0; 
  bumper_left = 0;
  bumper_right = 0;
  bumper_back = 0; 
  return RUNNING;   // return to RUNNING STATE again, it will run the RUNNING    
                   
}                                                            // STATE REPEATLY 


STATE stopped(){
disable_motors();                           // disable the motors
}


  void speed_change_smooth()                  // change speed, called in RUNING STATE
{
  speed_val += speed_change;                  // speed value add on speed change 
   if(speed_val > 500)                          // make sure speed change less than 1000
   speed_val = 500;
   speed_change = 0;    //make speed change equals 0 after updating the speed value 
}

// cruise function output command and flag
void cruise()
{
  cruise_command = FORWARD;
  cruise_output_flag=1; 
  }

// follow function output command and flag
void follow()
{ int delta;
  //int left_photo, right_photo, delta;
    //left_photo=analog(1);
   // right_photo=analog(0);
    delta=photo_right - photo_left;
    if (abs(delta)>photo_dead_zone)
      {if (delta>0)
        follow_command=LEFT_TURN;
      else 
        follow_command=RIGHT_TURN;
      follow_output_flag=1;
      }
    else
      follow_output_flag=0;
             
}
// avoid function output command and flag 
void avoid()
{int val;
     val=ir_detect;
    //val=ir_detect();
    if (val==1)
      {avoid_output_flag=1;
      avoid_command=BACKWARD;}
    else if (val==2)
      {avoid_output_flag=1;
      avoid_command=RIGHT_ARC;}
    else if (val==3)
      {avoid_output_flag=1;
      avoid_command=LEFT_ARC;}
    else
      {avoid_output_flag=0;}
     
 }


//escape function output command and flag
void escape()
{ 
//bumper_check();
if (bumper_left && bumper_right)
  {escape_output_flag=1;
  escape_command=BACKWARD_LEFT_TURN;
 }
else if (bumper_left)
  {escape_output_flag=1;
  escape_command=RIGHT_TURN;
  }
else if (bumper_right)
  {escape_output_flag=1;
  escape_command=LEFT_TURN;
  }
else if (bumper_back)
  {escape_output_flag=1;
  escape_command=LEFT_TURN;
  }
else
  escape_output_flag=0;   
}

// check flag and select command based on priority 
void arbitrate ()
 {
  if (cruise_output_flag==1)
  {motor_input=cruise_command;}
  if (follow_output_flag==1)
  {motor_input=follow_command;}
  if (avoid_output_flag ==1)
  {motor_input=avoid_command;}
  if (escape_output_flag==1)
  {motor_input=escape_command;}
  robotMove();                                    
  }


// connect the selected commands to the robot motions 
void robotMove()
{
switch(motor_input)
{
  case FORWARD:
  forward ();
  delay(1000);
  break;
  
  case BACKWARD:
  reverse ();
  delay(1000);
  break;
  
  case LEFT_TURN:
  ccw();
 delay(1000);
  break;
 
  case RIGHT_TURN:
  cw();
 delay(1000);
  break;
 
  case LEFT_ARC:
  strafe_left();
  delay(1000);
  break;
 
  case RIGHT_ARC:
  strafe_right();
   delay(1000);
  break;

  case BACKWARD_LEFT_TURN:
  reverse_ccw();
  delay(1000);
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
}
void forward(){                                                         // moving forward  
  left_front_motor.writeMicroseconds(1500 + speed_val);
  left_rear_motor.writeMicroseconds(1500 + speed_val);
  right_rear_motor.writeMicroseconds(1500 - speed_val);
  right_front_motor.writeMicroseconds(1500 - speed_val);
}

void reverse(){                                                                  // reverse  
  left_front_motor.writeMicroseconds(1500 - speed_val);
  left_rear_motor.writeMicroseconds(1500 - speed_val);
  right_rear_motor.writeMicroseconds(1500 + speed_val);
  right_front_motor.writeMicroseconds(1500 + speed_val);
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
  delay(500);
  left_front_motor.writeMicroseconds(1500 - speed_val);
  left_rear_motor.writeMicroseconds(1500 - speed_val);
  right_rear_motor.writeMicroseconds(1500 - speed_val);
  right_front_motor.writeMicroseconds(1500 - speed_val);
}






