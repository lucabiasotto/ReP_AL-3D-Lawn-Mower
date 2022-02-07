
void Motor_Action_Go_Mowing_Speed() {

  // No Mag speed adjustment active just go full speed
  if (MAG_Speed_Adjustment == 0) Motor_Action_Go_Full_Speed();

  // Adjust wheel speed according to the MAG level
  if (MAG_Speed_Adjustment == 1) {
    if (MAG_Now >= Slow_Speed_MAG) {
      Motor_Action_Go_Full_Speed();
    }

    if (MAG_Now < Slow_Speed_MAG)  {
      Motor_Action_Go_Slow_Speed();
    }
  }
}

void Motor_Action_Go_Full_Speed()     {

  // Full straighgt speed no motr speed ramp up.
  if (Ramp_Motor_ON == 0) {
#if defined(ROBOT_MOWER)
    analogWrite(ENAPin, PWM_MaxSpeed_RH);                       // Speed = 0-255  (255 is max speed). Speed is set in the settings
    analogWrite(ENBPin, PWM_MaxSpeed_LH);                       // Anaolgwirte sends PWM signals Speed = 0-255  (255 is max speed)
#endif


#if defined(ROBOT_AERATOR)
    analogWrite(ENAPin, PWM_MaxSpeed_RH);                       // Speed = 0-255  (255 is max speed). Speed is set in the settings
    analogWrite(ENBPin, PWM_MaxSpeed_LH);
    analogWrite(ENCPin, PWM_MaxSpeed_RH);
    analogWrite(ENDPin, PWM_MaxSpeed_LH);
#endif

    Serial.print(F("Wheel:FULL|"));
  }

  // If Ramp up is achieved just go full speed
  if ((Ramp_Motor_ON == 1) && (Full_Speed_Achieved == 1)) {

#if defined(ROBOT_MOWER)
    analogWrite(ENAPin, PWM_MaxSpeed_RH);                              // Ramp up the motor speed
    analogWrite(ENBPin, PWM_MaxSpeed_LH);                              // Ramp up the motor speed
#endif

#if defined(ROBOT_AERATOR)
    analogWrite(ENAPin, PWM_MaxSpeed_RH);                              // Ramp up the motor speed
    analogWrite(ENBPin, PWM_MaxSpeed_LH);
    analogWrite(ENCPin, PWM_MaxSpeed_RH);
    analogWrite(ENDPin, PWM_MaxSpeed_LH);
#endif

    Serial.print(F("Wheel:R-FULL|"));
  }

  // Ramp motor option
  if ((Ramp_Motor_ON == 1) && (Full_Speed_Achieved == 0)) {

    Serial.print(F("|WRamp: "));

    int Motor_Step = 150;

    while (Motor_Step > 1) {

#if defined(ROBOT_MOWER)
      analogWrite(ENAPin, (PWM_MaxSpeed_RH - Motor_Step));                       // Ramp up the motor speed
      analogWrite(ENBPin, (PWM_MaxSpeed_LH - Motor_Step));                       // Ramp up the motor speed
#endif

#if defined(ROBOT_AERATOR)
      analogWrite(ENAPin, (PWM_MaxSpeed_RH - Motor_Step));                       // Ramp up the motor speed
      analogWrite(ENBPin, (PWM_MaxSpeed_LH - Motor_Step));                       // Ramp up the motor speed
      analogWrite(ENCPin, (PWM_MaxSpeed_RH  - Motor_Step));
      analogWrite(ENDPin, (PWM_MaxSpeed_LH  - Motor_Step));
#endif

      delay(60);
      //Serial.print(F("Step: "));
      //Serial.println(Motor_Step);
      Motor_Step = Motor_Step - 10;
    }

    Serial.print(F("Wheel:S-FULL|"));
    Full_Speed_Achieved = 1;
  }


}

void Motor_Action_Go_Slow_Speed()     {
#if defined(ROBOT_MOWER)
  analogWrite(ENAPin,  PWM_Slow_Speed_RH);                       // Speed = 0-255  (255 is max speed). Speed is set in the settings
  analogWrite(ENBPin,  PWM_Slow_Speed_LH);                       // Anaolgwirte sends PWM signals Speed = 0-255  (255 is max speed)
#endif


#if defined(ROBOT_AERATOR)
  analogWrite(ENAPin,  PWM_Slow_Speed_RH);                       // Speed = 0-255  (255 is max speed). Speed is set in the settings
  analogWrite(ENBPin,  PWM_Slow_Speed_LH);                       // Anaolgwirte sends PWM signals Speed = 0-255  (255 is max speed)
  analogWrite(ENCPin,  PWM_Slow_Speed_RH);                       // Speed = 0-255  (255 is max speed). Speed is set in the settings
  analogWrite(ENDPin,  PWM_Slow_Speed_LH);
#endif

  Serial.print(F("Wheel:SLOW|"));
}

void Motor_Action_GoFullSpeed_Out_Garage()     {
  //Speeds can be changed to give the mower a slight curve when exiting the Garage.

  PWM_MaxSpeed_LH = PWM_MaxSpeed_LH + 20;
  if (PWM_MaxSpeed_LH > 255)  PWM_MaxSpeed_LH = 255;
  if (PWM_MaxSpeed_RH > 255)  PWM_MaxSpeed_RH = 255;

#if defined(ROBOT_MOWER)
  analogWrite(ENAPin, PWM_MaxSpeed_RH);                                       // Speed = 0-255  (255 is max speed). Speed is set in the settings
  analogWrite(ENBPin, PWM_MaxSpeed_LH);
#endif

#if defined(ROBOT_AERATOR)
  analogWrite(ENAPin, PWM_MaxSpeed_RH);                                       // Speed = 0-255  (255 is max speed). Speed is set in the settings
  analogWrite(ENBPin, PWM_MaxSpeed_LH);
  analogWrite(ENCPin, PWM_MaxSpeed_RH);                       // Speed = 0-255  (255 is max speed). Speed is set in the settings
  analogWrite(ENDPin, PWM_MaxSpeed_LH);
#endif

  Serial.print(F("Wheel:FULL|"));
}


void SetPins_ToGoForwards()     {                                 // Motor Bridge pins are set for both motors to move forwards.

#if defined(ROBOT_MOWER)
  digitalWrite(IN1Pin, LOW);                                      // Motor Birdge pins are set to high or low to set the direction of movement
  digitalWrite(IN2Pin, HIGH);
  digitalWrite(IN3Pin, LOW);
  digitalWrite(IN4Pin, HIGH);
#endif

#if defined(ROBOT_AERATOR)
  digitalWrite(IN1Pin, LOW);                                      //
  digitalWrite(IN2Pin, HIGH);

  digitalWrite(IN3Pin, LOW);
  digitalWrite(IN4Pin, HIGH);

  digitalWrite(IN5Pin, LOW);
  digitalWrite(IN6Pin, HIGH);

  digitalWrite(IN7Pin, LOW);
  digitalWrite(IN8Pin, HIGH);
#endif


  Serial.print(F("Wheel:For|"));
}


void SetPins_ToGoBackwards()      {                               // Motor Bridge pins are set for both motors to move Backwards
#if defined(ROBOT_MOWER)
  digitalWrite(IN1Pin, HIGH);                                     // Motor 1
  digitalWrite(IN2Pin, LOW);
  digitalWrite(IN3Pin, HIGH);                                     // Motor 2
  digitalWrite(IN4Pin, LOW);
#endif

#if defined(ROBOT_AERATOR)
  digitalWrite(IN1Pin, HIGH);                                     // Motor 1
  digitalWrite(IN2Pin, LOW);

  digitalWrite(IN3Pin, HIGH);                                     // Motor 2
  digitalWrite(IN4Pin, LOW);

  digitalWrite(IN5Pin, HIGH);
  digitalWrite(IN6Pin, LOW);

  digitalWrite(IN7Pin, HIGH);
  digitalWrite(IN8Pin, LOW);
#endif

  Serial.print(F("Wheel:Rev|"));
  delay(20);
}


void Motor_Action_Stop_Motors()  {                                     // Motor Bridge pins are set for both motors to stop
#if defined(ROBOT_MOWER)
  digitalWrite(ENAPin, 0);
  digitalWrite(IN1Pin, LOW);                                    //Motor 1
  digitalWrite(IN2Pin, LOW);
  digitalWrite(ENBPin, 0);                                      //Motor 2
  digitalWrite(IN3Pin, LOW);
  digitalWrite(IN4Pin, LOW);
#endif

#if defined(ROBOT_AERATOR)
  digitalWrite(ENAPin, 0);
  digitalWrite(IN1Pin, LOW);                                    //Motor 1
  digitalWrite(IN2Pin, LOW);
  digitalWrite(ENBPin, 0);                                      //Motor 2
  digitalWrite(IN3Pin, LOW);
  digitalWrite(IN4Pin, LOW);
  digitalWrite(ENCPin, 0);
  digitalWrite(IN5Pin, LOW);
  digitalWrite(IN6Pin, LOW);
  digitalWrite(ENDPin, 0);
  digitalWrite(IN7Pin, LOW);
  digitalWrite(IN8Pin, LOW);
#endif


  Serial.print(F("Wheel:0FF|"));

  Full_Speed_Achieved = 0;
}


void SetPins_ToTurnLeft()       {                              // Pins are set so that Motors drive in opposite directions
#if defined(ROBOT_MOWER)
  digitalWrite(IN1Pin, LOW);                                   // Motor 1
  digitalWrite(IN2Pin, HIGH);
  digitalWrite(IN3Pin, HIGH);                                  // Motor 2
  digitalWrite(IN4Pin, LOW);
#endif

#if defined(ROBOT_AERATOR)
  digitalWrite(IN1Pin, HIGH);                                   // Motor 1
  digitalWrite(IN2Pin, LOW);

  digitalWrite(IN3Pin, LOW);                                  // Motor 2
  digitalWrite(IN4Pin, HIGH);

  digitalWrite(IN5Pin, LOW);
  digitalWrite(IN6Pin, HIGH);

  digitalWrite(IN7Pin, HIGH);
  digitalWrite(IN8Pin, LOW);
#endif

  Serial.print(F("Wheel:TL_|"));
}


void SetPins_ToTurnRight() {                                    // Pins are set so that Motors drive in opposite directions
#if defined(ROBOT_MOWER)
  digitalWrite(IN1Pin, HIGH);                                   // Motor 1
  digitalWrite(IN2Pin, LOW);
  digitalWrite(IN3Pin, LOW);                                    //Motor 2
  digitalWrite(IN4Pin, HIGH);
#endif

#if defined(ROBOT_AERATOR)
  digitalWrite(IN1Pin, LOW);                                   // Motor 1
  digitalWrite(IN2Pin, HIGH);

  digitalWrite(IN3Pin, HIGH);                                    //Motor 2
  digitalWrite(IN4Pin, LOW);

  digitalWrite(IN5Pin, HIGH);
  digitalWrite(IN6Pin, LOW);

  digitalWrite(IN7Pin, LOW);
  digitalWrite(IN8Pin, HIGH);
#endif

  Serial.print(F("Wheel:R|"));
}


// USed to turn the mower at a set speed.
void Motor_Action_Turn_Speed() {
#if defined(ROBOT_MOWER)
  analogWrite(ENAPin, (PWM_MaxSpeed_RH - Turn_Adjust) );                                  // Change the 0 value to 10 or 20 to recuce the speed
  analogWrite(ENBPin, (PWM_MaxSpeed_LH - Turn_Adjust) );                                  // Change the 0 value to 10 or 20 to recuce the speed
#endif

#if defined(ROBOT_AERATOR)
  analogWrite(ENAPin, (PWM_MaxSpeed_RH - Turn_Adjust) );                                  // Change the 0 value to 10 or 20 to recuce the speed
  analogWrite(ENBPin, (PWM_MaxSpeed_LH - Turn_Adjust) );                                  // Change the 0 value to 10 or 20 to recuce the speed
  analogWrite(ENCPin, (PWM_MaxSpeed_LH - Turn_Adjust) );                                  // Change the 0 value to 10 or 20 to recuce the speed
  analogWrite(ENDPin, (PWM_MaxSpeed_RH - Turn_Adjust) );                                  // Change the 0 value to 10 or 20 to recuce the speed
#endif
}


// Turns the mowing blades on
void Motor_Action_Spin_Blades()  {

  if (Robot_Type == 1) {
    if (Cutting_Blades_Activate == 1) {                                       // Blades are turn ON in settings and will spin!
      delay(20);
      digitalWrite(R_EN, HIGH);
      digitalWrite(L_EN, HIGH);
      delay(20);
      analogWrite(RPWM, PWM_Blade_Speed);
      delay(20);
      Serial.print(F("Blades:ON_|"));
    }

    if (Cutting_Blades_Activate == 0) {                                     // Blades are turn off in settings and will not spin!
      void StopSpinBlades();

    }
  }
}


void Motor_Action_Stop_Spin_Blades()  {

  if (Robot_Type == 1) {
    delay(20);
    digitalWrite(R_EN, LOW);
    digitalWrite(L_EN, LOW);
    delay(20);
    Serial.print(F("Blades:0FF|"));
  }
}


//Steers the Mower depending on the PID input from the Algorythm
void Motor_Action_Dynamic_PWM_Steering() {
#if defined(ROBOT_MOWER)
  analogWrite(ENAPin, PWM_Right);                             // ENA low = Right Swerve   ENB low = Left Swerve
  analogWrite(ENBPin, PWM_Left);
#endif

#if defined(ROBOT_AERATOR)
  analogWrite(ENAPin, PWM_Right);                             // ENA low = Right Swerve   ENB low = Left Swerve
  analogWrite(ENBPin, PWM_Left);
  analogWrite(ENCPin, PWM_Right);                              // ENA low = Right Swerve   ENB low = Left Swerve
  analogWrite(ENDPin, PWM_Left);
#endif

  Serial.print(F("PWM_R:"));
  Serial.print(PWM_Right);
  Serial.print(F("|"));
  Serial.print(F("PWM_L:"));
  Serial.print(PWM_Left);
  Serial.print(F("|"));
}



void Set_Mecanum_Forwards_Left_Front() {
  #if defined(ROBOT_AERATOR)
  digitalWrite(IN7Pin, LOW);                                      //
  digitalWrite(IN8Pin, HIGH);
  #endif
  }

void Set_Mecanum_Backwards_Left_Front() {
  #if defined(ROBOT_AERATOR)
  digitalWrite(IN7Pin, HIGH);                                      //
  digitalWrite(IN8Pin, LOW);
  #endif
  }

void Set_Mecanum_Forwards_Right_Front() {
  #if defined(ROBOT_AERATOR)
  digitalWrite(IN5Pin, LOW);                                      //
  digitalWrite(IN6Pin, HIGH);
  #endif
  }

void Set_Mecanum_Backwards_Right_Front() {
  #if defined(ROBOT_AERATOR)
  digitalWrite(IN5Pin, HIGH);                                      //
  digitalWrite(IN6Pin, LOW);
  #endif
  }


void Set_Mecanum_Forwards_Left_Rear() {
  #if defined(ROBOT_AERATOR)
  digitalWrite(IN1Pin, LOW);                                      //
  digitalWrite(IN2Pin, HIGH);
  #endif
  }

void Set_Mecanum_Backwards_Left_Rear() {
  #if defined(ROBOT_AERATOR)
  digitalWrite(IN1Pin, HIGH);                                      //
  digitalWrite(IN2Pin, LOW);
  #endif
  }

void Set_Mecanum_Forwards_Right_Rear() {
  #if defined(ROBOT_AERATOR)
  digitalWrite(IN3Pin, LOW);                                      //
  digitalWrite(IN4Pin, HIGH);
  #endif
  }

void Set_Mecanum_Backwards_Right_Rear() {
  #if defined(ROBOT_AERATOR)
  digitalWrite(IN3Pin, HIGH);                                      //
  digitalWrite(IN4Pin, LOW);
  #endif
  }

void Full_Speed_Mecanum_Left_Rear() {
  #if defined(ROBOT_AERATOR)
  analogWrite(ENAPin, PWM_MaxSpeed_RH); 
  #endif
  }

void Full_Speed_Mecanum_Right_Rear() {
  #if defined(ROBOT_AERATOR)
  analogWrite(ENBPin, PWM_MaxSpeed_RH); 
  #endif
  }

void Full_Speed_Mecanum_Left_Front() {
  #if defined(ROBOT_AERATOR)
  analogWrite(ENDPin, PWM_MaxSpeed_RH); 
  #endif
  }

void Full_Speed_Mecanum_Right_Front() {
  #if defined(ROBOT_AERATOR)
  analogWrite(ENCPin, PWM_MaxSpeed_RH); 
  #endif
  }

void Mecanum_Side_Movement_Left() {
  #if defined(ROBOT_AERATOR)
  Set_Mecanum_Backwards_Left_Front();
  Set_Mecanum_Forwards_Right_Front();
  Set_Mecanum_Forwards_Left_Rear();
  Set_Mecanum_Backwards_Right_Rear();
  #endif
  }


void Mecanum_Side_Movement_Right() {
  #if defined(ROBOT_AERATOR)
  Set_Mecanum_Forwards_Left_Front();
  Set_Mecanum_Backwards_Right_Front();
  Set_Mecanum_Backwards_Left_Rear();
  Set_Mecanum_Forwards_Right_Rear();
  #endif
  }

void Mecanum_Diagonal_Movement_Left() {
  #if defined(ROBOT_AERATOR)
  Motor_Action_Stop_Motors();
  Set_Mecanum_Forwards_Right_Front();
  Set_Mecanum_Forwards_Left_Rear();
  #endif
  }


void Mecanum_Diagonal_Movement_Right() {
  #if defined(ROBOT_AERATOR)
  Motor_Action_Stop_Motors();
  Set_Mecanum_Forwards_Left_Front();
  Set_Mecanum_Forwards_Right_Rear();
  #endif
  }

void Mecanum_Rear_Steer_Right() {
  #if defined(ROBOT_AERATOR)
  Motor_Action_Stop_Motors();
  Set_Mecanum_Forwards_Left_Front();
  Set_Mecanum_Backwards_Right_Front();
  #endif
  }


void Mecanum_Rear_Steer_Left() {
  #if defined(ROBOT_AERATOR)
  Motor_Action_Stop_Motors();
  Set_Mecanum_Backwards_Left_Front();
  Set_Mecanum_Forwards_Right_Front();
  #endif
  }
