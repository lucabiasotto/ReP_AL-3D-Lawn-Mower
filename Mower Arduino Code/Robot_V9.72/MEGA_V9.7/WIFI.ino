// Blynk TX and RX functions to communicate with the MEGA and NODEMCU
// Each transmission has a label n or p or q or 

// USE V5 Blynk  12th Feb 2019

void Get_WIFI_Commands() {
  
  if (WIFI_Enabled == 1) {
      Receive_Data_From_NODEMCU(); 
      delay(5); 
      Transmit_All_To_NODEMCU(); 
      }
  }
void Receive_Data_From_NODEMCU() {
  while(Serial2.available()>0){
      val_WIFI = Serial2.parseInt();
      if(Serial2.read()== '\p'){
        delay(1000);
        Execute_Blynk_Command_To_Mower();
        }
  }
 }

void Receive_WIFI_Manuel_Commands() {
  while(Serial2.available()>0){
      val_WIFI = Serial2.parseInt();
      if(Serial2.read()== '\p'){
        delay(5);
        Execute_Manuel_Blynk_Command_To_Mower();
        }
  }
 }

void Transmit_All_To_NODEMCU() {
  delay(5);
  Serial2.print(Volts);
  Serial2.println("\g");
  delay(5);
  Serial2.print(Loop_Cycle_Mowing);
  Serial2.println("\c");
  delay(5);
  Serial2.print(Mower_Docked);
  Serial2.println("\d");
  delay(5);
  Serial2.print(Mower_Running);
  Serial2.println("\z");
  delay(5);
  Serial2.print(Mower_Parked);
  Serial2.println("\y");
  delay(5);
  Serial2.println(Charging);
  Serial2.println("\o");
  delay(5);
  Serial2.println(Tracking_Wire);
  Serial2.println("\m");   
  delay(5);
  }


void Transmit_APP_Buttons_Status_1() {

  Serial.println(F(""));
  Serial.print(F("|Updating APP Buttons 1"));
  
  delay(200);
  Serial2.print(Compass_Activate);
  Serial2.println("\a");
  delay(200);
  Serial2.print(Compass_Heading_Hold_Enabled);
  Serial2.println("\b");
  delay(200);
  Serial2.print(GYRO_Enabled);
  Serial2.println("\c");
  delay(200);
  Serial2.print(Sonar_1_Activate);
  Serial2.println("\d");
  delay(200);
  Serial2.print(Sonar_2_Activate);
  Serial2.println("\e");
  delay(200);
  Serial2.print(Sonar_3_Activate);
  Serial2.println("\f");
  delay(200);
  Serial2.print(Bumper_Activate_Frnt);
  Serial2.println("\g");
  delay(200);

  Serial.println("");
  Serial.print(F("Compass_Activate :"));
  Serial.println(Compass_Activate);
  Serial.print(F("Heading Hold TX :"));
  Serial.println(Compass_Heading_Hold_Enabled);  
  Serial.print(F("GYRO TX :"));
  Serial.println(GYRO_Enabled);
  Serial.print(F("Sonar 1 ON :"));
  Serial.println(Sonar_1_Activate);
  Serial.print(F("Sonar 2 ON :"));
  Serial.println(Sonar_2_Activate);
  Serial.print(F("Sonar 3 ON :"));
  Serial.println(Sonar_3_Activate);
  Serial.print(F("Bumper ON:"));
  Serial.println(Bumper_Activate_Frnt);
  Serial.println("");
  }

void Transmit_APP_Buttons_Status_2() {

  Serial.println(F(""));
  Serial.print(F("|Updating APP Buttons 2"));
  
  delay(200);
  Serial2.print(Alarm_1_ON);
  Serial2.println("\a");
  delay(200);
  Serial2.print(Alarm_2_ON);
  Serial2.println("\b");
  delay(200);
  Serial2.print(Alarm_3_ON);
  Serial2.println("\c");
  delay(200);
  Serial2.print(GPS_Enabled);
  Serial2.println("\d");
  delay(200);
  Serial2.print(Angle_Sensor_Enabled);
  Serial2.println("\e");
  delay(200);
  Serial2.print(Tip_Over_Sensor_Enabled);
  Serial2.println("\f");
  delay(200);
  Serial2.print(Wheel_Amp_Sensor_ON);
  Serial2.println("\g");
  delay(200);

  Serial.println("");
  Serial.print(F("Alarm 1 ON :"));
  Serial.println(Alarm_1_ON);
  Serial.print(F("Alarm 2 ON :"));
  Serial.println(Alarm_2_ON);
  Serial.print(F("Alarm 3 ON :"));
  Serial.println(Alarm_3_ON);
  Serial.print(F("GPS Enabled :"));
  Serial.println(GPS_Enabled);
  Serial.print(F("Angle Sensor :"));
  Serial.println(Angle_Sensor_Enabled );
  Serial.print(F("Tip Over Sensor :"));
  Serial.println(Tip_Over_Sensor_Enabled);
  Serial.print(F("Wheel Amp Sensor ON:"));
  Serial.println(Wheel_Amp_Sensor_ON);
  Serial.println("");
  }




void Execute_Blynk_Command_To_Mower() {

// Updates the Serial Monitor with the latest Blynk Commands and can be used to start
// functions on the mower when the command is recieved.
delay(30);


// Exit Dock to Zone 1
 if (val_WIFI == 14) {
       if (Mower_Docked == 1)   {  
    
       // Update the TFT Display
       Serial.println(F(""));
       Serial.println(F("Updating TFT: WIFI Exit dock"));
       Exiting_Dock = 1;
       Mower_Error_Value = 0;
       Send_Mower_Docked_Data();                                   // Send the Docked TX Data package to the mower.
         
       Serial.println(F(""));
       Serial.print(F("WIFI Command: ")); 
       Serial.print(val_WIFI);
       Serial.print(F("Exit Dock| "));
       Serial.println(F("Zone 1"));
       
       #if defined(LCD_KEYPAD)
       lcd.clear();
       lcd.print(F("WIFI Start"));
       lcd.setCursor(0,1);
       lcd.print(F("Exit Dock Z1"));
       delay(500);
       lcd.clear();
       #endif
       
       Exit_Zone = 1;
       Track_Wire_Itterations = Track_Wire_Zone_1_Cycles;
       Manouver_Exit_To_Zone_X();
       }
   if (Mower_Parked == 1) {
    Serial.println(F(""));
    Serial.println(F("Mower is Parked, not docked"));
   }
 }

 // Quick Start Button in Blynk App
 if (val_WIFI == 13) {
   Serial.println(F(""));
   Serial.print(F("WIFI Command:")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Quick Start"));
   
   #if defined(LCD_KEYPAD)
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print(F("WIFI Start"));
   #endif
   
   Serial.println(F("Quick Start"));
   if (Mower_Docked == 0) { 
     Manouver_Start_Mower();
     #if defined(LCD_KEYPAD)
     lcd.clear();   
     #endif 
     if (TFT_Screen_Menu == 1) Send_Mower_Docked_Data();    // Send the Docked TX Data package to the mower.
     }    
   else Serial.println(F("Mower Docked - Quick Start not possible"));   
   }

// Go To Dock Button in Blynk App
 if (val_WIFI == 12) {
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Go To Dock"));
   
   #if defined(LCD_KEYPAD)
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print(F("WIFI Go To Dock"));
   #endif
   
   Menu_Mode_Selection = 0;                                      // Releases the loop in the membrane button section.
   Motor_Action_Stop_Spin_Blades();
   Motor_Action_Stop_Motors();
   delay(1000);
   
   #if defined(LCD_KEYPAD)
   lcd.clear();
   #endif
   
   Manouver_Go_To_Charging_Station();      
   }

// STOP / Cancel Button in Blynk App
 if (val_WIFI == 11)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Pause/Stop"));
   Manouver_Park_The_Mower(); 
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
   }

// Manuel Button in Blynk App
 if (val_WIFI == 15)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Manuel Mode"));
   Manouver_Park_The_Mower(); 
   delay(500);
   Manouver_Manuel_Mode(); 
   Turn_On_Relay(); 
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
   }
   
// Automatic RANDOM Button in Blynk App
 if (val_WIFI == 16)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Automatic Mode RANDOM"));
   
   #if defined(LCD_KEYPAD)
   lcd.clear();
   lcd.print("Auto Random");
   lcd.setCursor(0,1);
   lcd.print("Pattern");
   delay(200);
   lcd.clear();
   #endif
   
   if (Mower_Running == 0) {
    Manouver_Park_The_Mower(); 
    Turn_On_Relay(); 
    }
   Pattern_Mow = 0;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }

// Automatic SPIRAL Button in Blynk App
 if (val_WIFI == 21)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Automatic Mode SPIRAL"));
   
   #if defined(LCD_KEYPAD)
   lcd.clear();
   lcd.print("Auto Spiral");
   lcd.setCursor(0,1);
   lcd.print("Pattern");
   delay(200);
   lcd.clear();
   #endif
   
   if (Mower_Running == 0) {
    Manouver_Park_The_Mower(); 
    Turn_On_Relay(); 
    }
   Pattern_Mow = 2;

   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }

// Automatic Parallel Button in Blynk App
 if (val_WIFI == 22)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Automatic Mode PARALLEL"));
   
   #if defined(LCD_KEYPAD)
   lcd.clear();
   lcd.print("Auto Parallel");
   lcd.setCursor(0,1);
   lcd.print("Pattern");
   delay(200);
   lcd.clear();
   #endif
   
   if (Mower_Running == 0) {
    Manouver_Park_The_Mower(); 
    Turn_On_Relay(); 
    }
   Pattern_Mow = 1;

   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }

// SETTINGS MOWER

// Compass Setting Via WIFI.
 if (val_WIFI == 40)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Compass ON"));
   Compass_Activate = 1;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once

   #if defined(BOARD_DUE)
   dueFlashStorage.write(19 , 1);
   dueFlashStorage.write(20 , Compass_Activate);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(19 , 1);
   EEPROM.write(20 , Compass_Activate);
   Serial.println(F("Saved to EEPROM"));
   #endif
   
   Serial.println(F(" "));
   }
 if (val_WIFI == 41)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Compass OFF"));
   Compass_Activate = 0;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
   
   #if defined(BOARD_DUE)
   dueFlashStorage.write(19 , 1);
   dueFlashStorage.write(20 , Compass_Activate);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(19 , 1);
   EEPROM.write(20 , Compass_Activate);
   Serial.println(F("Saved to EEPROM"));
   #endif
   
   Serial.println(F(" "));
   }

// Heading Hold
 if (val_WIFI == 42)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Heading Hold ON"));
   Compass_Heading_Hold_Enabled = 1;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
   
   #if defined(BOARD_DUE)
   dueFlashStorage.write(59 , 1);
   dueFlashStorage.write(60 , Compass_Heading_Hold_Enabled);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(59 , 1);
   EEPROM.write(60 , Compass_Heading_Hold_Enabled);
   Serial.println(F("Saved to EEPROM"));
   #endif
   
   Serial.println(F(" "));
   }
  
 if (val_WIFI == 43)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Heading Hold OFF"));
   Compass_Heading_Hold_Enabled = 0;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once

   #if defined(BOARD_DUE)
   dueFlashStorage.write(59 , 1);
   dueFlashStorage.write(60 , Compass_Heading_Hold_Enabled);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(59 , 1);
   EEPROM.write(60 , Compass_Heading_Hold_Enabled);
   Serial.println(F("Saved to EEPROM"));
   #endif
   
   Serial.println(F(" "));
   }

// GYRO
 if (val_WIFI == 44)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|GYRO ON"));
   GYRO_Enabled = 1;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once      
   
   #if defined(BOARD_DUE)
   dueFlashStorage.write(109 , 1);
   dueFlashStorage.write(110, GYRO_Enabled);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(109 , 1);
   EEPROM.write(110, GYRO_Enabled);
   Serial.println(F("Saved to EEPROM"));
   #endif
   
   Serial.println(F(" "));
   }
  
 if (val_WIFI == 45)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|GYRO OFF"));
   GYRO_Enabled = 0;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
   
   #if defined(BOARD_DUE)
   dueFlashStorage.write(109 , 1);
   dueFlashStorage.write(110, GYRO_Enabled);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(109 , 1);
   EEPROM.write(110, GYRO_Enabled);
   Serial.println(F("Saved to EEPROM"));
   #endif
   
   Serial.println(F(" "));
   }


// SONAR 1 Setting Via WIFI.
 if (val_WIFI == 47)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|SONAR 1 ON"));
   Sonar_1_Activate = 1;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
   
   #if defined(BOARD_DUE)
   dueFlashStorage.write(37, 1);
   dueFlashStorage.write(38, Sonar_1_Activate);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(37, 1);
   EEPROM.write(38, Sonar_1_Activate);
   Serial.println(F("Saved to EEPROM"));
   #endif
   
   Serial.println(F(" "));
   }
   
 if (val_WIFI == 48)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|SONAR 1 OFF"));
   Sonar_1_Activate = 0;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
   
   #if defined(BOARD_DUE)
   dueFlashStorage.write(37, 1);
   dueFlashStorage.write(38, Sonar_1_Activate);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(37, 1);
   EEPROM.write(38, Sonar_1_Activate);
   Serial.println(F("Saved to EEPROM"));
   #endif
   
   Serial.println(F(" "));
   }


// SONAR 2 Setting Via WIFI.
 if (val_WIFI == 49)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|SONAR 2 ON"));
   Sonar_2_Activate = 1;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
   
   #if defined(BOARD_DUE)
   dueFlashStorage.write(39, 1);
   dueFlashStorage.write(40, Sonar_2_Activate);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(39, 1);
   EEPROM.write(40, Sonar_2_Activate);
   Serial.println(F("Saved to EEPROM"));
   #endif
   
   Serial.println(F(" "));
   }
  
 if (val_WIFI == 50)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|SONAR 2 OFF"));
   Sonar_2_Activate = 0;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
   
   #if defined(BOARD_DUE)
   dueFlashStorage.write(39, 1);
   dueFlashStorage.write(40, Sonar_2_Activate);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(39, 1);
   EEPROM.write(40, Sonar_2_Activate);
   Serial.println(F("Saved to EEPROM"));
   #endif
   
   Serial.println(F(" "));
   }


// SONAR 3 Setting Via WIFI.
 if (val_WIFI == 51)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|SONAR 3 ON"));
   Sonar_3_Activate = 1;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
   
   #if defined(BOARD_DUE)
   dueFlashStorage.write(41, 1);
   dueFlashStorage.write(42, Sonar_3_Activate);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(41, 1);
   EEPROM.write(42, Sonar_3_Activate);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif
   
   Serial.println(F(" "));
   }

  
 if (val_WIFI == 52)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|SONAR 3 OFF"));
   Sonar_3_Activate = 0;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
   
   #if defined(BOARD_DUE)
     dueFlashStorage.write(41, 1);
     dueFlashStorage.write(42, Sonar_3_Activate);
     Serial.println(F("Saved to dueFlashStorage"));
     #endif 

   #if defined(BOARD_MEGA)
     EEPROM.write(41, 1);
     EEPROM.write(42, Sonar_3_Activate);
     Serial.println(F("Saved to dueFlashStorage"));
     #endif 
   
   Serial.println(F(" "));
   }


// Bumper Setting Via WIFI.
 if (val_WIFI == 53)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Bumper ON"));
   Bumper_Activate_Frnt = 1;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once

   #if defined(BOARD_DUE)
     dueFlashStorage.write(90 , 1);
     dueFlashStorage.write(91 , Bumper_Activate_Frnt);
     Serial.println(F("Saved to dueFlashStorage"));
     #endif

   #if defined(BOARD_MEGA)
     EEPROM.write(90 , 1);
     EEPROM.write(91 , Bumper_Activate_Frnt);
     Serial.println(F("Saved to EEPROM"));
     #endif

   
   Serial.println(F(" "));
   }



 if (val_WIFI == 54)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Bumper OFF"));
   Bumper_Activate_Frnt = 0;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
   
   #if defined(BOARD_DUE)
     dueFlashStorage.write(90 , 1);
     dueFlashStorage.write(91 , Bumper_Activate_Frnt);
     Serial.println(F("Saved to dueFlashStorage"));
     #endif
   
   #if defined(BOARD_MEGA)
     EEPROM.write(90 , 1);
     EEPROM.write(91 , Bumper_Activate_Frnt);
     Serial.println(F("Saved to EEPROM"));
     #endif   
   
   
   Serial.println(F(" "));
   }


// Alarm 1 ON Setting Via WIFI.
 if (val_WIFI == 57)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Alarm 1 ON"));
   Alarm_1_ON = 1;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once

   #if defined(BOARD_DUE)
     dueFlashStorage.write(1 , 1);
     dueFlashStorage.write(4 , Alarm_1_ON);
     Serial.println(F("Saved to dueFlashStorage"));
     #endif

   #if defined(BOARD_MEGA)
     EEPROM.write(1 , 1);
     EEPROM.write(4 , Alarm_1_ON);
     Serial.println(F("Saved to EEPROM"));
     #endif

   
   Serial.println(F(" "));
   }


// Alarm 1 OFF Setting Via WIFI.
 if (val_WIFI == 58)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Alarm 1 OFF"));
   Alarm_1_ON = 0;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once

   #if defined(BOARD_DUE)
     dueFlashStorage.write(1 , 1);
     dueFlashStorage.write(4 , Alarm_1_ON);
     Serial.println(F("Saved to dueFlashStorage"));
     #endif

   #if defined(BOARD_MEGA)
     EEPROM.write(1 , 1);
     EEPROM.write(4 , Alarm_1_ON);
     Serial.println(F("Saved to EEPROM"));
     #endif

   
   Serial.println(F(" "));
   }




// Alarm 2 ON Setting Via WIFI.
 if (val_WIFI == 59)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Alarm 2 ON"));
   Alarm_2_ON = 1;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once

   #if defined(BOARD_DUE)
     dueFlashStorage.write(5 , 1);
     dueFlashStorage.write(8 , Alarm_2_ON);
     Serial.println(F("Saved to dueFlashStorage"));
     #endif

   #if defined(BOARD_MEGA)
     EEPROM.write(5 , 1);
     EEPROM.write(8 , Alarm_2_ON);
     Serial.println(F("Saved to EEPROM"));
     #endif

   
   Serial.println(F(" "));
   }


// Alarm 2 OFF Setting Via WIFI.
 if (val_WIFI == 60)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Alarm 2 OFF"));
   Alarm_2_ON = 0;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once

   #if defined(BOARD_DUE)
     dueFlashStorage.write(5 , 1);
     dueFlashStorage.write(8 , Alarm_2_ON);
     Serial.println(F("Saved to dueFlashStorage"));
     #endif

   #if defined(BOARD_MEGA)
     EEPROM.write(5 , 1);
     EEPROM.write(8 , Alarm_2_ON);
     Serial.println(F("Saved to EEPROM"));
     #endif

   
   Serial.println(F(" "));
   }



// Alarm 3 ON Setting Via WIFI.
 if (val_WIFI == 61)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Alarm 3 ON"));
   Alarm_3_ON = 1;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once

   #if defined(BOARD_DUE)
   dueFlashStorage.write(9 , 1);
   dueFlashStorage.write(12 , Alarm_3_ON);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(9 , 1);
   EEPROM.write(12 , Alarm_3_ON);
   Serial.println(F("Saved to EEPROM"));
   #endif

   
   Serial.println(F(" "));
   }


// Alarm 3 OFF Setting Via WIFI.
 if (val_WIFI == 62)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Alarm 3 OFF"));
   Alarm_3_ON = 0;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once

   #if defined(BOARD_DUE)
   dueFlashStorage.write(9 , 1);
   dueFlashStorage.write(12 , Alarm_3_ON);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(9 , 1);
   EEPROM.write(12 , Alarm_3_ON);
   Serial.println(F("Saved to EEPROM"));
   #endif

   
   Serial.println(F(" "));
   }



// GPS Enabled OFF Setting Via WIFI.
 if (val_WIFI == 65)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|GPS ON"));
   GPS_Enabled = 1;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once

   #if defined(BOARD_DUE)
   dueFlashStorage.write(107 , 1);
   dueFlashStorage.write(108 , GPS_Enabled);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(107 , 1);
   EEPROM.write(108 , GPS_Enabled);
   Serial.println(F("Saved to EEPROM"));
   #endif

   
   Serial.println(F(" "));
   }


// GPS Enabled OFF Setting Via WIFI.
 if (val_WIFI == 66)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|GPS OFF"));
   GPS_Enabled = 0;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once

   #if defined(BOARD_DUE)
   dueFlashStorage.write(107 , 1);
   dueFlashStorage.write(108 , GPS_Enabled);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(107 , 1);
   EEPROM.write(108 , GPS_Enabled);
   Serial.println(F("Saved to EEPROM"));
   #endif

   
   Serial.println(F(" "));
   }



// Angle Sensor
 if (val_WIFI == 67)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Angle Sensor ON"));
   Angle_Sensor_Enabled = 1;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once

   #if defined(BOARD_DUE)
   dueFlashStorage.write(29 , 1);
   dueFlashStorage.write(30 , Angle_Sensor_Enabled);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(29 , 1);
   EEPROM.write(30 , Angle_Sensor_Enabled);
   Serial.println(F("Saved to EEPROM"));
   #endif

   
   Serial.println(F(" "));
   }


// Angle Sensor
 if (val_WIFI == 68)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Angle Sensor OFF"));
   Angle_Sensor_Enabled = 0;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once

   #if defined(BOARD_DUE)
   dueFlashStorage.write(29 , 1);
   dueFlashStorage.write(30 , Angle_Sensor_Enabled);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(29 , 1);
   EEPROM.write(30 , Angle_Sensor_Enabled);
   Serial.println(F("Saved to EEPROM"));
   #endif

   
   Serial.println(F(" "));
   }



// Tip Sensor
 if (val_WIFI == 69)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Tip Sensor ON"));
   Tip_Over_Sensor_Enabled = 1;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once

   #if defined(BOARD_DUE)
   dueFlashStorage.write(92 , 1);
   dueFlashStorage.write(93 , Tip_Over_Sensor_Enabled);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(92 , 1);
   EEPROM.write(93 , Tip_Over_Sensor_Enabled);
   Serial.println(F("Saved to EEPROM"));
   #endif

   
   Serial.println(F(" "));
   }


// Tip Sensor
 if (val_WIFI == 70)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Tip Sensor OFF"));
   Tip_Over_Sensor_Enabled = 0;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once

   #if defined(BOARD_DUE)
   dueFlashStorage.write(92 , 1);
   dueFlashStorage.write(93 , Tip_Over_Sensor_Enabled);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(92 , 1);
   EEPROM.write(93 , Tip_Over_Sensor_Enabled);
   Serial.println(F("Saved to EEPROM"));
   #endif

   
   Serial.println(F(" "));
   }



// Wheel Amp Sensor
 if (val_WIFI == 71)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Wheel Amp Sensor ON"));
   Wheel_Amp_Sensor_ON = 1;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once

   #if defined(BOARD_DUE)
   dueFlashStorage.write(115 , 1);
   dueFlashStorage.write(116 , Wheel_Amp_Sensor_ON);
   Serial.println(F("Saved to dueFlashStorage"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(115 , 1);
   EEPROM.write(116 , Wheel_Amp_Sensor_ON);
   Serial.println(F("Saved to EEPROM"));
   #endif

   
   Serial.println(F(" "));
   }


// Wheel Amp Sensor
 if (val_WIFI == 72)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Wheel Amp Sensor OFF"));
   Wheel_Amp_Sensor_ON = 0;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once

   #if defined(BOARD_DUE)   
   dueFlashStorage.write(115 , 1);
   dueFlashStorage.write(116 , Wheel_Amp_Sensor_ON);
   Serial.println(F("Saved to DueFlash"));
   #endif

   #if defined(BOARD_MEGA)
   EEPROM.write(115 , 1);
   EEPROM.write(116 , Wheel_Amp_Sensor_ON);
   Serial.println(F("Saved to EEPROM"));
   #endif

   
   Serial.println(F(" "));
   }

// UPDATE THE WIFI APP WITH THE BUTTON STATUS
// This is a 2 step process.  First the MEGA receives a code from the App/NodeMCU board
// That it should stop the loop and get ready to transmit the data
// On the secnd command the data is sent.
// This ensures the MEGA is synced to the NodeMCU
 
 if (val_WIFI == 46)  {    
   Serial.println(F(""));
   Serial.println(F("Value "));
   Serial.print(val_WIFI);
   Serial.println(F(" received"));
   
   // only go into the wait mode if the mower is parked or docked.
   if ( (Mower_Docked == 1) || (Mower_Parked == 1)) {   
    Serial.println(F("Waiting..."));
    Menu_NodeMCU_Complete = false;
    
    while (Menu_NodeMCU_Complete == false) {                      
         while(Serial2.available()>0){
         val_WIFI = Serial2.parseInt();
         if(Serial2.read()== '\p'){
         delay(200);         
          // Receive_Data_From_NODEMCU();                  // Look for the transmission signal
          if (val_WIFI == 55)  {                        // If the signal arrived send the data
            Serial.println(F(""));
            Serial.println(F("Updating WIFI Buttons - Sync 1"));
            Serial.println(F(""));
            Transmit_APP_Buttons_Status_1();              // Send the button status data
            Menu_NodeMCU_Complete = true;               // break the while loop
            }
         }
      }
               
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
   }
  }
}


// Sync 2 Settings
 if (val_WIFI == 63)  {    
   Serial.println(F(""));
   Serial.println(F("Value "));
   Serial.print(val_WIFI);
   Serial.println(F(" received"));
   
   // only go into the wait mode if the mower is parked or docked.
   if ( (Mower_Docked == 1) || (Mower_Parked == 1)) {   
    Serial.println(F("Waiting..."));
    Menu_NodeMCU_Complete = false;
    
    while (Menu_NodeMCU_Complete == false) {                      
         while(Serial2.available()>0){
         val_WIFI = Serial2.parseInt();
         if(Serial2.read()== '\p'){
         delay(200);         
          // Receive_Data_From_NODEMCU();                  // Look for the transmission signal
          if (val_WIFI == 64)  {                        // If the signal arrived send the data
            Serial.println(F(""));
            Serial.println(F("Updating WIFI Buttons - Sync 2"));
            Serial.println(F(""));
            Transmit_APP_Buttons_Status_2();              // Send the button status data
            Menu_NodeMCU_Complete = true;               // break the while loop
            }
         }
      }
               
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
   }
  }
}



}


void Execute_Manuel_Blynk_Command_To_Mower() {
// insert wheel motions here.

 if (val_WIFI == 16)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Automatic Mode"));
   Manouver_Park_The_Mower(); 
   Turn_On_Relay(); 
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }

 if (val_WIFI == 17)  {    
   Serial.print(F("WIFI")); 
   Serial.print(val_WIFI);
   Serial.print(F("|Wheel Forward"));
   SetPins_ToGoForwards(); 
   bool Temp_Ramp_Motor_ON = Ramp_Motor_ON;                                              // Save Ramp_Motor status
   Ramp_Motor_ON = 0;                                                                    // Turn off ramp motor option to get fast response
   Motor_Action_Go_Full_Speed();
   if (Temp_Ramp_Motor_ON == 1) (Ramp_Motor_ON = 1);                                     // Turn Ramp Motor ON or OFF again
   if (Temp_Ramp_Motor_ON == 0) (Ramp_Motor_ON = 0);
   delay(300);
   Motor_Action_Stop_Motors();
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }

 if (val_WIFI == 18)  {    
   Serial.print(F("WIFI:")); 
   Serial.print(val_WIFI);
   Serial.print(F("|Wheel Reverse"));
   SetPins_ToGoBackwards(); 
   bool Temp_Ramp_Motor_ON = Ramp_Motor_ON;                                              // Save Ramp_Motor status
   Ramp_Motor_ON = 0;                                                                    // Turn off ramp motor option to get fast response
   Motor_Action_Go_Full_Speed();
   if (Temp_Ramp_Motor_ON == 1) (Ramp_Motor_ON = 1);                                     // Turn Ramp Motor ON or OFF again
   if (Temp_Ramp_Motor_ON == 0) (Ramp_Motor_ON = 0);
   delay(300);
   Motor_Action_Stop_Motors();
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }

 if (val_WIFI == 19)  {    
   Serial.print(F("WIFI:")); 
   Serial.print(val_WIFI);
   Serial.print(F("|Wheel Left"));
   SetPins_ToTurnLeft();
   bool Temp_Ramp_Motor_ON = Ramp_Motor_ON;                                              // Save Ramp_Motor status
   Ramp_Motor_ON = 0;                                                                    // Turn off ramp motor option to get fast response
   Motor_Action_Go_Full_Speed(); 
   if (Temp_Ramp_Motor_ON == 1) (Ramp_Motor_ON = 1);                                     // Turn Ramp Motor ON or OFF again
   if (Temp_Ramp_Motor_ON == 0) (Ramp_Motor_ON = 0);
   if (Robot_Type == 1) delay(200);
   if (Robot_Type == 2) delay(800);
   
   Motor_Action_Stop_Motors();
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }

 if (val_WIFI == 20)  {    
   Serial.print(F("WIFI:")); 
   Serial.print(val_WIFI);
   Serial.print(F("|Wheel Right"));
   SetPins_ToTurnRight();
   bool Temp_Ramp_Motor_ON = Ramp_Motor_ON;                                              // Save Ramp_Motor status
   Ramp_Motor_ON = 0;                                                                    // Turn off ramp motor option to get fast response
   Motor_Action_Go_Full_Speed();
   if (Temp_Ramp_Motor_ON == 1) (Ramp_Motor_ON = 1);                                     // Turn Ramp Motor ON or OFF again
   if (Temp_Ramp_Motor_ON == 0) (Ramp_Motor_ON = 0);
   if (Robot_Type == 1) delay(200);
   if (Robot_Type == 2) delay(800);
   Motor_Action_Stop_Motors();
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }


if (val_WIFI == 80) {
   Serial.print(F("WIFI:")); 
   Serial.print(val_WIFI);
   Serial.print(F("|MECANUM Left Shift"));
   Mecanum_Side_Movement_Left();
   bool Temp_Ramp_Motor_ON = Ramp_Motor_ON;                                              // Save Ramp_Motor status
   Ramp_Motor_ON = 0;                                                                    // Turn off ramp motor option to get fast response
   Motor_Action_Go_Full_Speed();
   if (Temp_Ramp_Motor_ON == 1) (Ramp_Motor_ON = 1);                                     // Turn Ramp Motor ON or OFF again
   if (Temp_Ramp_Motor_ON == 0) (Ramp_Motor_ON = 0);
   if (Robot_Type == 1) delay(200);
   if (Robot_Type == 2) delay(800);
   Motor_Action_Stop_Motors();
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once   
}

if (val_WIFI == 81) {
   Serial.print(F("WIFI:")); 
   Serial.print(val_WIFI);
   Serial.print(F("|MECANUM Right Shift"));
   Mecanum_Side_Movement_Right();
   bool Temp_Ramp_Motor_ON = Ramp_Motor_ON;                                              // Save Ramp_Motor status
   Ramp_Motor_ON = 0;                                                                    // Turn off ramp motor option to get fast response
   Motor_Action_Go_Full_Speed();
   if (Temp_Ramp_Motor_ON == 1) (Ramp_Motor_ON = 1);                                     // Turn Ramp Motor ON or OFF again
   if (Temp_Ramp_Motor_ON == 0) (Ramp_Motor_ON = 0);
   if (Robot_Type == 1) delay(200);
   if (Robot_Type == 2) delay(800);
   Motor_Action_Stop_Motors();
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once   
}


if (val_WIFI == 82) {
   Serial.print(F("WIFI:")); 
   Serial.print(val_WIFI);
   Serial.print(F("|MECANUM Left Diagonal"));
   Mecanum_Diagonal_Movement_Left();
   bool Temp_Ramp_Motor_ON = Ramp_Motor_ON;                                              // Save Ramp_Motor status
   Ramp_Motor_ON = 0;                                                                    // Turn off ramp motor option to get fast response
   Motor_Action_Go_Full_Speed();
   if (Temp_Ramp_Motor_ON == 1) (Ramp_Motor_ON = 1);                                     // Turn Ramp Motor ON or OFF again
   if (Temp_Ramp_Motor_ON == 0) (Ramp_Motor_ON = 0);
   if (Robot_Type == 1) delay(200);
   if (Robot_Type == 2) delay(800);
   Motor_Action_Stop_Motors();
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once   
}

if (val_WIFI == 83) {
   Serial.print(F("WIFI:")); 
   Serial.print(val_WIFI);
   Serial.print(F("|MECANUM Right Diagonal"));
   Mecanum_Diagonal_Movement_Right();
   bool Temp_Ramp_Motor_ON = Ramp_Motor_ON;                                              // Save Ramp_Motor status
   Ramp_Motor_ON = 0;                                                                    // Turn off ramp motor option to get fast response
   Motor_Action_Go_Full_Speed();
   if (Temp_Ramp_Motor_ON == 1) (Ramp_Motor_ON = 1);                                     // Turn Ramp Motor ON or OFF again
   if (Temp_Ramp_Motor_ON == 0) (Ramp_Motor_ON = 0);
   if (Robot_Type == 1) delay(200);
   if (Robot_Type == 2) delay(800);
   Motor_Action_Stop_Motors();
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once   
}


 if (val_WIFI == 56)  {    
   Serial.println(F(""));
   Serial.print(F("WIFI:")); 
   Serial.print(val_WIFI);
   Serial.println(F("Dock in this direction"));
   Serial.println(F(""));

   bool Keep_Compass_Value = 0;
   if (Compass_Activate == 1) {
    Compass_Activate = 0;
    Keep_Compass_Value = 1;
    }

   // Compass is deactivated before to ensure the mower docks in this direction.
   Manouver_Go_To_Charging_Station();
   
   if (Keep_Compass_Value == 1) {
    Compass_Activate = 1;
    }


  }

// Test Drill Cycle
 if (val_WIFI == 75) {
   Serial.println(F(""));
   Serial.print(F("WIFI:")); 
   Serial.print(val_WIFI);    
   Serial.println(F(""));
   Serial.println(F("WIFI Drill Cycle"));
   Initiate_Drill_Cycle();
   }  

}


void Update_Blynk_Start_Conditions() {

  Serial2.print(Compass_Activate);
  Serial2.println("\a");
  delay(5);
  Serial2.print(Loop_Cycle_Mowing);
  Serial2.println("\b");
  delay(5);
  Serial2.print(Mower_Docked);
  Serial2.println("\c");
  delay(5);
  Serial2.print(Mower_Running);
  Serial2.println("\d");
  delay(5);
  Serial2.print(Mower_Parked);
  Serial2.println("\e");
  delay(5);
  Serial2.println(Charging);
  Serial2.println("\f");
  delay(5);
  Serial2.println(Tracking_Wire);
  Serial2.println("\g"); 
  } 
