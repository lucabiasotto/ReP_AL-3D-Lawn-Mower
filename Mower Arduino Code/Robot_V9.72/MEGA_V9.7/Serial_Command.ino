void Check_Serial_Input() {
  
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    int Command_Last = Command;
    Command = Serial.read();
    if (Command == 10) Command = Command_Last;

    // Print Comamnd to Serial Monitor
    if (Command != 0) {
        Serial.print(F(""));
        Serial.print(F(""));
        Serial.println(Command);
        }
  }

Execute_Serial_Command();
}


void Execute_Serial_Command() {
// Rotate 20 degrees CCW

if (Command == 104) {                   // letter r
    Command = 0;
    Serial.println(F("  "));
    Serial.println(F("  "));
    Serial.println(F("        HELP MENU"));
    Serial.println(F("      Enter the following letter"));
    Serial.println(F("  --------------------"));
    Serial.println(F("  a = Send PWM Signal on A7"));
    Serial.println(F("  b"));
    Serial.println(F("  c = Goto Charging Station")); //99
    Serial.println(F("  d = Set to Docked"));         // 100                
    Serial.println(F("  e = Exit dock"));             //101
    Serial.println(F("  f"));
    Serial.println(F("  g"));
    Serial.println(F("  h = Help Menu"));             //104
    Serial.println(F("  i = Drill ON/OFF"));          // 105
    Serial.println(F("  j"));
    Serial.println(F("  k = Test Drill Cycle"));      //107
    Serial.println(F("  l = Spikes Down"));           //108
    Serial.println(F("  m = Manuel Mode"));
    Serial.println(F("  n"));
    Serial.println(F("  o = Spikes UP"));             //111
    Serial.println(F("  p = Pause/Park Mower"));      //112
    Serial.println(F("  q = Quick Start Mower"));     //113
    Serial.println(F("  r = Rain Sensor ON/OFF"));    //114
    Serial.println(F("  s"));
    Serial.println(F("  t"));
    Serial.println(F("  u"));
    Serial.println(F("  v = Fake Voltage of 12.6V"));                         //118
    Serial.println(F("  w = Fake Wire Signal ON/OFF"));
    Serial.println(F("  x"));
    Serial.println(F("  y"));
    Serial.println(F("  z = Fake All Volts, Wheel Amps "));    //122"));
    Serial.println(F("  "));
    Serial.println(F("  "));

        
    delay(7000);   
    }

if (Command == 96) {                   // letter c
    Command = 0;
    analogWrite(PWM_1, 255);
    }

if (Command == 99) {                   // letter c
    Command = 0;
    Manouver_Go_To_Charging_Station();
    }

if (Command == 100) {                   // letter d
    Command = 0;
    Mower_Docked = 1 ;
    Mower_Parked = 0 ;
    }

if (Command == 101) {                   // letter e
    Command = 0;
    Alarm_Start_Exit_Zone_2();
    }


if (Command == 105) {                   // letter i
    Command = 0;
    int Skip = 0;

    
    if (Drill_ON == 0) {
      Serial.println(F(""));
      Serial.println(F("Drill = ON"));
      Turn_On_Relay();
      delay(1000);
      Motor_Action_Spin_Drill();
      Drill_ON = 1;
      Skip = 1;
      }
    
    
    if ((Drill_ON == 1) && (Skip == 0)) {
      Serial.println(F(""));
      Serial.println(F("Drill = OFF"));
      Motor_Action_Stop_Drill();
      Turn_Off_Relay();
      Drill_ON = 0;
      
      
      }
    

    }

//Test FULL Drill Cycle
// press 'Space Bar' and 'Return' to Stop Cycle

//###########################
if (Command == 107) {                   // letter k
    Command = 0;
    Initiate_Drill_Cycle();
    Turn_Off_Relay();  
    }





// Go UP Spikes
if (Command == 111) {                   // letter o
    Command = 0;
    Setup_Microswitches();
    int cycles = 0;
    bool Stop = 0;
    Turn_On_Relay();
    delay(2000);
    Check_End_Stops();

  
   while ((End_Stop_Upper == false) && (cycles < 13000) && (Stop == 0)) {
      Spikes_Up();      
      Check_End_Stops();
      cycles ++;
      Serial.print("Cycles:");
      Serial.println(cycles);
      Check_Serial_Input();
      if (Command == 32) Stop = 1;
      }
    
    Command = 0;
    Motor_Action_Stop_Spikes();
    Turn_Off_Relay();
    }


// Go DOWN Spikes
if (Command == 108) {                   // letter l
    Command = 0;
    Setup_Microswitches();
    
    int cycles = 0;
    bool Stop = 0;
    
    Turn_On_Relay();
    delay(2000);
    Check_End_Stops();

    while ((End_Stop_Lower == false) && (cycles < 12000) && (Stop == 0)) {
      Spikes_Down();      
      Check_End_Stops();
      cycles ++;
      Serial.print("Cycles:");
      Serial.println(cycles);
      Check_Serial_Input();
      if (Command == 32) Stop = 1;
      }
    
    Command = 0;
    Motor_Action_Stop_Spikes();
    Turn_Off_Relay();
    }


if (Command == 109) {                   // letter m  
    Command = 0;
    Manouver_Park_The_Mower(); 
    delay(100);
    Manouver_Manuel_Mode(); 
    Turn_On_Relay();
    }

if (Command == 112) {                   // letter p  
    Command = 0;
    Manouver_Park_The_Mower();
    }

if (Command == 113) {                   // letter q  
    Command = 0;
    if (Mower_Docked == 1) {
      Serial.println(F("Overriding Docked Status - Starting Mower"));
      delay(2000);
      Mower_Docked = 0;
      }
    Alarm_Start_Quick_Go();
    }


if (Command == 114) {                   // letter r
    Command = 0;
    if (Rain_Sensor_Installed == 0) {
      Rain_Sensor_Installed = 1;
      Serial.println(F("Rain Sensor Enabled = ON"));
      }
    if (Rain_Sensor_Installed == 1) {
      Rain_Sensor_Installed = 0;
      Serial.println(F("Rain Sensor Enabled = OFF"));
      }
    }


if (Command == 118) {                   // letter v  
    Command = 0;
    }

if (Command == 119) {                   // letter w
    Command = 0;
    bool Skip = 0;
    if (Fake_Wire == 0) {
      Fake_Wire = 1;
      Serial.println(F("Fake_Wire Signal Enabled = ON"));
      Skip = 1;
      }
    if ((Fake_Wire == 1) && (Skip == 0)) {
      Fake_Wire = 0;
      Serial.println(F("Fake_Wire Signal Enabled = OFF"));
      }
    }


if (Command == 122) {                   // letter v
    Command = 0;
    int Skip = 0;

    
    if (Fake_All_Settings == 0) {
      Serial.println(F(""));
      
      Wheel_Amp_Sensor_ON = 0;      
      Fake_All_Settings = 1;     

      Serial.println(F(""));
      Serial.println(F("Fake Volts Enabled = ON"));
      Serial.println(F("Wheel Amp Sensor Disabled"));
      Serial.println(F("Fake Settings ON !!!"));
      Serial.println(F(""));
      Skip = 1;
      }
    
    
    if ((Fake_All_Settings == 1) && (Skip == 0)) {
      Serial.println(F(""));
      
      Perimeter_Wire_Enabled = 1;
      Wheel_Amp_Sensor_ON = 1;
      Fake_All_Settings = 0;
      
      Serial.println(F(""));
      Serial.println(F("All Sensors normal status"));
      Serial.println(F(""));
      }
    

    }

}





void Check_Mower_Status() {
      Serial.println("");
      Serial.print(F("Mower Status:  "));
      Serial.print(F("Parked ="));
      Serial.print(Mower_Parked);
      Serial.print(F(" | Docked = "));
      Serial.print(Mower_Docked);
      Serial.print(F(" | Tracking = "));
      Serial.print(Tracking_Wire);
      Serial.print(F(" |  Exit Dock = "));
      Serial.print(Exiting_Dock);
      Serial.print(F(" |  Error Value = "));
      Serial.print(Mower_Error_Value);
      Serial.println("");
      } 
