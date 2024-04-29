// read simulative sensor reading   
void serial_read_conditions()
{
 char in_data[8];
  if(Serial.available()>0)
          {
                for(int k = 0; k < 9; k++)    
                {
                   char c = Serial.read();
                   in_data[k] = c;
                   delay(2);
                   Serial.flush();
                }
    int temp0 = in_data[0]-'0';
    int temp1 = in_data[1]-'0';
    int temp2 = in_data[2]-'0';
    int temp3 = in_data[3]-'0';
    int temp4 = in_data[4]-'0';
    int temp5 = in_data[5]-'0';
    int temp6 = in_data[6]-'0';
    int temp7 = in_data[7]-'0';
    
    photo_left = 10*temp0 + temp1;    
    photo_right = 10*temp2 + temp3;
    ir_detect = temp4;
    bumper_left = temp5;
    bumper_right = temp6;
    bumper_back = temp7;
  }
}
