// Access to the PCB onboard Clock and arduino clock Module
// Alarm actions.


void Get_Current_Time_Print_On_Serial_Monitor() {
      
      if (PCB == 0) {
        Time t = rtc.time();        
        Time_Hour = t.hr;
        Time_Minute = t.min;
        Time_Second = t.sec;    
        }
      if (PCB == 1) {
        Display_DS3231_Time();          // onboard RTC clock on the PCB board

        }

      Serial.print(F("Time:"));  
      if (Time_Hour < 10) Serial.print(F("0"));
      Serial.print(Time_Hour);
      Serial.print(F(":"));
      if (Time_Minute < 10) Serial.print(F("0"));
      Serial.print(Time_Minute);
      Serial.print(F(":"));
      if (Time_Second < 10) Serial.print(F("0"));
      Serial.print(Time_Second);
      Serial.print(F("|"));
      
      }


// Arduino RTC Clock Module
void DisplayTime_DS1302()   {
  Serial.print(F("Time:"));
  Time t = rtc.time();  
 
  // Name the day of the week.
  const String day = dayAsString(t.day);

  // Format the time and date and insert into the temporary buffer.
  char buf[50];
  snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d",
           day.c_str(),
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);

  // Print the formatted string to serial so we can see the time.
  Serial.print(buf);
 
  }






// PCB Clock
void Set_DS3231_Time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year) {
    // sets time and date data to DS3231
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0);                                // set next input to start at the seconds register
    Wire.write(decToBcd(second));                 // set seconds
    Wire.write(decToBcd(minute));                 // set minutes
    Wire.write(decToBcd(hour));                   // set hours
    Wire.write(decToBcd(dayOfWeek));              // set day of week (1=Sunday, 7=Saturday)
    Wire.write(decToBcd(dayOfMonth));             // set date (1 to 31)
    Wire.write(decToBcd(month));                  // set month
    Wire.write(decToBcd(year));                   // set year (0 to 99)
    Wire.endTransmission();
    }




// PCB Clock
void Display_DS3231_Time() {
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  Read_DS3231_PCB_Time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  static byte trigger = 1;
  Time_Hour = hour;
  Time_Minute = minute;
  Time_Second = second;

  if (second >= 30) //use second for once a minute, use minute for once an hour
  {
    if (trigger)
    {
    //digitalWrite(LED_BUILTIN, HIGH);
    trigger = 0;
    }
    else
    {
      trigger = 1;
    }
   
  }
  else
  {
    if (trigger)
    {
    //digitalWrite(LED_BUILTIN, LOW);
    trigger = 0;
    }
    else
    {trigger = 1;
    }
  }
}



// PCB Clock
void Read_DS3231_PCB_Time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year) {
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0);                                                    // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);                          // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
  }



void Activate_Alarms() {
  
  // Manual ALARM 1
  if (Alarm_1_ON == 1) {  
     if ((Time_Hour== Alarm_1_Hour) && (Time_Minute == Alarm_1_Minute)) {
       Serial.println(F(""));
       Serial.println(F("ALARM 1 - Activated "));
       Serial.println(F(""));
       delay(2000);

        // Exit Dock : Zone 1
       if (Alarm_1_Action == 1) Alarm_Start_Exit_Zone_1();           // Exit Dock : Zone 1
       if (Alarm_1_Action == 2) Alarm_Start_Exit_Zone_2();           // Exit Dock : Zone 2
       if (Alarm_1_Action == 3) {                                    // Mow The Line
        Alarm_Start_Exit_Zone_2();
        Blade_Override = 1;
        } 
       if (Alarm_1_Action == 4) Alarm_Start_Quick_Go();              // Quick Go

       
       if (Alarm_1_Action == 5) {                                    // Custom Option
        Serial.println(F("Insert Custom Code"));
         }      
       }
    }
 
  
  // Manual ALARM 2
  if (Alarm_2_ON == 1) {  
     if ((Time_Hour == Alarm_2_Hour) && (Time_Minute == Alarm_2_Minute)) {
       Serial.println(F(""));
       Serial.println(F("ALARM 2 - Activated "));
       Serial.println(F(""));
       delay(2000);

        // Exit Dock : Zone 1
       if (Alarm_2_Action == 1) Alarm_Start_Exit_Zone_1();           // Exit Dock : Zone 1
       if (Alarm_2_Action == 2) Alarm_Start_Exit_Zone_2();           // Exit Dock : Zone 2
       if (Alarm_2_Action == 3) {                                    // Mow The Line
        Alarm_Start_Exit_Zone_2();
        Blade_Override = 1;
        }
       if (Alarm_2_Action == 4) Alarm_Start_Quick_Go();              // Quick Go

       
       if (Alarm_2_Action == 5) {                                    // Custom Option
        Serial.println(F("Insert Custom Code"));
         }      
       }
    }
     
  // Manual ALARM 3
  if (Alarm_3_ON == 1) {  
     if ((Time_Hour == Alarm_3_Hour) && (Time_Minute == Alarm_3_Minute)) {
       Serial.println(F(""));
       Serial.println(F("ALARM 3 - Activated "));
       Serial.println(F(""));
       delay(2000);

        // Exit Dock : Zone 1
       if (Alarm_3_Action == 1) Alarm_Start_Exit_Zone_1();           // Exit Dock : Zone 1
       if (Alarm_3_Action == 2) Alarm_Start_Exit_Zone_2();           // Exit Dock : Zone 2
       if (Alarm_3_Action == 3) {                                    // Mow The Line
        Alarm_Start_Exit_Zone_2();
        Blade_Override = 1;
        }
       if (Alarm_3_Action == 4) Alarm_Start_Quick_Go();              // Quick Go

       
       if (Alarm_3_Action == 5) {                                    // Custom Option
        Serial.println(F("Insert Custom Code"));
         }      
       }
    }

}


void Alarm_Start_Exit_Zone_2() {
         Serial.print(F("Exit Dock| "));
         Serial.println(F("Zone 2"));
       
         #if defined(LCD_KEYPAD)
           lcd.clear();
           lcd.print(F("Alarm Start"));
           lcd.setCursor(0,1);
           lcd.print(F("Exit Dock Z2"));
           delay(500);
           lcd.clear();
           #endif
       
         Mower_Parked = 0;
         Exiting_Dock = 2;
         Mower_Error_Value = 0;
         Send_Mower_Docked_Data();                                   // Send the Docked TX Data package to the mower.       
         Exit_Zone = 2;
         Track_Wire_Itterations = Track_Wire_Zone_1_Cycles;
         Exiting_Dock = 1;
         Manouver_Exit_To_Zone_X();    
         }



void Alarm_Start_Exit_Zone_1() {
  
         Serial.print(F("Exit Dock| "));
         Serial.println(F("Zone 1"));
       
         #if defined(LCD_KEYPAD)
           lcd.clear();
           lcd.print(F("Alarm Start"));
           lcd.setCursor(0,1);
           lcd.print(F("Exit Dock Z1"));
           delay(500);
           lcd.clear();
           #endif
       
         Exiting_Dock = 1;
         Mower_Error_Value = 0;
         Send_Mower_Docked_Data();                                   // Send the Docked TX Data package to the mower.       
         Exit_Zone = 1;
         Track_Wire_Itterations = Track_Wire_Zone_1_Cycles;
         Manouver_Exit_To_Zone_X();     
         }


 void Alarm_Start_Quick_Go() {
   Serial.println(F("Alarm - Quick Start"));
   #if defined(LCD_KEYPAD)
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print(F("WIFI Start"));
     #endif
   

   if (Mower_Docked == 0) { 
     Manouver_Start_Mower();
     #if defined(LCD_KEYPAD)
     lcd.clear();   
     #endif 
     if (TFT_Screen_Menu == 1) Send_Mower_Docked_Data();    // Send the Docked TX Data package to the mower.
     }    
   else Serial.println(F("Mower Docked - Quick Start not possible"));   
   }



// Set when choosing an option of 1hr or 2hr mow etc.
void Check_Timed_Mow() {
    
  if (Alarm_Timed_Mow_ON == 1) {  
     if ((Time_Hour == Alarm_Timed_Mow_Hour) && (Time_Minute == Alarm_Timed_Mow_Minute)) {
       Serial.println(F("Timed Mow Complete"));
       delay(2000);
       //Insert action for Timed Mow Alarm Here
         if (Use_Charging_Station == 1) Manouver_Go_To_Charging_Station();                       // Stops the mowing and sends the mower back to the charging station via the permieter wire
         if (Use_Charging_Station == 0) Manouver_Park_The_Mower_Low_Batt();                      // Parks the mower with a low battery warning
       }
     }
}
 

// Prints the alarms set to the serial monitor
void Display_Next_Alarm()  {
  //Print_Day();
  
  if (Alarm_1_ON == 1 ) {
      Serial.print(F("|Alarm 1:"));
      Serial.print(Alarm_1_Hour);
      Serial.print(F(":"));
      if (Alarm_1_Minute < 10) Serial.print ("0");
      Serial.print(Alarm_1_Minute);
      Serial.print("|");
      }
  if (Alarm_1_ON == 0) Serial.print("|A1arm_1:OFF");
 
  if (Alarm_2_ON == 1) {
  Serial.print(F("|Alarm 2:"));
  Serial.print(Alarm_2_Hour);
  Serial.print(F(":"));
  if (Alarm_2_Minute < 10) Serial.print ("0");
  Serial.print(Alarm_2_Minute);
  Serial.print("|");
  }
  if (Alarm_2_ON == 0) Serial.print("|Alarm_2:OFF");
  
  if (Alarm_3_ON == 1) {
  Serial.print(F("|Alarm 3:"));
  Serial.print(Alarm_3_Hour);
  Serial.print(F(":"));
  if (Alarm_3_Minute < 10) Serial.print ("0");
  Serial.print(Alarm_3_Minute);
  Serial.print("|");
  }
  if (Alarm_3_ON == 0) Serial.print("|Alarm_3:OFF");

   
}




void Manage_Alarms() {
    Alarm_Timed_Mow_ON = 0;                                           // Turns off the 1 hr Alarm
    if (Alarm_1_Repeat == 0) Alarm_1_ON = 0;
    if (Alarm_2_Repeat == 0) Alarm_2_ON = 0;
    if (Alarm_3_Repeat == 0) Alarm_3_ON = 0;
    }




// Arduino RTC Clock Module
void Manual_Set_Time_On_DS1302(){
   // Set_Time to 1 in the setting menu to set time.  Load the sketch then immediatley Set_Time = 0 and reload the sketch.
   rtc.writeProtect(false);
   rtc.halt(false);
   Time t(2019, 9, 14, 17, 03, 00, Time::kSaturday);            // Year XXXX, Month XX, Day XX, Hour XX, Minute XX, Second, kXYZday
   rtc.time(t);    
   delay(10);
   }


// PCB Clock
void Manual_Set_Time_DS3231() {
    // Set_Time to 1 in the setting menu to set time.  Load the sketch then immediatley Set_Time = 0 and reload the sketch.
    Set_DS3231_Time(30,56,22,2,14,7,20);    //second, minute, hour, dayof week, day of month, month, year
    }
