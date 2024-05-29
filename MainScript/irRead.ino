float* irRead() {

  int previous_front_right = 0;
  int previous_side_right = 0;
  int previous_front_left = 0;
  int previous_side_left = 0;
  int previousIR_cm = 0;

  
  int current_front_right = 7225.6 * pow(analogRead(IR_front_right), -1.081);//y = 72225.6x^-1.081
  int current_side_right = 1861.7 * pow(analogRead(IR_side_right), -.0963);//y = 1861.7x^-0.963
  int current_front_left = 48776 * pow(analogRead(IR_front_left), -1.395);//y = 48776x^-1.395
  int current_side_left = 2328 * pow(analogRead(IR_side_left), -1);//y = 2328x^-1

 if (abs(current_front_right - previous_front_right) < 5) {
    previousIR_cm = current_front_right;
  } 

  if (abs(current_side_left - previous_side_right) < 5) {
    previousIR_cm = current_side_right;
  } 

  if (abs(current_front_left - previous_front_left) < 5) {
    previousIR_cm = current_front_left;
  } 

  if (abs(current_side_left - previous_side_left) < 5) {
    previousIR_cm = current_side_left;
  } 

   // Allocate array to hold current sensor values
  static float current_sensors[4];
  current_sensors[1] = current_front_right;
  current_sensors[3] = current_side_right;
  current_sensors[0] = current_front_left;
  current_sensors[2] = current_side_left;

  // Return the array of current sensor values
  return current_sensors;

}
