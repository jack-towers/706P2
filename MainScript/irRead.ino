//Front Right
//ir_Unamed_short
int IR_front_right = A10;

//Back Right
//ir_Y_short
int IR_back_right = A11;

//Front Left
//ir_A_Long
int IR_front_left = A8;

//Back left
//ir_B_Long
int IR_back_left = A9;


float irRead() {

  int previous_front_right = 0;
  int previous_back_right = 0;
  int previous_front_left = 0;
  int previous_back_left = 0;
  int previousIR_cm = 0;

  
  int current_front_right = 2227.4 * pow(analogRead(IR_front_right), -0.981);
  int current_back_right = 62843.2 * pow(analogRead(IR_back_right), -1.111);
  int current_front_left = 6336.7 * pow(analogRead(IR_front_left), -1.065);
  int current_back_left = 5259.9 * pow(analogRead(IR_back_left), -0.978);

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
