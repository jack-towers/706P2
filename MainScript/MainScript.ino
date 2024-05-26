#include <Servo.h>  //Need for Servo pulse output
#include <SoftwareSerial.h>

// create servo objects for each motor 
Servo left_front_motor;   // create servo object to control Vex Motor Controller 29
Servo left_rear_motor;   // create servo object to control Vex Motor Controller 29
Servo right_rear_motor;  // create servo object to control Vex Motor Controller 29
Servo right_front_motor;  // create servo object to control Vex Motor Controller 29

//servoMotor for fan
const byte fan_servo = 45;
Servo fan_servo_motor; 

int speed_val = 100;
int speed_change;

//Phototransistor initialisations
int ptLeft = A12;
int ptMidLeft = A13;
int ptMidRight = A14;
int ptRight = A15;

float ptLeftDist;
float ptMidLeftDist;
float ptMidRightDist;
float ptRightDist;

int ongoingAngle = 1500;
int angleIncrement = 40; //6.666667 is 1 degree
int turnDirection = 0;

Servo turret_motor;

//  define the sensor reading results 
int photo_left;
int photo_right;
int ir_detect;
int bumper_left;
int bumper_right;
int bumper_back;

bool bumper_frontleftProx,bumper_frontrightProx, bumper_leftProx,bumper_rightProx,bumper_frontProx;

// define threshold of phototransistor  difference 
int photo_dead_zone = 5;

//Default ultrasonic ranging sensor pins, these pins are defined my the Shield
const int TRIG_PIN = A3;
const int ECHO_PIN = A1;

const int FAN_PIN = 42;

int sensorPin = A4;
int sensorValue = 0;
float gyroRate = 0;
// supply voltage for gyro
float gyroZeroVoltage = 0;

//Serial Pointer
//HardwareSerial *SerialCom;

// Serial Data input pin
#define BLUETOOTH_RX 8
// Serial Data output pin
#define BLUETOOTH_TX 9

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
STOP,
};

// declare function output and function flag
MOTION search_command;
int search_output_flag;
MOTION cruise_command;
int cruise_output_flag;
MOTION follow_command;
int follow_output_flag;
MOTION avoid_command;
int avoid_output_flag;
MOTION escape_command;
int escape_output_flag;
MOTION target_acquired_command;
int target_acquired_flag;
MOTION motor_input;

String mode;

void setup() {
  turret_motor.attach(11);
  fan_servo_motor.attach(45);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  BluetoothSerial.begin(115200);
  
  // Setup the Serial port and pointer, the pointer allows switching the debug info through the USB port(Serial) or Bluetooth port(Serial1) with ease.
  //SerialCom = &Serial1;
 

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

 // delay(1000);  //settling time but no really needed
  /////////

  fan_servo_motor.writeMicroseconds(1500);

}

void loop() {
  // put your main code here, to run repeatedly:
  static STATE machine_state = INITIALISING; // start from the sate
  INITIALISING;
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

  // servoMotor();
  // delay(1000);
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
  //serial_read_conditions();  
  // four function
  //BluetoothSerial.print("Current State: ");
  //BluetoothSerial.println(mode);
  search(); 
  // BluetoothSerial.print("Current State: ");
  // BluetoothSerial.println(mode);
  cruise(); 
  // BluetoothSerial.print("Current State: ");
  // BluetoothSerial.println(mode);
  follow(); 
  //BluetoothSerial.print("Current State: ");
  //BluetoothSerial.println(mode);
  avoid(); 
  //BluetoothSerial.print("Current State: ");
  //BluetoothSerial.println(mode);
  escape();

  targetAcquired();
  // select the output command based on the function priority 
  arbitrate();
  photo_left = 0; 
  photo_right = 0;
  ir_detect = 0; 
  bumper_left = 0;
  bumper_right = 0;
  bumper_back = 0; 
  
  //delay(50);
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

void search() {
  search_command = RIGHT_TURN;
  search_output_flag=1;
}

// cruise function output command and flag
void cruise() {
  mode = "cruise";
  cruise_command = FORWARD;

  phototransistorRead();
    BluetoothSerial.print(ptLeftDist);
    BluetoothSerial.print(" , ");
    BluetoothSerial.print(ptMidLeftDist);
    BluetoothSerial.print(" , ");
    BluetoothSerial.print(ptMidRightDist);
    BluetoothSerial.print(" , ");
    BluetoothSerial.println(ptRightDist);
  if (((ptLeftDist > 25) | (ptMidLeftDist > 25) | (ptMidRightDist > 25) | (ptRightDist > 25)) & ((ptLeftDist < 155) | (ptMidLeftDist < 155) | (ptMidRightDist < 155) | (ptRightDist < 155) & (abs(ptRightDist - ptLeftDist) < 25))) {
    cruise_output_flag=1;
    BluetoothSerial.println("In Cruise");
  } else {
    cruise_output_flag=0;
  }
}

// follow function output command and flag
//Need to make turning more accurate
void follow() {
  mode = "follow";
  phototransistorRead();

  if (((ptLeftDist < 155) | (ptMidLeftDist < 155) | (ptMidRightDist < 155) | (ptRightDist < 155)) & (abs(ptRightDist - ptLeftDist) > 25)) {
     BluetoothSerial.println("In Follow");
    if (ptRightDist > ptLeftDist) {
      follow_command=LEFT_TURN;
    }  else {
      follow_command=RIGHT_TURN;
    }
    follow_output_flag=1;

  } else {
    follow_output_flag=0;
  }         
}



// avoid function output command and flag 
void avoid() {
    mode = "avoid";
     int val;
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
//bool bumper_frontleftProx,bumper_frontrightProx, bumper_leftProx,bumper_rightProx,bumper_frontProx;


void escape() { 
mode = "escape";
bumper_check();
if (bumper_frontProx)
  {escape_output_flag=1;
  BluetoothSerial.println("Object in front");  
  escape_command=BACKWARD;
 }
 
else if (bumper_leftProx)
  {escape_output_flag=1;
  BluetoothSerial.println("Object on left");
  escape_command=RIGHT_ARC;
  }
else if (bumper_rightProx)
  {escape_output_flag=1;
  BluetoothSerial.println("Object on right");
  escape_command=LEFT_ARC;
  }
else if (bumper_frontrightProx)
  {escape_output_flag=1;
   BluetoothSerial.println("Object on front right");
  escape_command=BACKWARD_LEFT_TURN;
  }
//SHOULD THERE BE A BACKWARD RIGHT TURN???
else if (bumper_frontleftProx)
  {escape_output_flag=1;
  BluetoothSerial.println("Object on front left");
  escape_command=BACKWARD;
  }
else
  BluetoothSerial.println("Clear");
  escape_output_flag=0;   
}

void targetAcquired (){

  if((ptLeftDist < 60) | (ptMidLeftDist < 35) | (ptMidRightDist < 35) | (ptRightDist < 60)){
    target_acquired_flag = 1;
    target_acquired_command = STOP;

    fanRun();
 }
  else{
    target_acquired_flag = 0;
  }
 


}

// check flag and select command based on priority 
void arbitrate () {
  if (search_output_flag==1)
  {motor_input=search_command;}
  if (cruise_output_flag==1)
  {motor_input=cruise_command;}
  if (follow_output_flag==1)
  {motor_input=follow_command;}
  if (avoid_output_flag ==1)
  {motor_input=avoid_command;}
  if (escape_output_flag==1)
  {motor_input=escape_command;}
  if (target_acquired_flag==1)
  {motor_input=target_acquired_command;}
  BluetoothSerial.print("Command is:");
  BluetoothSerial.println(motor_input);
  robotMove();                                    
}

//Sensor Checkkssss

void bumper_check(){
  
  // Allocate array to hold current sensor values

  float* current_sensors = irRead();
  if ( current_sensors[1] < 5){
    bumper_frontrightProx = true;
  }else{
    bumper_frontrightProx = false;    
  }

  if( current_sensors[0]< 5){
    bumper_frontleftProx = true;
  }else{
    bumper_frontleftProx = false;
  }

  if(current_sensors[2]< 5){
    bumper_leftProx = true;
  }else{
    bumper_leftProx = false;
  }

  if(current_sensors[3]<5){
    BluetoothSerial.print("Distance Right");
    BluetoothSerial.println(current_sensors[3]);
    bumper_rightProx = true;
  }else{
    bumper_rightProx = false;
  }

  if(sonarRead() < 5){
    bumper_frontProx = true; 
    BluetoothSerial.print("Distance: "); 
    BluetoothSerial.println(sonarRead());
  }else{
    bumper_frontProx = false;
  }

}



////////////////////////////