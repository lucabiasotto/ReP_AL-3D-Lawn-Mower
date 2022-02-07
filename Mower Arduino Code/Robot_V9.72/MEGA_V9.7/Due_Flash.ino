// Experimental - This is ready for a potential future swith to a 
// 32bit Arduino due which will improve the loop speed

// Loads the saved values from dueFlashStorage
// to the settings menuu

void Load_dueFlashStorage_Saved_Data() {

#if defined(BOARD_DUE)

Serial.println("Loading dueFlashStorage Settings");

  int Robot_Type_dueFlashStorage = dueFlashStorage.read(121);
  if (Robot_Type_dueFlashStorage == 1) {
    Robot_Type = dueFlashStorage.read(122); 
    Serial.print(F("FLASH: Robot_Type: "));
    if (Robot_Type == 1) Serial.println(F("FLASH: Robot Type = Mower"));
    if (Robot_Type == 2) Serial.println(F("FLASH: Robot Type = Aerator"));
  }

  if (Robot_Type_dueFlashStorage != 1) {
    if (Robot_Type == 1) Serial.println(F("Settings: Robot Type = Mower"));
    if (Robot_Type == 2) Serial.println(F("Settings: Robot Type = Aerator"));
  }


  int PCB_dueFlashStorage = dueFlashStorage.read(119);
  if (PCB_dueFlashStorage == 1) {
    PCB = dueFlashStorage.read(120); 
    Serial.print(F("FLASH: PCB Enabled: "));
      if (PCB == 1) Serial.println(F("Enabled"));
      if (PCB == 0) Serial.println(F("Disabled"));
      }

  if (PCB_dueFlashStorage != 1) {
    Serial.print(F("Settings: PCB Enabled: "));
      if (PCB == 1) Serial.println(F("Enabled"));
      if (PCB == 0) Serial.println(F("Disabled"));
      }

 
  int Alarm_1_Saved_dueFlashStorage = dueFlashStorage.read(1);
  
  if (Alarm_1_Saved_dueFlashStorage == 1) {
    Alarm_1_Hour    = dueFlashStorage.read(2);
    Alarm_1_Minute  = dueFlashStorage.read(3);
    Alarm_1_ON      = dueFlashStorage.read(4);
    Alarm_1_Action  = dueFlashStorage.read(87);

    if (Alarm_1_Hour == 255)   {
      Alarm_1_Hour = 0;
      Alarm_1_ON = 0;
      }
    if (Alarm_1_Minute == 255) {
      Alarm_1_Minute = 0;
      Alarm_1_ON  = 0;
      }
    
    if (Alarm_1_ON == 0)  Serial.println("Alarm 1 : OFF");
 
    if (Alarm_1_ON == 1) {
      Serial.print(F("dueFlashStorage Alarm 1 Active | Time Set:"));
      Serial.print(Alarm_1_Hour);
      Serial.print(F(":"));
      if (Alarm_1_Minute < 10) Serial.print("0");
      Serial.print(Alarm_1_Minute);
      Serial.print(" | Action :");
      if (Alarm_1_Action == 1) Serial.println("Exit Zone 1");
      if (Alarm_1_Action == 2) Serial.println("Exit Zone 2");
      if (Alarm_1_Action == 3) Serial.println("Mow the Line");
      if (Alarm_1_Action == 4) Serial.println("Quick Start");
      if (Alarm_1_Action == 5) Serial.println("Custom");
      }
    }
  if (Alarm_1_Saved_dueFlashStorage != 1) {
    if (Alarm_1_ON == 1) {
      Serial.print(F("Settings: Alarm 1 Active | Time Set:"));
      Serial.print(Alarm_1_Hour);
      Serial.print(F(":"));
      if (Alarm_1_Minute < 10) Serial.print("0");
      Serial.print(Alarm_1_Minute);
      Serial.print(" | Action :");
      if (Alarm_1_Action == 1) Serial.println("Exit Zone 1");
      if (Alarm_1_Action == 2) Serial.println("Exit Zone 2");
      if (Alarm_1_Action == 3) Serial.println("Mow the Line");
      if (Alarm_1_Action == 4) Serial.println("Quick Start");
      if (Alarm_1_Action == 5) Serial.println("Custom");
      }
  }
    
  int Alarm_2_Saved_dueFlashStorage = dueFlashStorage.read(5);
  
  if (Alarm_2_Saved_dueFlashStorage == 1) {
    Alarm_2_Hour    = dueFlashStorage.read(6);
    Alarm_2_Minute  = dueFlashStorage.read(7);
    Alarm_2_ON      = dueFlashStorage.read(8);
    Alarm_2_Action  = dueFlashStorage.read(88);
    
    if (Alarm_2_Hour == 255)   {
      Alarm_2_Hour = 0;
      Alarm_2_ON = 0;
      }
    if (Alarm_2_Minute == 255) {
      Alarm_2_Minute = 0;
      Alarm_2_ON  = 0;
      }
    
    if (Alarm_2_ON == 0) Serial.println("Alarm 2 : OFF:");
    
    if (Alarm_2_ON == 1) {
      Serial.print(F("Alarm 2 Active | Time Set:"));
      Serial.print(Alarm_2_Hour);
      Serial.print(F(":"));
      if (Alarm_2_Minute < 10) Serial.print("0");
      Serial.print(Alarm_2_Minute);  
      Serial.print(" | Action :");
      if (Alarm_2_Action == 1) Serial.println("Exit Zone 1");
      if (Alarm_2_Action == 2) Serial.println("Exit Zone 2");
      if (Alarm_2_Action == 3) Serial.println("Mow the Line");
      if (Alarm_2_Action == 4) Serial.println("Quick Start");
      if (Alarm_2_Action == 5) Serial.println("Custom");
      }

    }

  if (Alarm_2_Saved_dueFlashStorage != 1) {
    if (Alarm_2_ON == 1) {
      Serial.print(F("Settings: Alarm 2 Active | Time Set:"));
      Serial.print(Alarm_2_Hour);
      Serial.print(F(":"));
      if (Alarm_2_Minute < 10) Serial.print("0");
      Serial.print(Alarm_2_Minute);
      Serial.print(" | Action :");
      if (Alarm_2_Action == 1) Serial.println("Exit Zone 1");
      if (Alarm_2_Action == 2) Serial.println("Exit Zone 2");
      if (Alarm_2_Action == 3) Serial.println("Mow the Line");
      if (Alarm_2_Action == 4) Serial.println("Quick Start");
      if (Alarm_2_Action == 5) Serial.println("Custom");
      }
  }


  int Alarm_3_Saved_dueFlashStorage = dueFlashStorage.read(9);
  
  if (Alarm_3_Saved_dueFlashStorage == 1) {
    Alarm_3_Hour    = dueFlashStorage.read(10);
    Alarm_3_Minute  = dueFlashStorage.read(11);
    Alarm_3_ON      = dueFlashStorage.read(12);
    Alarm_3_Action  = dueFlashStorage.read(89);

    if (Alarm_3_Hour == 255)   {
      Alarm_3_Hour = 0;
      Alarm_3_ON = 0;
      }
    if (Alarm_3_Minute == 255) {
      Alarm_3_Minute = 0;
      Alarm_3_ON  = 0;
      }
    
    
    if (Alarm_3_ON == 0) Serial.println("Alarm 3 : OFF: ");
   
    if (Alarm_3_ON == 1) {
      Serial.print(F("Alarm 3 Active | Time Set:"));
      Serial.print(Alarm_3_Hour);
      Serial.print(F(":"));
      if (Alarm_3_Minute < 10) Serial.print("0");
      Serial.print(Alarm_3_Minute);
      Serial.print(" | Action :");
      if (Alarm_3_Action == 1) Serial.println("Exit Zone 1");
      if (Alarm_3_Action == 2) Serial.println("Exit Zone 2");
      if (Alarm_3_Action == 3) Serial.println("Mow the Line");
      if (Alarm_3_Action == 4) Serial.println("Quick Start");
      if (Alarm_3_Action == 5) Serial.println("Custom");  
      }

    }

  if (Alarm_3_Saved_dueFlashStorage != 1) {
    if (Alarm_3_ON == 1) {
      Serial.print(F("Settings: Alarm 3 Active | Time Set:"));
      Serial.print(Alarm_3_Hour);
      Serial.print(F(":"));
      if (Alarm_3_Minute < 10) Serial.print("0");
      Serial.print(Alarm_3_Minute);
      Serial.print(" | Action :");
      if (Alarm_3_Action == 1) Serial.println("Exit Zone 1");
      if (Alarm_3_Action == 2) Serial.println("Exit Zone 2");
      if (Alarm_3_Action == 3) Serial.println("Mow the Line");
      if (Alarm_3_Action == 4) Serial.println("Quick Start");
      if (Alarm_3_Action == 5) Serial.println("Custom");
      }
  }
    

  int PWM_LEFT_dueFlashStorage = dueFlashStorage.read(13);
  if (PWM_LEFT_dueFlashStorage == 1) {
    PWM_MaxSpeed_LH = dueFlashStorage.read(14);
    Serial.print(F("FLASH: PWM Wheel Left value : "));
    Serial.println(PWM_MaxSpeed_LH);
  }

  if (PWM_LEFT_dueFlashStorage != 1) {
    Serial.print(F("Settings: PWM Wheel Left value: "));
    Serial.println(PWM_MaxSpeed_LH);
    }

  int PWM_RIGHT_dueFlashStorage = dueFlashStorage.read(15);
  if (PWM_RIGHT_dueFlashStorage == 1) {
    PWM_MaxSpeed_RH = dueFlashStorage.read(16); 
    Serial.print(F("FLASH: PWM Wheel Right value: ")); 
    Serial.println(PWM_MaxSpeed_RH);
  }

  if (PWM_RIGHT_dueFlashStorage != 1) {
    Serial.print(F("Settings: PWM Wheel Right value: ")); 
    Serial.println(PWM_MaxSpeed_RH);
  }

  int PWM_LEFT_Slow_dueFlashStorage = dueFlashStorage.read(94);
  if (PWM_LEFT_Slow_dueFlashStorage == 1) {
    PWM_Slow_Speed_LH = dueFlashStorage.read(95);
    Serial.print(F("FLASH: PWM Wheel Slow Left value: "));
    Serial.println(PWM_Slow_Speed_LH);
  }

  if (PWM_LEFT_Slow_dueFlashStorage != 1) {
    Serial.print(F("Settings: PWM Wheel Slow Left value: "));
    Serial.println(PWM_Slow_Speed_LH);
  }

  int PWM_RIGHT_Slow_dueFlashStorage = dueFlashStorage.read(96);
  if (PWM_RIGHT_Slow_dueFlashStorage == 1) {
    PWM_Slow_Speed_RH = dueFlashStorage.read(97); 
    Serial.print(F("FLASH: PWM Wheel Slow Right value: ")); 
    Serial.println(PWM_Slow_Speed_RH);
  }

  if (PWM_RIGHT_Slow_dueFlashStorage != 1) {
    PWM_Slow_Speed_RH = dueFlashStorage.read(97); 
    Serial.print(F("Settings: PWM Wheel Slow Right value: ")); 
    Serial.println(PWM_Slow_Speed_RH);
  }
  
  int Slow_Speed_MAG_dueFlashStorage = dueFlashStorage.read(98);
  if (Slow_Speed_MAG_dueFlashStorage == 1) {
    Slow_Speed_MAG = dueFlashStorage.read(99); 
    Slow_Speed_MAG = (Slow_Speed_MAG * -1 ) * 10;
    Serial.print(F("FLASH: Slow Speed MAG: ")); 
    Serial.println(Slow_Speed_MAG);
  }

 if (Slow_Speed_MAG_dueFlashStorage == 1) {
    Serial.print(F("Settings: Slow Speed MAG: ")); 
    Serial.println(Slow_Speed_MAG);
  }


  int PWM_BLADE_dueFlashStorage = dueFlashStorage.read(17);
  if (PWM_BLADE_dueFlashStorage == 1) {
    PWM_Blade_Speed = dueFlashStorage.read(18); 
    Serial.print(F("FLASH: PWM Blade value: ")); 
    Serial.println(PWM_Blade_Speed);
  }

  if (PWM_BLADE_dueFlashStorage != 1) {
    Serial.print(F("Settings: PWM Blade value: ")); 
    Serial.println(PWM_Blade_Speed);
  }


  int COMPASS_dueFlashStorage = dueFlashStorage.read(19);
  if (COMPASS_dueFlashStorage == 1) {
    Compass_Activate = dueFlashStorage.read(20);  
    Serial.print(F("FLASH: Compass Activated: "));
    if (Compass_Activate == 0) Serial.println(F("OFF"));
    if (Compass_Activate == 1) Serial.println(F("ON"));
  }

  if (COMPASS_dueFlashStorage != 1) {
    Serial.print(F("Settings: Compass Activated: "));
    if (Compass_Activate == 0) Serial.println(F("OFF"));
    if (Compass_Activate == 1) Serial.println(F("ON"));
  }

  int COMPASS_Setup_Mode_dueFlashStorage = dueFlashStorage.read(113);
  if (COMPASS_Setup_Mode_dueFlashStorage == 1) {
    Compass_Setup_Mode = dueFlashStorage.read(114);  
    Serial.print(F("FLASH: Compass Setup Mode: "));
   if (Compass_Setup_Mode == 1) Serial.println(F("Setup DFRobot_QMC5883 Compass"));
   if (Compass_Setup_Mode == 2) Serial.println(F("Setup QMC5883_Manual Compass"));           
   if (Compass_Setup_Mode == 3) Serial.println(F("Setup QMC5883L Compass"));      
   }

  if (COMPASS_Setup_Mode_dueFlashStorage != 1) {
    Serial.print(F("Settings: Compass Setup Mode: "));
   if (Compass_Setup_Mode == 1) Serial.println(F("Setup DFRobot_QMC5883 Compass"));
   if (Compass_Setup_Mode == 2) Serial.println(F("Setup QMC5883_Manual Compass"));           
   if (Compass_Setup_Mode == 3) Serial.println(F("Setup QMC5883L Compass"));      
   }

  int Compass_Heading_Hold_Enabled_dueFlashStorage = dueFlashStorage.read(59);
  if (Compass_Heading_Hold_Enabled_dueFlashStorage == 1) {
    Compass_Heading_Hold_Enabled = dueFlashStorage.read(60);  
    Serial.print(F("FLASH: Compass Heading Hold: "));
    if (Compass_Heading_Hold_Enabled == 0) Serial.println(F("OFF"));
    if (Compass_Heading_Hold_Enabled == 1) Serial.println(F("ON"));
  }

  if (Compass_Heading_Hold_Enabled_dueFlashStorage != 1) {
    Serial.print(F("Settings: Compass Heading Hold: "));
    if (Compass_Heading_Hold_Enabled == 0) Serial.println(F("OFF"));
    if (Compass_Heading_Hold_Enabled == 1) Serial.println(F("ON"));
  }

  int CPower_dueFlashStorage = dueFlashStorage.read(61);
  if (CPower_dueFlashStorage == 1) {
    CPower = dueFlashStorage.read(62); 
    CPower = CPower / 10; 
    Serial.print(F("FLASH: Compass Power: "));
    Serial.println(CPower);
  }

  if (CPower_dueFlashStorage != 1) {
    CPower = CPower / 10; 
    Serial.print(F("Settings: Compass Power: "));
    Serial.println(CPower);
  }

  int Compass_Home_dueFlashStorage = dueFlashStorage.read(27);
  if (Compass_Home_dueFlashStorage == 1) {
    Home_Wire_Compass_Heading = (dueFlashStorage.read(28) * 10);    // *10 as value can be more than 255. Vaule is therefore stored as a tenth value
    Serial.print(F("FLASH: Compass Home Degrees : ")); 
    Serial.println(Home_Wire_Compass_Heading);
  }

  if (Compass_Home_dueFlashStorage != 1) {
    Serial.print(F("Settings: Compass Home Degrees : ")); 
    Serial.println(Home_Wire_Compass_Heading);
  }

 int Tracking_PID_P_dueFlashStorage = dueFlashStorage.read(21);
  if (Tracking_PID_P_dueFlashStorage == 1) {
    P = dueFlashStorage.read(22); 
    P = P / 100; 
    Serial.print(F("FLASH: Traking PID P: "));
    Serial.println(P);
  }


  if (Tracking_PID_P_dueFlashStorage != 1) {
    Serial.print(F("Settings: Traking PID P: "));
    Serial.println(P);
  }

  int Pattern_Mow_dueFlashStorage = dueFlashStorage.read(23);
  if (Pattern_Mow_dueFlashStorage == 1) {
    Pattern_Mow = dueFlashStorage.read(24);  
    //Pattern_Mow = 1;
    Serial.print(F("FLASH: Pattern Mow Type: "));
    if (Pattern_Mow == 0) Serial.println(F("OFF"));
    if (Pattern_Mow == 1) Serial.println(F("ON Parallel"));
    if (Pattern_Mow == 2) Serial.println(F("ON Spiral"));
  }

  if (Pattern_Mow_dueFlashStorage != 1) {
    Serial.print(F("Settings: Pattern Mow Type: "));
    if (Pattern_Mow == 0) Serial.println(F("OFF"));
    if (Pattern_Mow == 1) Serial.println(F("ON Parallel"));
    if (Pattern_Mow == 2) Serial.println(F("ON Spiral"));
  }


  int Minimum_Volt_dueFlashStorage = dueFlashStorage.read(25);
  if (Minimum_Volt_dueFlashStorage == 1) {
    Battery_Min = dueFlashStorage.read(26); 
    Battery_Min = Battery_Min / 10; 
    Serial.print(F("FLASH: Minimum Battery Voltage: "));
    Serial.println(Battery_Min);
  }

  if (Minimum_Volt_dueFlashStorage != 1) {
    Serial.print(F("Settings: Minimum Battery Voltage: "));
    Serial.println(Battery_Min);
  }


  int Angle_Sensor_Enabled_dueFlashStorage = dueFlashStorage.read(29);
  if (Angle_Sensor_Enabled_dueFlashStorage == 1) {
    Angle_Sensor_Enabled = dueFlashStorage.read(30);  
    Serial.print(F("FLASH: Angle Sensor Enabled: "));
    if (Angle_Sensor_Enabled == 0) Serial.println(F("OFF"));
    if (Angle_Sensor_Enabled == 1) Serial.println(F("ON"));
  }

  if (Angle_Sensor_Enabled_dueFlashStorage != 1) {
    Serial.print(F("Settings: Angle Sensor Enabled: "));
    if (Angle_Sensor_Enabled == 0) Serial.println(F("OFF"));
    if (Angle_Sensor_Enabled == 1) Serial.println(F("ON"));
  }


  int Tip_Over_Sensor_Enabled_dueFlashStorage = dueFlashStorage.read(92);
  if (Tip_Over_Sensor_Enabled_dueFlashStorage == 1) {
    Tip_Over_Sensor_Enabled = dueFlashStorage.read(93);  
    Serial.print(F("dueFlashStorage Tip Sensor Enabled:  "));
    if (Tip_Over_Sensor_Enabled == 0) Serial.println(F("OFF"));
    if (Tip_Over_Sensor_Enabled == 1) Serial.println(F("ON"));
  }

  if (Tip_Over_Sensor_Enabled_dueFlashStorage != 1) {
    Serial.print(F("Settings: Tip Sensor Enabled:  "));
    if (Tip_Over_Sensor_Enabled == 0) Serial.println(F("OFF"));
    if (Tip_Over_Sensor_Enabled == 1) Serial.println(F("ON"));
  }

  
  int Mower_Turn_Delay_Min_dueFlashStorage = dueFlashStorage.read(31);
  if (Mower_Turn_Delay_Min_dueFlashStorage == 1) {
    Mower_Turn_Delay_Min = dueFlashStorage.read(32);
    Mower_Turn_Delay_Min = Mower_Turn_Delay_Min * 100;
    Serial.print(F("FLASH: Mower Turn Delay Min: "));
    Serial.println(Mower_Turn_Delay_Min);
  }

  if (Mower_Turn_Delay_Min_dueFlashStorage != 1) {
    Serial.print(F("Settings: Mower Turn Delay Min: "));
    Serial.println(Mower_Turn_Delay_Min);
  }


  int Mower_Turn_Delay_Max_dueFlashStorage = dueFlashStorage.read(33);
  if (Mower_Turn_Delay_Max_dueFlashStorage == 1) {
    Mower_Turn_Delay_Max = dueFlashStorage.read(34);
    Mower_Turn_Delay_Max =  Mower_Turn_Delay_Max * 100; 
    Serial.print(F("FLASH: Mower Turn Delay Max: "));
    Serial.println(Mower_Turn_Delay_Max);
  }


  if (Mower_Turn_Delay_Max_dueFlashStorage != 1) {
    Serial.print(F("Settings: Mower Turn Delay Max: "));
    Serial.println(Mower_Turn_Delay_Max);
  }


  int Mower_Reverse_Delay_dueFlashStorage = dueFlashStorage.read(35);
  if (Mower_Reverse_Delay_dueFlashStorage == 1) {
    Mower_Reverse_Delay = dueFlashStorage.read(36);
    Mower_Reverse_Delay = (Mower_Reverse_Delay * 100);
    Serial.print(F("FLASH: Mower Reverse Time Delay/ms: "));
    Serial.println(Mower_Reverse_Delay);
  }

  if (Mower_Reverse_Delay_dueFlashStorage != 1) {
    Serial.print(F("Settings: Mower Reverse Time Delay/ms: "));
    Serial.println(Mower_Reverse_Delay);
  }



  int Track_Wire_Zone_1_Cycles_dueFlashStorage = dueFlashStorage.read(43);
  if (Track_Wire_Zone_1_Cycles_dueFlashStorage == 1) {
    Track_Wire_Zone_1_Cycles = dueFlashStorage.read(44);
    Track_Wire_Zone_1_Cycles = (Track_Wire_Zone_1_Cycles * 100);
    Serial.print(F("FLASH: Zone 1 Tracking Cycles "));
    Serial.println(Track_Wire_Zone_1_Cycles);
  }

  if (Track_Wire_Zone_1_Cycles_dueFlashStorage != 1) {
    Serial.print(F("Settings: Zone 1 Tracking Cycles "));
    Serial.println(Track_Wire_Zone_1_Cycles);
  }

  int Track_Wire_Zone_2_Cycles_dueFlashStorage = dueFlashStorage.read(45);
  if (Track_Wire_Zone_2_Cycles_dueFlashStorage == 1) {
    Track_Wire_Zone_2_Cycles = dueFlashStorage.read(46);
    Track_Wire_Zone_2_Cycles = (Track_Wire_Zone_2_Cycles * 100);
    Serial.print(F("FLASH: Zone 2 Tracking Cycles "));
    Serial.println(Track_Wire_Zone_2_Cycles);
  }

  if (Track_Wire_Zone_2_Cycles_dueFlashStorage != 1) {
    Serial.print(F("Settings: Zone 2 Tracking Cycles "));
    Serial.println(Track_Wire_Zone_2_Cycles);
  }


  int Use_Charging_Station_dueFlashStorage = dueFlashStorage.read(47);
  if (Use_Charging_Station_dueFlashStorage == 1) {
    Use_Charging_Station = dueFlashStorage.read(48);  
    Serial.print(F("FLASH: Charge Station: "));
    if (Use_Charging_Station == 0) Serial.println(F("OFF"));
    if (Use_Charging_Station == 1) Serial.println(F("ON"));
  }

  if (Use_Charging_Station_dueFlashStorage != 1) {
    Serial.print(F("Settings: Charge Station: "));
    if (Use_Charging_Station == 0) Serial.println(F("OFF"));
    if (Use_Charging_Station == 1) Serial.println(F("ON"));
  }


  int CW_Tracking_To_Charge_dueFlashStorage = dueFlashStorage.read(49);
  if (CW_Tracking_To_Charge_dueFlashStorage == 1) {
    CW_Tracking_To_Charge = dueFlashStorage.read(50);  
    Serial.print(F("FLASH: Tracking Direction to Charge : "));
    if (CW_Tracking_To_Charge == 1) Serial.println(F("CW"));
    if (CW_Tracking_To_Charge == 0) Serial.println(F("OFF"));
  }


  if (CW_Tracking_To_Charge_dueFlashStorage != 1) {
    Serial.print(F("Settings: Tracking Direction to Charge : "));
    if (CW_Tracking_To_Charge == 1) Serial.println(F("CW"));
    if (CW_Tracking_To_Charge == 0) Serial.println(F("OFF"));
  }

  int CCW_Tracking_To_Charge_dueFlashStorage = dueFlashStorage.read(51);
  if (CCW_Tracking_To_Charge_dueFlashStorage == 1) {
    CCW_Tracking_To_Charge = dueFlashStorage.read(52);  
    Serial.print(F("dueFlashStorage Tracking Direction to Charge : "));
    if (CCW_Tracking_To_Charge == 1) Serial.println(F("CCW"));
    if (CCW_Tracking_To_Charge == 0) Serial.println(F("OFF"));
  }

  if (CCW_Tracking_To_Charge_dueFlashStorage != 1) {
    Serial.print(F("Settings: Tracking Direction to Charge : "));
    if (CCW_Tracking_To_Charge == 1) Serial.println(F("CCW"));
    if (CCW_Tracking_To_Charge == 0) Serial.println(F("OFF"));
  }


  int CW_Tracking_To_Start_dueFlashStorage = dueFlashStorage.read(53);
  if (CW_Tracking_To_Start_dueFlashStorage == 1) {
    CW_Tracking_To_Start = dueFlashStorage.read(54);  
    Serial.print(F("FLASH: Tracking Direction to Start : "));
    if (CW_Tracking_To_Start == 1) Serial.println(F("CW"));
    if (CW_Tracking_To_Start == 0) Serial.println(F("OFF"));
  }

  if (CW_Tracking_To_Start_dueFlashStorage != 1) {
    Serial.print(F("Settings: Tracking Direction to Start : "));
    if (CW_Tracking_To_Start == 1) Serial.println(F("CW"));
    if (CW_Tracking_To_Start == 0) Serial.println(F("OFF"));
  }


  int CCW_Tracking_To_Start_dueFlashStorage = dueFlashStorage.read(55);
  if (CCW_Tracking_To_Start_dueFlashStorage == 1) {
    CCW_Tracking_To_Start = dueFlashStorage.read(56);  
    Serial.print(F("EERPOM: Tracking Direction to Start : "));
    if (CCW_Tracking_To_Start == 1) Serial.println(F("CCW"));
    if (CCW_Tracking_To_Start == 0) Serial.println(F("OFF"));
  }

  if (CCW_Tracking_To_Start_dueFlashStorage != 1) {
    Serial.print(F("Settings: Tracking Direction to Start : "));
    if (CCW_Tracking_To_Start == 1) Serial.println(F("CCW"));
    if (CCW_Tracking_To_Start == 0) Serial.println(F("OFF"));
  }

  int Max_Cycles_Straight_dueFlashStorage = dueFlashStorage.read(57);
  if (Max_Cycles_Straight_dueFlashStorage == 1) {
    Max_Cycles_Straight = dueFlashStorage.read(58);
    Max_Cycles_Straight = (Max_Cycles_Straight * 10);
    Serial.print(F("FLASH: Straight Line Cycles before turn: "));
    Serial.println(Max_Cycles_Straight);
  }

  if (Max_Cycles_Straight_dueFlashStorage != 1) {
    Serial.print(F("Settings: Straight Line Cycles before turn: "));
    Serial.println(Max_Cycles_Straight);
  }

  int Sonar_1_Activate_dueFlashStorage = dueFlashStorage.read(37);
  if (Sonar_1_Activate_dueFlashStorage == 1) {
    Sonar_1_Activate = dueFlashStorage.read(38);
    Serial.print(F("FLASH: Sonar 1 Activated: "));
    Serial.println(Sonar_1_Activate);
  }

  if (Sonar_1_Activate_dueFlashStorage != 1) {
    Serial.print(F("Settings: Sonar 1 Activated: "));
    Serial.println(Sonar_1_Activate);
  }

  int Sonar_2_Activate_dueFlashStorage = dueFlashStorage.read(39);
  if (Sonar_2_Activate_dueFlashStorage == 1) {
    Sonar_2_Activate = dueFlashStorage.read(40);
    Serial.print(F("FLASH: Sonar 2 Activated: "));
    Serial.println(Sonar_2_Activate);
  }

  if (Sonar_2_Activate_dueFlashStorage != 1) {
    Serial.print(F("Settings: Sonar 2 Activated: "));
    Serial.println(Sonar_2_Activate);
  }

  int Sonar_3_Activate_dueFlashStorage = dueFlashStorage.read(41);
  if (Sonar_3_Activate_dueFlashStorage == 1) {
    Sonar_3_Activate = dueFlashStorage.read(42);
    Serial.print(F("FLASH: Sonar 3 Activated: "));
    Serial.println(Sonar_3_Activate);
  }

  if (Sonar_3_Activate_dueFlashStorage != 1) {
    Serial.print(F("Settings: Sonar 3 Activated: "));
    Serial.println(Sonar_3_Activate);
  }

  int Max_Sonar_Hit_dueFlashStorage = dueFlashStorage.read(63);
  if (Max_Sonar_Hit_dueFlashStorage == 1) {
    Max_Sonar_Hit = dueFlashStorage.read(64); 
    Serial.print(F("FLASH: Sonar Sensitivity: "));
    Serial.println(Max_Sonar_Hit);
  }

  if (Max_Sonar_Hit_dueFlashStorage != 1) {
    Serial.print(F("Settings: Sonar Sensitivity: "));
    Serial.println(Max_Sonar_Hit);
  }

  int maxdistancesonar_dueFlashStorage = dueFlashStorage.read(65);
  if (maxdistancesonar_dueFlashStorage == 1) {
    maxdistancesonar = dueFlashStorage.read(66); 
    Serial.print(F("FLASH: Sonar Activation Distance: "));
    Serial.println(maxdistancesonar);
  }

  if (maxdistancesonar_dueFlashStorage != 1) {
    Serial.print(F("Settings: Sonar Activation Distance: "));
    Serial.println(maxdistancesonar);
  }

  int Perimeter_Wire_Enabled_dueFlashStorage = dueFlashStorage.read(67);
  if (Perimeter_Wire_Enabled_dueFlashStorage == 1) {
    Perimeter_Wire_Enabled = dueFlashStorage.read(68);  
    Serial.print(F("FLASH: Wire Module ON/OFF: "));
    if (Perimeter_Wire_Enabled == 0) Serial.println(F("OFF"));
    if (Perimeter_Wire_Enabled == 1) Serial.println(F("ON"));
  }

  if (Perimeter_Wire_Enabled_dueFlashStorage != 1) {
    Serial.print(F("Settings: Wire Module ON/OFF: "));
    if (Perimeter_Wire_Enabled == 0) Serial.println(F("OFF"));
    if (Perimeter_Wire_Enabled == 1) Serial.println(F("ON"));
  }


  int Max_Cycle_Wire_Find_dueFlashStorage = dueFlashStorage.read(69);
  if (Max_Cycle_Wire_Find_dueFlashStorage == 1) {
    Max_Cycle_Wire_Find = dueFlashStorage.read(70);
    Max_Cycle_Wire_Find = (Max_Cycle_Wire_Find * 10);
    Serial.print(F("FLASH: Track Cycles Forwards to find Wire "));
    Serial.println(Max_Cycle_Wire_Find);
  }


  if (Max_Cycle_Wire_Find_dueFlashStorage != 1) {
    Serial.print(F("Settings: Track Cycles Forwards to find Wire "));
    Serial.println(Max_Cycle_Wire_Find);
  }


  int Max_Cycle_Wire_Find_Back_dueFlashStorage = dueFlashStorage.read(71);
  if (Max_Cycle_Wire_Find_Back_dueFlashStorage == 1) {
    Max_Cycle_Wire_Find_Back = dueFlashStorage.read(72);
    Max_Cycle_Wire_Find_Back = (Max_Cycle_Wire_Find_Back * 10);
    Serial.print(F("FLASH: Track Cycles Back to find Wire "));
    Serial.println(Max_Cycle_Wire_Find_Back);
  }


  if (Max_Cycle_Wire_Find_Back_dueFlashStorage != 1) {
    Serial.print(F("Settings: Track Cycles Back to find Wire "));
    Serial.println(Max_Cycle_Wire_Find_Back);
  }


  int Max_Tracking_Turn_Right_dueFlashStorage = dueFlashStorage.read(73);
  if (Max_Tracking_Turn_Right_dueFlashStorage == 1) {
    Max_Tracking_Turn_Right = dueFlashStorage.read(74);
    Max_Tracking_Turn_Right = (Max_Tracking_Turn_Right * 10);
    Serial.print(F("FLASH: Wheel RH Cycles before restart "));
    Serial.println(Max_Tracking_Turn_Right);
  }

  if (Max_Tracking_Turn_Right_dueFlashStorage != 1) {
    Serial.print(F("Settings: Wheel RH Cycles before restart "));
    Serial.println(Max_Tracking_Turn_Right);
  }



  int Max_Tracking_Turn_Left_dueFlashStorage = dueFlashStorage.read(75);
  if (Max_Tracking_Turn_Left_dueFlashStorage == 1) {
    Max_Tracking_Turn_Left = dueFlashStorage.read(76);
    Max_Tracking_Turn_Left = (Max_Tracking_Turn_Left * 10);
    Serial.print(F("FLASH: Wheel LH Cycles before restart "));
    Serial.println(Max_Tracking_Turn_Left);
  }


  if (Max_Tracking_Turn_Left_dueFlashStorage != 1) {
    Serial.print(F("Settings: Wheel LH Cycles before restart "));
    Serial.println(Max_Tracking_Turn_Left);
  }


  int Rain_Sensor_Installed_dueFlashStorage = dueFlashStorage.read(77);
  if (Rain_Sensor_Installed_dueFlashStorage == 1) {
    Rain_Sensor_Installed = dueFlashStorage.read(78);  
    Serial.print(F("FLASH: Rain Sensor ON/OFF:  "));
    if (Rain_Sensor_Installed == 0) Serial.println(F("OFF"));
    if (Rain_Sensor_Installed == 1) Serial.println(F("ON"));
  }


  if (Rain_Sensor_Installed_dueFlashStorage != 1) {
    Serial.print(F("Settings: Rain Sensor ON/OFF: "));
    if (Rain_Sensor_Installed == 0) Serial.println(F("OFF"));
    if (Rain_Sensor_Installed == 1) Serial.println(F("ON"));
  }


  int Rain_Total_Hits_Go_Home_dueFlashStorage = dueFlashStorage.read(79);
  if (Rain_Total_Hits_Go_Home_dueFlashStorage == 1) {
    Rain_Total_Hits_Go_Home = dueFlashStorage.read(80); 
    Serial.print(F("Settings: Rain Sensitivity: "));
    Serial.println(Rain_Total_Hits_Go_Home);
  }

  if (Rain_Total_Hits_Go_Home_dueFlashStorage != 1) {
    Serial.print(F("Settings: Rain Sensitivity: "));
    Serial.println(Rain_Total_Hits_Go_Home);
  }

  int WIFI_Enabled_dueFlashStorage = dueFlashStorage.read(81);
  if (WIFI_Enabled_dueFlashStorage == 1) {
    WIFI_Enabled = dueFlashStorage.read(82);  
    Serial.print(F("FLASH: WIFI Enabled: "));
    if (WIFI_Enabled == 0) Serial.println(F("OFF"));
    if (WIFI_Enabled == 1) Serial.println(F("ON"));
  }

  if (WIFI_Enabled_dueFlashStorage != 1) {
    Serial.print(F("Settings: WIFI Enabled: "));
    if (WIFI_Enabled == 0) Serial.println(F("OFF"));
    if (WIFI_Enabled == 1) Serial.println(F("ON"));
  }

  int Cutting_Blades_Activate_dueFlashStorage = dueFlashStorage.read(83);
  if (Cutting_Blades_Activate_dueFlashStorage == 1) {
    Cutting_Blades_Activate = dueFlashStorage.read(84);  
    Serial.print(F("FLASH: CUTTING BLADE Motor SET TO : "));
    if (Cutting_Blades_Activate == 0) Serial.println(F("OFF"));
    if (Cutting_Blades_Activate == 1) Serial.println(F("ON"));
  }


  if (Cutting_Blades_Activate_dueFlashStorage != 1) {
    Serial.print(F("Settings: CUTTING BLADE Motor SET TO : "));
    if (Cutting_Blades_Activate == 0) Serial.println(F("OFF"));
    if (Cutting_Blades_Activate == 1) Serial.println(F("ON"));
  }

  int Low_Battery_Instances_Chg_dueFlashStorage = dueFlashStorage.read(85);
  if (Low_Battery_Instances_Chg_dueFlashStorage == 1) {
    Low_Battery_Instances_Chg = dueFlashStorage.read(86); 
    Serial.print(F("FLASH: Battery to Charge Sensitivity: "));
    Serial.println(Low_Battery_Instances_Chg);
  }

  if (Low_Battery_Instances_Chg_dueFlashStorage != 1) {
    Serial.print(F("Settings: Battery to Charge Sensitivity: "));
    Serial.println(Low_Battery_Instances_Chg);
  }


  int Bumper_Activate_Frnt_dueFlashStorage = dueFlashStorage.read(90);
  if (Bumper_Activate_Frnt_dueFlashStorage == 1) {
    Bumper_Activate_Frnt = dueFlashStorage.read(91);  
    Serial.print(F("FLASH: Bumper Bar Enabled: "));
    if (Bumper_Activate_Frnt == 0) Serial.println(F("OFF"));
    if (Bumper_Activate_Frnt == 1) Serial.println(F("ON"));
  }

  if (Bumper_Activate_Frnt_dueFlashStorage != 1) {
    Serial.print(F("Settings: Bumper Bar Enabled: "));
    if (Bumper_Activate_Frnt == 0) Serial.println(F("OFF"));
    if (Bumper_Activate_Frnt == 1) Serial.println(F("ON"));
  }


  int Turn_90_Delay_LH_dueFlashStorage = dueFlashStorage.read(101);
  if (Turn_90_Delay_LH_dueFlashStorage == 1) {
    Turn_90_Delay_LH = dueFlashStorage.read(102);  
    Turn_90_Delay_LH = Turn_90_Delay_LH * 10;
    Serial.print(F("FLASH: Turn_90_Delay_LH Enabled: "));
    Serial.println(Turn_90_Delay_LH);
  }

  if (Turn_90_Delay_LH_dueFlashStorage != 1) {
    Serial.print(F("Settings: Turn_90_Delay_LH Enabled: "));
    Serial.println(Turn_90_Delay_LH);
  }


  int Turn_90_Delay_RH_dueFlashStorage = dueFlashStorage.read(103);
  if (Turn_90_Delay_RH_dueFlashStorage == 1) {
    Turn_90_Delay_RH = dueFlashStorage.read(104);  
    Turn_90_Delay_RH = Turn_90_Delay_RH * 10;
    Serial.print(F("FLASH: Turn_90_Delay_RH Enabled: "));
    Serial.println(Turn_90_Delay_RH);
  }

  if (Turn_90_Delay_RH_dueFlashStorage != 1) {
    Serial.print(F("Settings: Turn_90_Delay_RH Enabled: "));
    Serial.println(Turn_90_Delay_RH);
  }


  int Line_Length_Cycles_dueFlashStorage = dueFlashStorage.read(105);
  if (Line_Length_Cycles_dueFlashStorage == 1) {
    Line_Length_Cycles = dueFlashStorage.read(106);  
    Line_Length_Cycles = Line_Length_Cycles * 10;
    Serial.print(F("FLASH: Line_Length_Cycles Enabled: "));
    Serial.println(Line_Length_Cycles);
  }


  if (Line_Length_Cycles_dueFlashStorage != 1) {
    Serial.print(F("Settings: Line_Length_Cycles Enabled: "));
    Serial.println(Line_Length_Cycles);
  }



  int GPS_Enabled_dueFlashStorage = dueFlashStorage.read(107);
  if (GPS_Enabled_dueFlashStorage == 1) {
    GPS_Enabled = dueFlashStorage.read(108);  
    Serial.print(F("FLASH: GPS Enabled: "));
    if (GPS_Enabled == 0) Serial.println(F("Disabled"));
    if (GPS_Enabled == 1) Serial.println(F("Enabled"));
  }

  if (GPS_Enabled_dueFlashStorage != 1) {
    Serial.print(F("Settings: GPS Enabled: "));
    if (GPS_Enabled == 0) Serial.println(F("Disabled"));
    if (GPS_Enabled == 1) Serial.println(F("Enabled"));
  }

  int GYRO_Enabled_dueFlashStorage = dueFlashStorage.read(109);
  if (GYRO_Enabled_dueFlashStorage == 1) {
    GYRO_Enabled = dueFlashStorage.read(110);  
    Serial.print(F("FLASH: GYRO Enabled: "));
    if (GYRO_Enabled == 0) Serial.println(F("Disabled"));
    if (GYRO_Enabled == 1) Serial.println(F("Enabled"));
  }

  if (GYRO_Enabled_dueFlashStorage == 1) {
    Serial.print(F("Settings: GYRO Enabled: "));
    if (GYRO_Enabled == 0) Serial.println(F("Disabled"));
    if (GYRO_Enabled == 1) Serial.println(F("Enabled"));
  }

  
  int GPower_dueFlashStorage = dueFlashStorage.read(111);
  if (GPower_dueFlashStorage == 1) {
    GPower = dueFlashStorage.read(112); 
    GPower = GPower / 10; 
    Serial.print(F("FLASH: GYRO PID : "));
    Serial.println(GPower);
  }

  if (GPower_dueFlashStorage != 1) {
    Serial.print(F("Settings: GYRO PID : "));
    Serial.println(GPower);
  }

  int Wheel_Amp_Sensor_ON_Enabled_dueFlashStorage = dueFlashStorage.read(115);
  if (Wheel_Amp_Sensor_ON_Enabled_dueFlashStorage == 1) {
    Wheel_Amp_Sensor_ON = dueFlashStorage.read(116);  
    Serial.print(F("FLASH: Wheel Block Amp Sensor Enabled: "));
    if (Wheel_Amp_Sensor_ON == 0) Serial.println(F("Disabled"));
    if (Wheel_Amp_Sensor_ON == 1) Serial.println(F("Enabled"));
  }


  if (Wheel_Amp_Sensor_ON_Enabled_dueFlashStorage != 1) {
    Serial.print(F("Settings: Wheel Block Amp Sensor Enabled: "));
    if (Wheel_Amp_Sensor_ON == 0) Serial.println(F("Disabled"));
    if (Wheel_Amp_Sensor_ON == 1) Serial.println(F("Enabled"));
  }

  
  int Max_Wheel_Amps_dueFlashStorage = dueFlashStorage.read(117);
  if (Max_Wheel_Amps_dueFlashStorage == 1) {
    Max_Wheel_Amps = dueFlashStorage.read(118); 
    Max_Wheel_Amps = Max_Wheel_Amps / 10; 
    Serial.print(F("FLASH: Wheel Block Amps: "));
    Serial.println(Max_Wheel_Amps);
  }

  if (Max_Wheel_Amps_dueFlashStorage != 1) {
    Max_Wheel_Amps = Max_Wheel_Amps / 10; 
    Serial.print(F("Settings: Wheel Block Amps: "));
    Serial.println(Max_Wheel_Amps);
  }

  if (Max_Wheel_Amps_dueFlashStorage != 1) {
    Max_Wheel_Amps = Max_Wheel_Amps / 10; 
    Serial.print(F("Settings: Wheel Block Amps: "));
    Serial.println(Max_Wheel_Amps);
  }







Serial.println(F("*********** DONE **************"));
 delay(500);

#endif
}


void Clear_FLASH() {
  
  #if defined(BOARD_DUE)
  
  dueFlashStorage.write(1,0);      // Clear Alarm 1
  dueFlashStorage.write(5,0);      // Clear Alarm 2
  dueFlashStorage.write(9,0);      // Clear Alarm 3
  dueFlashStorage.write(13,0);     // Clear PWM Left Wheel
  dueFlashStorage.write(15,0);     // Clear PWM Right Wheel
  dueFlashStorage.write(17,0);     // Clear PWM Blade
  dueFlashStorage.write(19,0);     // Clear Compass Setting dueFlashStorage
  dueFlashStorage.write(21,0);     // Clear PID Setting
  dueFlashStorage.write(23,0);     // Clear Pattern Mow
  dueFlashStorage.write(25,0);     // Clear Volt Minimum
  dueFlashStorage.write(27,0);     // Clear Compass Home
  dueFlashStorage.write(29,0);     // Clear Tilt Tip Safety
  dueFlashStorage.write(31,0);     // Clear Turn Time Min
  dueFlashStorage.write(33,0);     // Clear Turn Time Max
  dueFlashStorage.write(35,0);     // Clear Reverse Time
  dueFlashStorage.write(37,0);     // Clear Sonar 1
  dueFlashStorage.write(39,0);     // Clear Sonar 2
  dueFlashStorage.write(41,0);     // Clear Sonar 3
  dueFlashStorage.write(43,0);     // Clear Zone 1 Cycles
  dueFlashStorage.write(45,0);     // Clear Zone 2 Cycles
  dueFlashStorage.write(47,0);     // Clear Charging Station Options
  dueFlashStorage.write(49,0);     // Reset CW and CCW Exit and Dock Directions
  dueFlashStorage.write(51,0);     //  CW CCW Tracking
  dueFlashStorage.write(53,0);     //  CW CCW Tracking
  dueFlashStorage.write(55,0);     //  CW CCW Tracking
  dueFlashStorage.write(57,0);     // Max Cycle Straight
  dueFlashStorage.write(59,0);     // Compass Heading HOld ON/OFF
  dueFlashStorage.write(61,0);     // Compass PID reset.
  dueFlashStorage.write(63,0);     // Sonar sensitivity.
  dueFlashStorage.write(65,0);      // MAx distance sonar reset.
  dueFlashStorage.write(67,0);     // Wire Sensor ON/OFF
  dueFlashStorage.write(69,0);     // Track cycles Forwards
  dueFlashStorage.write(71,0);     // Track Cycles Back
  dueFlashStorage.write(73,0);     // RH Cycles to restart
  dueFlashStorage.write(75,0);     // LH Cycles to restart
  dueFlashStorage.write(77,0);     // Rain ON/OFF
  dueFlashStorage.write(79,0);     // Rain sensitivity
  dueFlashStorage.write(81,0);     // WIFI ON/OFF
  dueFlashStorage.write(83,0);     // Cutting Blades ON/OFF
  dueFlashStorage.write(85,0);     // Batt sensitivity;
  dueFlashStorage.write(87,0);     // Alarm Actions 1-3
  dueFlashStorage.write(88,0);
  dueFlashStorage.write(89,0);
  dueFlashStorage.write(90,0);     // Bumper Bar
  dueFlashStorage.write(92,0);     // Tip Over sensor
  dueFlashStorage.write(94,0);     // Wheel Slow Speed LH
  dueFlashStorage.write(96,0);     // Wheel Slow Speed RH
  dueFlashStorage.write(98,0);     // Slow MAG Point
  dueFlashStorage.write(102,0);    // Turn_90_Delay_LH 
  dueFlashStorage.write(104,0);    // Turn_90_Delay_RH
  dueFlashStorage.write(106,0);    // Line_Length_Cycles
  dueFlashStorage.write(107,0);    // GPS Enabled
  dueFlashStorage.write(109,0);    // GYRO Enabled
  dueFlashStorage.write(111,0);    // GYRO Power
  dueFlashStorage.write(113,0);    // Compass Setup Mode
  dueFlashStorage.write(115,0);    // Wheel amp sensor 
  dueFlashStorage.write(117,0);    // Wheel amp sensor value

  #endif
}
