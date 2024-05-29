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

int speed_val = 225;
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
int in_avoid = 0;
int sonar_toggle;

bool bumper_frontleftProx,bumper_frontrightProx, bumper_leftProx,bumper_rightProx,bumper_frontProx;

// define threshold of phototransistor  difference 
int photo_dead_zone = 5;
int turningRange;
//Default ultrasonic ranging sensor pins, these pins are defined my the Shield
const int TRIG_PIN = A3;
const int ECHO_PIN = A1;

const int FAN_PIN = 42;

//Front Right
//ir_Unamed_short
int IR_front_right = A10;

//Back Right
//ir_Y_short
int IR_side_right = A11;

//Front Left
//ir_A_Long
int IR_front_left = A8;

//Back left
//ir_B_Long
int IR_side_left = A9;

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

int gotBlown = 0;
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
LEFT_ARC_DELAYED,
RIGHT_ARC,
RIGHT_ARC_DELAYED,
BACKWARD_LEFT_TURN,
BACKWARD_RIGHT_TURN,
DIAG_LEFT,
DIAG_RIGHT,
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
  fan_servo_motor.detach();
  if(!batteryTest()){
    BluetoothSerial.print("FUCK YOU, CHARGE THE FUCKN ROBOT");
    delay(100000000000000);
  }
}

void loop() {

  // put your main code here, to run repeatedly:
  // static STATE machine_state = INITIALISING; // start from the sate
  // INITIALISING;
  // switch (machine_state)
  // {
  //   case INITIALISING:
  //     machine_state = initialising();
  //   break;
  //   case RUNNING:
  //     machine_state = running();
  //   break;
  //   case STOPPED:
  //     machine_state = stopped();
  //   break;
  // }

  enable_motors(); 

  //read_serial_command();                      // read command from serial communication
  speed_change_smooth();                 //function to speed up and slow down smoothly 
  // this is just for test functions to read simulative                       sensor reading from monitor
  
  search(); 
  cruise(); 
  follow(); 
  avoid(); 
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
  sonar_toggle = 1;
  if (cruise_output_flag || follow_output_flag || target_acquired_flag) {
    sonar_toggle = 0;
  }
}

// cruise function output command and flag
void cruise() {
  mode = "cruise";
  cruise_command = FORWARD;

  phototransistorRead();
  if (((ptLeftDist > 35) | (ptMidLeftDist > 35) | (ptMidRightDist > 35) | (ptRightDist > 35)) & ((ptLeftDist < 175) | (ptMidLeftDist < 175) | (ptMidRightDist < 175) | (ptRightDist < 175)) & (abs(ptRightDist - ptLeftDist) < 25)) {
    cruise_output_flag=1;
  } else {
    cruise_output_flag=0;
  }
}

// follow function output command and flag
//Need to make turning more accurate
void follow() {
  mode = "follow";
  phototransistorRead();
  
  if((ptLeftDist+ptMidLeftDist+ptMidRightDist+ptRightDist)/4 > 50){
    turningRange = 20;
  }else if ((ptLeftDist+ptMidLeftDist+ptMidRightDist+ptRightDist)/4 > 30){
    turningRange = 15;
  } else {
    turningRange = 10;
  }
  
  if (((ptLeftDist < 175) | (ptMidLeftDist < 175) | (ptMidRightDist < 175) | (ptRightDist < 175)) & (abs((ptMidRightDist - ptMidLeftDist) > turningRange))) {
    if (ptMidRightDist > ptMidLeftDist) {
      // BluetoothSerial.println("Follow Left");
      follow_command=LEFT_TURN;
    }  else {
      // BluetoothSerial.println("Follow Right");
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
     bumper_check_avoid();
   if (bumper_frontProx && !sonar_toggle) {
     in_avoid = 1;
     if (analogRead(IR_side_left) > 400) {
      //  BluetoothSerial.println("Object In Front: Avoid Left");  
       avoid_output_flag=1;
       avoid_command=RIGHT_ARC;
      } else if(analogRead(IR_side_right) > 400) {
        // BluetoothSerial.println("Object In Front: Avoid Right");  
       avoid_output_flag=1;
       avoid_command= LEFT_ARC;
      } else {
        // BluetoothSerial.println("Object In Front: Avoid, Assumed");  
        avoid_output_flag=1;
        avoid_command= LEFT_ARC;
      }
   } else if (bumper_leftProx) {
    in_avoid = 1;
    avoid_output_flag=1;
    avoid_command= RIGHT_ARC;
    // BluetoothSerial.println("Object On Left: Avoid");
  } else if (bumper_rightProx) {
    in_avoid = 1;
    avoid_output_flag=1;
    avoid_command= LEFT_ARC;
    // BluetoothSerial.println("Object On Right: Avoid");
  } else if (bumper_frontrightProx) { 
    in_avoid = 1;
    avoid_output_flag=1;
    avoid_command= LEFT_ARC;
    // BluetoothSerial.println("Object Front Right: Avoid");
  }
  //SHOULD THERE BE A BACKWARD RIGHT TURN???
  else if (bumper_frontleftProx) {
    in_avoid = 1;
    avoid_output_flag=1;
    avoid_command= RIGHT_ARC;
    // BluetoothSerial.println("Object Front Left: Avoid");
  } else {
    // BluetoothSerial.println("Clear: Avoid");
    if (in_avoid) {
      in_avoid = 0;
      avoid_output_flag=1;
      if (avoid_command == LEFT_ARC) {
        avoid_command = LEFT_ARC_DELAYED;
      } else if (avoid_command == RIGHT_ARC) {
      avoid_command = RIGHT_ARC_DELAYED;
      }
    }
    avoid_output_flag=0;   
  }
}


//escape function output command and flag
//bool bumper_frontleftProx,bumper_frontrightProx, bumper_leftProx,bumper_rightProx,bumper_frontProx;


void escape() { 
  mode = "escape";
  bumper_check_escape();
  if (bumper_frontProx) {
    escape_output_flag=1;
    // BluetoothSerial.println("Object In Front: Escape");  
    escape_command=BACKWARD;
  } else if (bumper_leftProx) {
    escape_output_flag=1;
    // BluetoothSerial.println("Object On Left: Escape");
    escape_command=RIGHT_ARC;
  } else if (bumper_rightProx) {
    escape_output_flag=1;
    // BluetoothSerial.println("Object Right: Escape");
    escape_command=LEFT_ARC;
  } else if (bumper_frontrightProx) { 
    escape_output_flag=1;
    // BluetoothSerial.println("Object Front Right: Escape");
    escape_command=DIAG_LEFT;
  }
  //SHOULD THERE BE A BACKWARD RIGHT TURN???
  else if (bumper_frontleftProx) {
    escape_output_flag=1;
    // BluetoothSerial.println("Object Front Left: Escape");
    escape_command=DIAG_RIGHT;
  } else {
    // BluetoothSerial.println("Clear: Escape");
    escape_output_flag=0;   
  }
}

void targetAcquired(){
  //NEEDS TO BE AN AND WHEN CENTERING WORKS

  phototransistorRead();
  // BluetoothSerial.print(ptLeftDist);
  // BluetoothSerial.print(" , ");
  // BluetoothSerial.print(ptMidLeftDist);
  // BluetoothSerial.print(" , ");
  // BluetoothSerial.print(ptMidRightDist);
  // BluetoothSerial.print(" , ");
  // BluetoothSerial.println(ptRightDist);
  BluetoothSerial.println(sonarRead());
  if(((ptMidLeftDist + ptMidRightDist)/2) < 18){ 
    // BluetoothSerial.println("STOPPED");
    target_acquired_flag = 1;
    if ((sonarRead() <= 7.5) || (sonarRead() >= 35)) {
      target_acquired_command = STOP;
      gotBlown += 1;
    } else {
      target_acquired_command = FORWARD;
    }
     //fanRun();
  } else{
    target_acquired_flag = 0;
  }
}

// check flag and select command based on priority 
void arbitrate () {
  if(search_output_flag==1)
  {motor_input=search_command;}
  if(cruise_output_flag==1)
  {motor_input=cruise_command;}
  if(follow_output_flag==1)
  {motor_input=follow_command;}
  if(avoid_output_flag ==1)
  {motor_input=avoid_command;}
  if(escape_output_flag==1)
  {motor_input=escape_command;}
  if(target_acquired_flag==1)
  {motor_input=target_acquired_command;}
  robotMove();                                    
}

//Sensor Checkkssss

void bumper_check_avoid(){
  int front_detect = sonarRead();

  if (analogRead(IR_front_right) > 600){
    // BluetoothSerial.print("Distance Front Right: ");
    // BluetoothSerial.println(analogRead(IR_front_right));
    bumper_frontrightProx = true;
  }else{
    bumper_frontrightProx = false;    
  }

  if(analogRead(IR_front_left) > 600){
    // BluetoothSerial.print("Distance front left: ");
    // BluetoothSerial.println(analogRead(IR_front_left));
    bumper_frontleftProx = true;
  }else{
    bumper_frontleftProx = false;
  }

  if(analogRead(IR_side_left) > 600){
    // BluetoothSerial.print("Distance Left: ");
    // BluetoothSerial.println(analogRead(IR_side_left));
    bumper_leftProx = true;
  }else{
    bumper_leftProx = false;
  }

  if(analogRead(IR_side_right) > 600){
    // BluetoothSerial.print("Distance Right: ");
    // BluetoothSerial.println(analogRead(IR_side_right));
    bumper_rightProx = true;
  }else{
    bumper_rightProx = false;
  }

  
  if(((ptLeftDist > 50 ) && (ptMidLeftDist > 50 ) && (ptMidRightDist > 50 ) && (ptRightDist > 50 )) && (front_detect <= 20)){
    bumper_frontProx = true; 
    // BluetoothSerial.print("Avoid Distance: "); 
    // BluetoothSerial.println(front_detect);
  }else{
    bumper_frontProx = false;
  }
}

void bumper_check_escape(){
  int front_detect = sonarRead();

  if (analogRead(IR_front_right) > 750){
    // BluetoothSerial.print("Distance Front Right: ");
    // BluetoothSerial.println(analogRead(IR_front_right));
    bumper_frontrightProx = true;
  }else{
    bumper_frontrightProx = false;    
  }

  if(analogRead(IR_front_left) > 750){
    // BluetoothSerial.print("Distance front left: ");
    // BluetoothSerial.println(analogRead(IR_front_left));
    bumper_frontleftProx = true;
  }else{
    bumper_frontleftProx = false;
  }

  if(analogRead(IR_side_left) > 750){
    // BluetoothSerial.print("Distance Left: ");
    // BluetoothSerial.println(analogRead(IR_side_left));
    bumper_leftProx = true;
  }else{
    bumper_leftProx = false;
  }

  if(analogRead(IR_side_right) > 750){
    // BluetoothSerial.print("Distance Right: ");
    // BluetoothSerial.println(analogRead(IR_side_right));
    bumper_rightProx = true;
  }else{
    bumper_rightProx = false;
  }
  
  if(((ptLeftDist > 55) && (ptMidLeftDist > 20) && (ptMidRightDist > 45) && (ptRightDist > 55)) && (front_detect <= 10)) {
    bumper_frontProx = true; 
    // BluetoothSerial.print("Escape Distance: "); 
    // BluetoothSerial.println(front_detect);
  }else{
    bumper_frontProx = false;
  }
}

////////////////////////////