
void Spikes_Up()     {                                              // Spikes Motor Bridge p      
      #if defined(ROBOT_AERATOR)
      Serial.print(F("Spike:UP|"));
      digitalWrite(IN9Pin, LOW);                                      // Motor Birdge pins are set to high or low to set the direction of movement
      digitalWrite(IN10Pin, HIGH);
      analogWrite (ENEPin,  200);
      #endif
      
      }


void Spikes_Down()     {                                            // Spike Motor Bridge pins 
      #if defined(ROBOT_AERATOR)
      Serial.print(F("Spike:DOWN|"));
      digitalWrite(IN9Pin, HIGH);                                      // Motor Birdge pins are set to high or low to set the direction of movement
      digitalWrite(IN10Pin, LOW);
      analogWrite (ENEPin,  200); 
      #endif
      }


void Motor_Action_Stop_Spikes() {
      #if defined(ROBOT_AERATOR)
      digitalWrite(ENEPin, 0);
      digitalWrite(IN9Pin, LOW);                                    //Motor 1
      digitalWrite(IN10Pin, LOW);  
      #endif
      }


// Turns the mowing blades on
void Motor_Action_Spin_Drill()  {
  
        delay(20);
        digitalWrite(R_EN, HIGH);
        digitalWrite(L_EN, HIGH);
        delay(20);
        analogWrite(RPWM, PWM_Blade_Speed);
        delay(20);
        Serial.print(F("Drill:ON_|"));   
        }


void Motor_Action_Stop_Drill()  {
  
  
  delay(20);
  digitalWrite(R_EN, LOW);
  digitalWrite(L_EN, LOW);
  delay(20);
  Serial.print(F("Drill:0FF|"));
  }




void Initiate_Drill_Cycle() {
    Serial.println(F(""));
    Serial.println(F("Starting Drill Cycle"));
    Setup_Microswitches();
    Read_Serial1_Nano(); 
    
    int cycles = 0;
    
    Stop = 0;
    
    
    Drill_Status = 1;
    Send_Aerator_Running_Data();
    
    Turn_On_Relay();
    delay(1000);
    Check_End_Stops();
    Motor_Action_Spin_Drill();
    Drill_ON = 1;
    delay(1000);

    while ((End_Stop_Lower == false) && (cycles < max_cycles) && (Stop == 0) && (Drill_Blocked != 4)) {
      Spikes_Down();      
      Check_End_Stops();
      cycles ++;
      Serial.print("Cycles:");
      Serial.println(cycles);
      Check_Serial_Input();
      if (Command == 32) Stop = 1;     
      delay(200);
      Read_Serial1_Nano();  
      Check_Drill_Amps(); 
      if (TFT_Screen_Menu == 1) Check_TFT_Serial_Input();
      }
    
    // maybe peck with the drill?
    
    Motor_Action_Stop_Spikes();
    Motor_Action_Stop_Drill();                    // Stop the drill motor to save amps and prevent heating
    if (Stop == 0) delay(2000);
    cycles = 0;
    Drill_Status = 2;
    Send_Aerator_Running_Data();
    Drill_ON = 1;
    
    while ((End_Stop_Upper == false) && (cycles < max_cycles) && (Stop == 0)) {

      Spikes_Up();      
      Check_End_Stops();
      cycles ++;
      if ((cycles > 5) && (Drill_ON == 1)) {
        Motor_Action_Spin_Drill();     // restart motor to aid retraction
        Drill_ON = 1;
        }
      if (cycles > 25) {
        Motor_Action_Stop_Drill();    // Stop the drill motor to save amps and prevent heating
        Drill_ON = 0;
        }
      Serial.print("Cycles:");
      Serial.println(cycles);
      Check_Serial_Input();
      if (Command == 32) Stop = 1;
      delay(200);
      Read_Serial1_Nano();  
      Check_Drill_Amps(); 
      if (TFT_Screen_Menu == 1) Check_TFT_Serial_Input();
      }
    
    Motor_Action_Stop_Spikes();
    if (Stop == 0) delay(1000);
    Motor_Action_Stop_Drill();
    Drill_ON = 0;
    Drill_Status = 0;
    Send_Aerator_Running_Data();
    
    if (Stop == 0) delay(1000);
    //Turn_Off_Relay();    
}


void Check_End_Stops() {

  
  
  if (digitalRead(Microswitch_1))   End_Stop_Upper = false;    // The switch is not activated the variable Bump is false
  if (!digitalRead(Microswitch_1))  End_Stop_Upper = true;     // The switch is activated the variable Bump is true
  
  if (digitalRead(Microswitch_2))   End_Stop_Lower = false;    // The switch is not activated the variable Bump is false
  if (!digitalRead(Microswitch_2))  End_Stop_Lower = true;     // The switch is activated the variable Bump is true
  
  if (End_Stop_Lower == true )   {
    Serial.println(F(""));
    Serial.println(F("Lower End Stop Activated"));
    }
  if (End_Stop_Upper == true )   {
    Serial.println(F(""));
    Serial.println(F("Top End Stop Activated"));
    }
  }


void Calculate_Drill_Amps() {
  // Calculate Amp Value from Drill Amp sensor
 int mVperAmp = 185;
 int ACSoffset = 2500; 
 double VoltageAmp = 0;
 //double Amps = 0;
 VoltageAmp = (RawDrillAmp / 1024.0) * 5000; // Gets you mV
 DrillAmps =  ((VoltageAmp - ACSoffset) / mVperAmp);
 if (DrillAmps < 0) DrillAmps = DrillAmps * - 1;
 if (DrillAmps > 10) DrillAmps = 0;
 Serial.print(("WA:"));
 Serial.print(DrillAmps);
 Serial.print(F("|"));

 }


void Check_Drill_Amps() {
  Calculate_Drill_Amps();
  if (Drill_Amp_Sensor_ON == 1) {
           Serial.print(F("D_Amps:"));
           Serial.print(DrillAmps);
           Serial.print(F("|Max_D_Amps:"));
           Serial.print(Max_Drill_Amps);   
           Serial.print(F("|Drill_Block:"));
           Serial.print(Drill_Blocked_Count);
           Serial.print(F("|"));
      
        if (DrillAmps >= Max_Drill_Amps) Drill_Blocked_Count = Drill_Blocked_Count + 1;            
        else {
          Drill_Blocked = 0;
          Drill_Blocked_Count = 0;
          }
          
        if (Drill_Blocked_Count > Drill_Blocked_Count_Max) {                     
                Serial.println(F("!! Drill_Blocked !!"));
                Serial.print(F("|")); 
                Drill_Blocked = 4;                
                }
           }

   
  }



void Ensure_Drills_Are_Restarcted() {
      
      Serial.println(F("Ensuring Drills are Retracted"));
      Drill_Status = 2;
      Send_Aerator_Running_Data();
      Check_End_Stops();

      if (End_Stop_Upper == false) {
        while ((End_Stop_Upper == false) && (Stop == 0)) {
            Spikes_Up();      
            Check_End_Stops();
            Check_Serial_Input();
            if (Command == 32) Stop = 1;
            delay(200);
            Read_Serial1_Nano();  
            Check_Drill_Amps(); 
            if (TFT_Screen_Menu == 1) Check_TFT_Serial_Input();
            }
        
      }


      Drill_Status = 0;
      Send_Aerator_Running_Data();
}
