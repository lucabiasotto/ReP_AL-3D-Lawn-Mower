//https://www.benripley.com/diy/arduino/three-ways-to-read-a-pwm-signal-with-arduino/


void Run_PIXHAWK_Motor_Test() {
  Check_Motor_PWM_LH();
  
  if (PIXHAWK_PWM_Value_LH >= 1600) {
    Serial.print("LH Motor ON  - PWM_P = ");
    Serial.print(PIXHAWK_PWM_Value_LH);
    Calculate_PWM_Arduino_Output_LH();
    Serial.print("   PWM_A= ");
    Serial.print(PWM_Arduino_LH);    
  }
  if (PIXHAWK_PWM_Value_LH < 1600) {
    Serial.print("LH Motor OFF - PWM_P = ");
    Serial.print(PIXHAWK_PWM_Value_LH);
    Calculate_PWM_Arduino_Output_LH();
    Serial.print("   PWM_A= ");
    Serial.print(PWM_Arduino_LH);    
  }

  Check_Motor_PWM_RH();

  if (PIXHAWK_PWM_Value_RH >= 1600) {
    Serial.print("  |  RH Motor ON  - PWM_Pix = ");
    Serial.print(PIXHAWK_PWM_Value_RH);
    Calculate_PWM_Arduino_Output_RH();
    Serial.print("   PWM_Ard = ");
    Serial.print(PWM_Arduino_RH);    
  }
  if (PIXHAWK_PWM_Value_RH < 1600) {
    Serial.print("  |  RH Motor OFF - PWM_Pix = ");
    Serial.print(PIXHAWK_PWM_Value_RH);
    Calculate_PWM_Arduino_Output_RH();
    Serial.print("   PWM_Ard = ");
    Serial.print(PWM_Arduino_RH);    
  }

  Serial.print("  |  ");
  PWM_Left = PWM_Arduino_LH;
  PWM_Right = PWM_Arduino_RH;
  Motor_Action_Dynamic_PWM_Steering();
  Serial.println("");
}

 


void Check_Motor_PWM_LH() {
  PIXHAWK_PWM_Value_LH = pulseIn(PIXHAWK_LH_PWM, HIGH);
  delay(10);
  }

void Check_Motor_PWM_RH() {  
  PIXHAWK_PWM_Value_RH = pulseIn(PIXHAWK_RH_PWM, HIGH);
  delay(10);
}


 

void Calculate_PWM_Arduino_Output_LH() {
  PWM_Arduino_LH = (PIXHAWK_PWM_Value_LH - 1500);
  PWM_Arduino_LH = (PWM_Arduino_LH / 400) * 255;
  if (PWM_Arduino_LH > 255) PWM_Arduino_LH = 255;
  if (PWM_Arduino_LH < 5) PWM_Arduino_LH = 0;
}

void Calculate_PWM_Arduino_Output_RH() {
  PWM_Arduino_RH = (PIXHAWK_PWM_Value_RH - 1500);
  PWM_Arduino_RH = (PWM_Arduino_RH / 400) * 255;
  if (PWM_Arduino_RH > 255) PWM_Arduino_RH = 255;
  if (PWM_Arduino_RH < 5) PWM_Arduino_RH = 0;
}
