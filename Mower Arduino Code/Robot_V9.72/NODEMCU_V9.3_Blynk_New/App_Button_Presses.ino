
// Actions for the buttons pressed in the APP
// The Virtual pin numbers give the identification for the button



BLYNK_WRITE(V0)        // Quick Start Mowing Function
{
  Clear_APP(); 
  
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  if (pinValue == 1) StartMower();       
  //Clear_APP();      
  delay(100);
  Blynk.virtualWrite(V0, HIGH);   // Start Mowing Buton ON
  Blynk.virtualWrite(V11, 0);
  Sync_Settings_Now = 0;
  }


BLYNK_WRITE(V1)      // Go to Docking Station
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  if (pinValue == 1) Going_Home();
  //Clear_APP();
  delay(100);
  Blynk.virtualWrite(V1, HIGH);           // Docking Button ON
  Blynk.virtualWrite(V11, 0);

}



BLYNK_WRITE(V2)      // Pause Mower Function
{
  int pinValue = param.asInt(); // assigning incoming value from pin V2 to a variable
  if (pinValue == 1)  Pause_Mower();
  //Clear_APP();
  delay(100);
  Blynk.virtualWrite(V2, HIGH);           // Pause Button ON
  Blynk.virtualWrite(V11, 0);
   }





BLYNK_WRITE(V10)      // Exit Dock Function
{
  int pinValue = param.asInt(); // assigning incoming value from pin V10 to a variable
  if (pinValue == 1) Exit_Dock();
  //Clear_APP();
  delay(100);
  Blynk.virtualWrite(V10, HIGH);        // Dock Button ON
  Blynk.virtualWrite(V11, 0);
  
  

}


BLYNK_WRITE(V13)                        // Manuel Forward Motion
{
  if (Manuel_Mode == 1) {
  int pinValue = param.asInt(); // assigning incoming value from pin V10 to a variable
  if (pinValue == 1) {
    Serial.println("");
    Serial.println("FWD");
    Serial.println("");    
    transmit_blynk_code = 7;  //+10 added to the code TX
    Transmit_Blynk_Data_to_Mega();
    Blynk.virtualWrite(V13, LOW);   // Start Mowing Buton ON
    }
  }
  

}

BLYNK_WRITE(V14)                        // Manuel Reverse Motion
{
  if (Manuel_Mode == 1) {
  int pinValue = param.asInt(); // assigning incoming value from pin V10 to a variable
  if (pinValue == 1) {
    Serial.println("");
    Serial.println("BACK");
    Serial.println("");    
    transmit_blynk_code = 8;   //+10 added to the code TX
    Transmit_Blynk_Data_to_Mega();
    }
  }
}

BLYNK_WRITE(V15)                        // Manuel Left Turn
{
  if (Manuel_Mode == 1) {
  int pinValue = param.asInt(); // assigning incoming value from pin V10 to a variable
    if (pinValue == 1) {
    Serial.println("");
    Serial.println("LEFT");
    Serial.println("");   
    transmit_blynk_code = 9;    //+10 added to the code TX
    Transmit_Blynk_Data_to_Mega();
    }
  }
}

BLYNK_WRITE(V16)                        // Manuel Right Turn
{
  if (Manuel_Mode == 1) {
  int pinValue = param.asInt(); // assigning incoming value from pin V10 to a variable
    if (pinValue == 1) {
    Serial.println("");
    Serial.println("RIGHT");
    Serial.println("");          
    transmit_blynk_code = 10;  //+10 added to the code TX
    Transmit_Blynk_Data_to_Mega();
    }


  }
}

BLYNK_WRITE(V27)                        // Manuel Dock in this direction.
{
  if (Manuel_Mode == 1) {                // Check its in Manuel Mode.
    int pinValue = param.asInt(); // assigning incoming value from pin V10 to a variable
    if (pinValue == 1) {
        transmit_blynk_code = 46;  //+10 added to the code TX means 56 on the MEGA
        Transmit_Blynk_Data_to_Mega();
        Blynk.virtualWrite(V1, HIGH);           // Docking Button ON
        Blynk.virtualWrite(V11, 0);
        Update_Blynk_App_With_Status();  
        Automatic_Mode = 1;
        Manuel_Mode = 0;
        Blynk.virtualWrite(V27, 0);       // Go To Dock Button
        Blynk.virtualWrite(V4, 1023);       // Go To Dock Button
        } 
    }
}


BLYNK_WRITE(V65)                        // Drill Hole
{
  int pinValue = param.asInt(); // assigning incoming value from pin V10 to a variable
    if (pinValue == 1) {
    Serial.println("");
    Serial.println("Drill Activated");
    transmit_blynk_code = 65;  //+10 added to the code TX
    Transmit_Blynk_Data_to_Mega();
    }

}


BLYNK_WRITE(V4) {
  switch (param.asInt())
  {
    case 1: { // Item 1
        Serial.println("Automatic Mode");
        Automatic_Mode = 1;
        Manuel_Mode = 0;
        ErrorLED.off();
        Set_To_Automatic_Mode_Random();
        //Update_Blynk_App_With_Status();        
        break;
      }
    case 2: { // Item 2
        Serial.println("Automatic Spiral");
        Automatic_Mode = 2;
        Manuel_Mode = 0;
        Set_To_Automatic_Mode_Spiral();
        //Update_Blynk_App_With_Status();
        break;
      }
    
    case 3: { // Item 3
        Serial.println("Automatic Parallel");
        Manuel_Mode = 0;
        Automatic_Mode = 1;
        Set_To_Automatic_Mode_Parallel();
        Mower_Parked = 0;
        Mower_Docked = 0;
        Mower_Running = 0;
        //Update_Blynk_App_With_Status();
        break;
      }

    
    case 4: { // Item 4
        Serial.println("Manuel Mode");
        Manuel_Mode = 1;
        Automatic_Mode = 0;
        Set_To_Manuel_Mode();
        Mower_Parked = 0;
        Mower_Docked = 0;
        Mower_Running = 0;
        ErrorLED.off();
        //Update_Blynk_App_With_Status();        
        break;
      }

    }
}


// Relay Switch for NODEMCU
BLYNK_WRITE(V17) {
  switch (param.asInt())
  {
    case 1: { // Relay ON
        Serial.println("RELAY ON / CAM ON");
        digitalWrite(D6, HIGH);
        break;
      }
    case 2: { // Relay OFF
        Serial.println("RELAY OFF / CAM OFF");
        digitalWrite(D6, LOW);
        break;
      }
    

    }
}

// Reset Mower MEGA
BLYNK_WRITE(V18)                        
{
      Serial.print("|MEGA ERROR");  
      Blynk.virtualWrite(V11, 1023);
      digitalWrite(D5, HIGH); 
      digitalWrite(D5, LOW); 
      delay(1000);
      digitalWrite(D5, HIGH); 
      delay(2000);
      Blynk.virtualWrite(V11, 0);
      Total_Error = 0;
      Error_Loop = 0;
      Error_Volt = 0;


  }


// Reset Mower MEGA
BLYNK_WRITE(V19)                        
{
  Serial.print("|ERROR Re-Start");  
  Clear_APP(); 
  
  StartMower();       
  delay(100);
  Blynk.virtualWrite(V0, HIGH);   // Start Mowing Buton ON
  Blynk.virtualWrite(V11, 0);
  }


// Compass ON/OFF

BLYNK_WRITE(V20) {
  Serial.println("");
  switch (param.asInt())
  {
    case 1: { // Compass ON
        Serial.println("Compass ON");
        transmit_blynk_code = 30;  //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();BlynkEdgent.run();
        Compass_Activate = 1;
        break;
      }
    case 2: { // Compass OFF
        Serial.println("Compass OFF");
        transmit_blynk_code = 31;  //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Compass_Activate = 0;
        break;
      }  
    }
}


// Heading Hold ON/OFF
BLYNK_WRITE(V21) {
  Serial.println("");
  switch (param.asInt())
  {
    case 1: { // Relay ON
        Serial.println("Heading Hold ON");
        transmit_blynk_code = 32;  //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Compass_Heading_Hold_Enabled = 1;
        break;
      }
    case 2: { // Relay OFF
        Serial.println("HEading Hold OFF");
        transmit_blynk_code = 33;   //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Compass_Heading_Hold_Enabled = 0;
        break;
      }  
    }
}


// GYRO ON/OFF
BLYNK_WRITE(V22) {
  Serial.println("");
  switch (param.asInt())
  {
    case 1: { // GYRO ON
        Serial.println("GYRO ON");
        transmit_blynk_code = 34;  //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        GYRO_Enabled = 1;
        break;
      }
    case 2: { // GYRO OFF
        Serial.println("GYRO OFF");
        transmit_blynk_code = 35; //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega(); 
        GYRO_Enabled = 0;
        break;
      }  
    }
}



// Sonar 1 ON/OFF
BLYNK_WRITE(V24) {
  Serial.println("");
  switch (param.asInt())
  {
    case 1: { // Sonar 1 ON
        Serial.println("SONAR 1 ON"); 
        transmit_blynk_code = 37;  //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Sonar_1_Activate = 1;
        break;
      }
    case 2: { // Sonar 1 OFF
        Serial.println("SONAR 1 OFF");
        transmit_blynk_code = 38;  //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Sonar_1_Activate = 0;
        break;
      }  
    }
}

// Sonar 2 ON/OFF
BLYNK_WRITE(V25) {
  Serial.println("");
  switch (param.asInt())
  {
    case 1: { // Sonar 2 ON
        Serial.println("SONAR 2 ON");
        transmit_blynk_code = 39;  //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Sonar_2_Activate = 1;
        break;
      }
    case 2: { // Sonar 2 OFF
        Serial.println("SONAR 2 OFF");
        transmit_blynk_code = 40;  //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Sonar_2_Activate = 0;
        break;
      }  
    }
}


// Sonar 3 ON/OFF
BLYNK_WRITE(V26) {
  Serial.println("");
  switch (param.asInt())
  {
    case 1: { // Sonar 3 ON
        Serial.println("SONAR 3 ON");
        transmit_blynk_code = 41; //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Sonar_3_Activate = 1;
        break;
      }
    case 2: { // Sonar 3 OFF
        Serial.println("SONAR 3 OFF");
        transmit_blynk_code = 42;  //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Sonar_3_Activate = 0;
        break;
      }  
    }
}


// Bumper ON/OFF
BLYNK_WRITE(V29) {
  Serial.println("");
  switch (param.asInt())
  
  {
    case 1: { // Bumper ON
        Serial.println("Bumper ON");
        transmit_blynk_code = 43; //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Bumper_Activate_Frnt = 1;
        break;
      }
    case 2: { // Bumper OFF
        Serial.println("Bumper OFF");
        transmit_blynk_code = 44;  //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Bumper_Activate_Frnt = 0;
        break;
      }  
    }
}


// Alarm 1 ON/OFF
BLYNK_WRITE(V30) {
  Serial.println("");
  switch (param.asInt())
  
  {
    case 1: { // Alarm 1 ON
        Serial.println("Alarm 1 ON");
        transmit_blynk_code = 47; //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Alarm_1_ON = 1;
        break;
      }
    case 2: { // Alarm 1 OFF
        Serial.println("Alarm 1 OFF");
        transmit_blynk_code = 48;  //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Alarm_1_ON = 0;
        break;
      }  
    }
}


// Alarm 2 ON/OFF
BLYNK_WRITE(V31) {
  Serial.println("");
  switch (param.asInt())
  
  {
    case 1: { // Alarm 2 ON
        Serial.println("Alarm 2 ON");
        transmit_blynk_code = 49; //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Alarm_2_ON = 1;
        break;
      }
    case 2: { // Alarm 2 OFF
        Serial.println("Alarm 2 OFF");
        transmit_blynk_code = 50;  //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Alarm_2_ON = 0;
        break;
      }  
    }
}


// Alarm 3 ON/OFF
BLYNK_WRITE(V32) {
  Serial.println("");
  switch (param.asInt())
  
  {
    case 1: { // Alarm 3 ON
        Serial.println("Alarm 3 ON");
        transmit_blynk_code = 51; //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Alarm_3_ON = 1;
        break;
      }
    case 2: { // Alarm 2 OFF
        Serial.println("Alarm 3 OFF");
        transmit_blynk_code = 52;  //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Alarm_3_ON = 0;
        break;
      }  
    }
} 


// GPS ON/OFF
BLYNK_WRITE(V33) {
  Serial.println("");
  switch (param.asInt())
  
  {
    case 1: { // GPS ON
        Serial.println("GPS ON");
        transmit_blynk_code = 55; //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        GPS_Enabled = 1;
        break;
      }
    case 2: { // GPS OFF
        Serial.println("GPS OFF");
        transmit_blynk_code = 56;  //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        GPS_Enabled = 0;
        break;
      }  
    }
} 

BLYNK_WRITE(V34) {
   Serial.println("");
  switch (param.asInt())
 
  {
    case 1: { // Angle Sensor ON
        Serial.println("Angle Sensor ON");
        transmit_blynk_code = 57; //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Angle_Sensor_Enabled = 1;
        break;
      }
    case 2: { // Angle Sensor OFF
        Serial.println("Angle Sensor OFF");
        transmit_blynk_code = 58;  //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Angle_Sensor_Enabled = 0;
        break;
      }  
    }
} 



BLYNK_WRITE(V35) {
  Serial.println("");
  switch (param.asInt())
  
  {
    case 1: { // Tip Sensor
        Serial.println("Tip Sensor ON");
        transmit_blynk_code = 59; //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Tip_Over_Sensor_Enabled = 1;
        break;
      }
    case 2: { // Tip Sensor
        Serial.println("Tip Sensor OFF");
        transmit_blynk_code = 60;  //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Tip_Over_Sensor_Enabled = 0;
        break;
      }  
    }
}


BLYNK_WRITE(V36) {
  Serial.println("");
  switch (param.asInt()) 
  {
    case 1: { // Wheel Amp Sensor
        Serial.println("Wheel Amp Sensor ON");
        transmit_blynk_code = 61; //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Wheel_Amp_Sensor_ON = 1;
        break;
      }
    case 2: { // Wheel Amp Sensor
        Serial.println("Wheel Amp Sensor OFF");
        transmit_blynk_code = 62;  //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Wheel_Amp_Sensor_ON = 0;
        break;
      }  
    }
}



// Set the WIFI APP buttons to the correct status
// Uses a 2 step transmission to set both boards into the right
// Status to receive a reliable long transfer list
// The sync part of it is written as a sperate function so it can be called
// periodically to update the APP.

// Update App Settings Sync 1
BLYNK_WRITE(V23)    {                     
  switch (param.asInt())
  {
    case 1: {    // Update App 
        Serial.println("");
        Serial.println("Manuel Sync Button Pressed");
        Serial.println("");
        Sync_Settings_Now = 10;
        Sync_Settings();
        Sync_Settings_Now = 15;
        Sync_Settings();
        }
    Serial.println(F(""));
    }
 }


// Update App Settings Sync 2
BLYNK_WRITE(V37)    {                     
  switch (param.asInt())
  {
    case 1: {    // Update App Sync 2
        Blynk.virtualWrite(V37, 2);
        Serial.println(F(""));
        Sync_App_Settings_2();
        }
    Serial.println(F(""));
    Blynk.virtualWrite(V37, 0);
    }
 }

void Sync_App_Settings_1() {
        // Transmit get ready code
        Serial.println("");
        transmit_blynk_code = 36;       //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Serial.println("");
        // Tell the MEGA to break the current loop of commands and 
        // be ready to send the button status update
        Serial.println("Sync 1: Waiting for to be MEGA Ready for TX");    
        Serial.println("");
        delay(3000);   
        // Transmit code to tell the MEGA to now transmit the buttons status
        transmit_blynk_code = 45;
        Transmit_Blynk_Data_to_Mega();
        delay(2500);
        Recieve_App_Button_Status_1();  
        }



void Sync_App_Settings_2() {
        // Transmit get ready code
        Serial.println("");
        transmit_blynk_code = 53;       //+10 added to the code TX
        Transmit_Blynk_Data_to_Mega();
        Serial.println("");
        // Tell the MEGA to break the current loop of commands and 
        // be ready to send the button status update
        Serial.println("Waiting for to be MEGA Ready for TX");    
        Serial.println("");
        delay(3000);    
        // Transmit code to tell the MEGA to now transmit the buttons status
        transmit_blynk_code = 54;
        Transmit_Blynk_Data_to_Mega();
        delay(2500);
        Recieve_App_Button_Status_2();
        }
