
// Check the mower is inside (0) or outside (1) the perimeter wire
void Check_Wire_In_Out() {

  // If the perimeter wire sensor is de-activated
  if (Perimeter_Wire_Enabled == 0) {
      if (TFT_Screen_Menu == 1) {
        Outside_Wire = 0;
        //Data_Sent_Wire = 0;                                           // Resets the counter to send info to the TFT
        //Send_Mower_Running_Data();
        }    
      }


  // If the perimeter wire sensor is activated
  if (Perimeter_Wire_Enabled == 1) {
    UpdateWireSensor();                                               // Read the wire sensor and see of the mower is now  or outside the wire  
    ADCMan.run();
    PrintBoundaryWireStatus();        
    
    // OUTSIDE the wire
    if ( (perimeter.isInside(0)) == 0 )  {                            // Mower is OUTSIDE the wire
      Outside_Wire = 1;                                               // Outside wire variable is tuend on.
      if (Mower_Running == 1) Motor_Action_Stop_Motors();             // Stop immediatley the wheel motors
      Print_LCD_Wire();                                               // Update the LCD screem
      Outside_Wire_Count = Outside_Wire_Count + 1;                    // Count the number of times the mower is consecutiley outside the wire
                                                                      // If a certain number is reached its assumed thw mower is lost outside the wire.
      }
    
    // INSIDE the wire
    if ( (perimeter.isInside(0)) == 1 )   {
      Outside_Wire = 0;                                               // Outside wire variable is tuend off
      Outside_Wire_Count = 0;                                         // The number of outside wire counts is reset to 0
      Wire_Refind_Tries = 0;
    }

    if ( Fake_All_Settings == 1) Execute_Fake_It();    
      
     
    }

  // If the mower consectivley detects its outside the wire a number of times
  // its assumed the mower is lost and a this search function is started to try and 
  // re-find the wire.  Otherwise the mower is hibernated.
  if (Outside_Wire_Count >= Outside_Wire_Count_Max) {
    if  (Action_On_Over_Wire_Count_Max == 1) Manouver_Hibernate_Mower();                  // Put the mower to sleep and wait
    if  (Action_On_Over_Wire_Count_Max == 2) Manouver_Outside_Wire_ReFind_Function();     // re-find Garden using Sonar 1 and wire detect
    
    if  (Action_On_Over_Wire_Count_Max == 3) {     // try to locate the wire using wire find function
      
      #if defined(LCD_KEYPAD)
      if (LCD_Screen_Keypad_Menu == 1) {
          lcd.clear();
          lcd.print("Wire Find");
          lcd.setCursor(0,1);
          lcd.print("Special Function");
          }
      #endif
      
      Loop_Cycle_Mowing = 999;
      if ((WIFI_Enabled == 1) && (Manuel_Mode == 0)) Get_WIFI_Commands();                                   // TX and RX data from NodeMCU
      delay(2000);
      Outside_Wire_Count = 0;
      Specials_Find_Wire_Track();                  
      SetPins_ToGoBackwards();                                                              // Set the mower to back up
      Motor_Action_Go_Full_Speed(); 
      delay(1000);
      Motor_Action_Stop_Motors();  
      UpdateWireSensor();                                               // Read the wire sensor and see of the mower is now  or outside the wire  
      ADCMan.run();
      PrintBoundaryWireStatus(); 
      delay(1000);
      UpdateWireSensor();                                               // Read the wire sensor and see of the mower is now  or outside the wire  
      ADCMan.run();
      PrintBoundaryWireStatus(); 
      Wire_Refind_Tries = Wire_Refind_Tries + 1;
      Loop_Cycle_Mowing = 0;
      if (Manuel_Mode == 0) Get_WIFI_Commands();                                   // TX and RX data from NodeMCU
      Serial.println(F(""));
      Serial.print(F("|Wire Refind Atempts:"));
      Serial.print(Wire_Refind_Tries);
      Serial.print("|");
      Serial.println(F(""));
      if (Wire_Refind_Tries > 4) {
        Motor_Action_Stop_Motors(); 
        #if defined(LCD_KEYPAD) 
        lcd.clear(); 
        #endif
        Mower_Error = 1;
        Serial.println(F(""));
        Serial.println("Max refind tries exceeded - Parking the Mower");
        delay(2000);
      
      }
      
    }
  }
    
  }



//Check that boundary wire is turned on
//************************************************************************************
void Running_Test_for_Boundary_Wire()  {
  ADCMan.run();
  UpdateWireSensor();
  
  if (Perimeter_Wire_Enabled == 1) {                                               // Perimeter use is ON - Perimter_USE can be turned on or off in the setup.

    // Checks the MAG field of the boundary wire
    MAG_Now = perimeter.getMagnitude(0);

    // Only used for testing purposes
    if (Fake_All_Settings == 1) Execute_Fake_It();
    if (Fake_Wire == 1) MAG_Now = -500;
    Serial.print(F("|MAG:"));
    Serial.print(MAG_Now);



   
    // If the MAG field is very low between these values we can assume the wire is off
    if ( (MAG_Now > -20 ) && (MAG_Now < 20 )  ) {
      //Serial.print("WIRE Not Detected");
      Wire_Detected = 0;
      Print_LCD_NO_Wire();
      Wire_Off = Wire_Off + 1;
      }


    // if the MAG field is strong then the wire is on.
    if (   (MAG_Now <= -21) || (MAG_Now >= 20)  ) {
      Print_LCD_Wire_ON();  
      //Serial.print("WIRE Detected");
      Wire_Detected = 1;                                            // Wire is detected  
      Wire_Off = 0;                                                 // Resets the counter
      }
      
      // If the mower is docked or Parked then the TFT screen just shows a wire off warning
      if ( (Wire_Off > 5) && (Mower_Docked == 0) && (Mower_Parked == 0) ) {
        Serial.println(F("Wire Test Failed - Hibernating Mower"));
        Manouver_Hibernate_Mower();      // Send the mower to sleep        
        }
       

    Serial.print(F("|Wire"));
    Serial.print(F(":"));
    if (Wire_Detected == 0) Serial.print(F("OFF|"));
    if (Wire_Detected == 1) Serial.print(F("ON|"));
    }

  
  // If the wire is not enabled
  if (Perimeter_Wire_Enabled == 0) {   
    Serial.print(F("Wire"));
    Serial.print(F(":"));                                            
    Serial.print(F("DISABLED|"));
    Wire_Detected = 1;
    Wire_Off = 0;

  }
}


void UpdateWireSensor()   {
  if (millis() >= nextTime)  {
    nextTime = millis() + 50;
    if (perimeter.isInside(0) != inside) {
      inside = perimeter.isInside(0);
      counter++;
    }
  }

  if ( Fake_All_Settings == 1) {
    inside = 1;
  }
}


void PrintBoundaryWireStatus() {

  Serial.print(F("|IN/OUT:"));
  Serial.print((perimeter.isInside(0)));
  Serial.print(F("|Mag:"));
  Serial.print((int)perimeter.getMagnitude(0));
  Serial.print(F("|Smag:"));
  Serial.print((int)perimeter.getSmoothMagnitude(0));
  Serial.print(F("|Q:"));
  Serial.print((perimeter.getFilterQuality(0)));
  Serial.print(F("|"));
    
  }



void Run_Initial_Boundary_Wire_Test() {
    
    Wire_Off      = 0;     // reset the wire off count.
    Wire_Detected = 0;     // reset wire detected
    
    if (Perimeter_Wire_Enabled == 1) {
        Mower_Track_To_Exit = 1;
        Serial.println(F("Testing Boundary Wire #1"));
        Running_Test_for_Boundary_Wire();
        delay(50);    
        Serial.print(F("MAG Detected = "));
        Serial.print(MAG_Now);   
        Serial.print(F(" | Wire Detected = "));
        Serial.print(Wire_Detected);           
        Serial.println(F(""));
        
        // If the boundary wire is turned off / not detected
        // Then the test boundary wire function will already put the mower into hibernate mode.
        
        if (Wire_Detected == 0) {
            Serial.println("Perimeter Wire not detected - Testing Again");
            Serial.println(F("Testing Boundary Wire #2"));
            delay(2000);
            Wire_Detected = 1;
            Running_Test_for_Boundary_Wire();                                     // Test again for the boundary wire
            Serial.print(F("MAG Detected = "));
            Serial.println(MAG_Now);                
                            
            if (Wire_Detected == 0) {                                 // if its still saying the wire is off then park the mower.
              Serial.println("Testing Boundary Wire #3");
              Serial.print(F("Wait .... 2secs"));
              delay(2000);      
              Wire_Detected = 1;
              Running_Test_for_Boundary_Wire();
              Serial.print(F("MAG Detected = "));
              Serial.println(MAG_Now);
              } 
              if (Wire_Detected == 0) {                                 // if its still saying the wire is off then park the mower.
                Serial.println("Perimeter Wire not detected - Last Try.....");
                Serial.print(F("Wait .... 2secs"));
                delay(2000);      
                Wire_Detected = 1;
                Running_Test_for_Boundary_Wire();
                Serial.print(F("MAG Detected = "));
                Serial.println(MAG_Now);
                } 
                
            }
         }      
      

  if (Perimeter_Wire_Enabled == 0) {
     Wire_Detected = 1;
     Serial.print(F("Wire"));
     Serial.print(F(":"));                                            
     Serial.print(F("DISABLED|"));
     Wire_Detected = 1;
     Wire_Off = 0;
  
  }

}
