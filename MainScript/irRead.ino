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


float irRead() {

  int previous_front_right = 0;
  int previous_back_right = 0;
  int previous_front_left = 0;
  int previous_back_left = 0;
  int previousIR_cm = 0;

  
  int current_front_right = 7225.6 * pow(analogRead(IR_front_right), -1.081);//y = 72225.6x^-1.081
  int current_side_right = 1861.7 * pow(analogRead(IR_back_right), -1.0.963);//y = 1861.7x^-0.963
  int current_front_left = 48776 * pow(analogRead(IR_front_left), -1.395);//y = 48776x^-1.395
  int current_side_left = 2328 * pow(analogRead(IR_back_left), -1);//y = 2328x^-1

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
