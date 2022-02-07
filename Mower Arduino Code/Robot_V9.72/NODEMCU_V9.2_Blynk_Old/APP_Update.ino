

void Update_Blynk_App_With_Status() {

         if (Mower_Docked_Filter == 2)   {
           //App Buttons
           Blynk.virtualWrite(V10,LOW);       // Exit Dock Button 
           Blynk.virtualWrite(V0, LOW);       // Quick Start Button
           Blynk.virtualWrite(V2, LOW);       // Pause/Stop Button
           Blynk.virtualWrite(V1, LOW);       // Go To Dock Button
           }
         
        if (Mower_Running_Filter == 2)   {
           //App Buttons
           Blynk.virtualWrite(V10,LOW);       // Exit Dock Button 
           Blynk.virtualWrite(V0, LOW);       // Quick Start Button
           Blynk.virtualWrite(V2, LOW);       // Pause/Stop Button
           Blynk.virtualWrite(V1, LOW);       // Go To Dock Button
           Wire_ON();
           }

        if (Manuel_Mode == 1)   {
           //App Buttons
           Blynk.virtualWrite(V10,LOW);       // Exit Dock Button 
           Blynk.virtualWrite(V0, LOW);       // Quick Start Button
           Blynk.virtualWrite(V2, LOW);       // Pause/Stop Button
           Blynk.virtualWrite(V1, LOW);       // Go To Dock Button
           
           Blynk.virtualWrite(V6, 0);
           Blynk.virtualWrite(V7, 0);
           Blynk.virtualWrite(V9, 0);     
           Blynk.virtualWrite(V8, 0);
           Wire_ON();
           }

        if (Mower_Parked == 1)   {
           //App Buttons
           Blynk.virtualWrite(V10,LOW);       // Exit Dock Button 
           Blynk.virtualWrite(V0, LOW);       // Quick Start Button
           Blynk.virtualWrite(V2, LOW);       // Pause/Stop Button
           Blynk.virtualWrite(V1, LOW);       // Go To Dock Button
           }

        if (Compass_Activate == 1) Blynk.virtualWrite(V20, 1);  
        if (Compass_Activate == 0) Blynk.virtualWrite(V20, 2);
          
        if (Compass_Heading_Hold_Enabled == 1) Blynk.virtualWrite(V21, 1);  
        if (Compass_Heading_Hold_Enabled == 0) Blynk.virtualWrite(V21, 2);
          
        if (GYRO_Enabled == 1) Blynk.virtualWrite(V22, 1);  
        if (GYRO_Enabled == 0) Blynk.virtualWrite(V22, 2);

        if (Sonar_1_Activate == 1) Blynk.virtualWrite(V24, 1);  
        if (Sonar_1_Activate == 0) Blynk.virtualWrite(V24, 2);
        if (Sonar_2_Activate == 1) Blynk.virtualWrite(V25, 1);  
        if (Sonar_2_Activate == 0) Blynk.virtualWrite(V25, 2);
        if (Sonar_3_Activate == 1) Blynk.virtualWrite(V26, 1);  
        if (Sonar_3_Activate == 0) Blynk.virtualWrite(V26, 2); 
        
        if (Bumper_Activate_Frnt == 1) Blynk.virtualWrite(V29, 1);  
        if (Bumper_Activate_Frnt == 0) Blynk.virtualWrite(V29, 2); 

        if (Alarm_1_ON == 1) Blynk.virtualWrite(V30, 1);  
        if (Alarm_1_ON == 0) Blynk.virtualWrite(V30, 2);
        if (Alarm_2_ON == 1) Blynk.virtualWrite(V31, 1);  
        if (Alarm_2_ON == 0) Blynk.virtualWrite(V31, 2);
        if (Alarm_3_ON == 1) Blynk.virtualWrite(V32, 1);  
        if (Alarm_3_ON == 0) Blynk.virtualWrite(V32, 2);
        if (GPS_Enabled == 1) Blynk.virtualWrite(V33, 1);  
        if (GPS_Enabled == 0) Blynk.virtualWrite(V33, 2);
        if (Angle_Sensor_Enabled == 1) Blynk.virtualWrite(V34, 1);  
        if (Angle_Sensor_Enabled == 0) Blynk.virtualWrite(V34, 2);
        if (Tip_Over_Sensor_Enabled == 1) Blynk.virtualWrite(V35, 1);  
        if (Tip_Over_Sensor_Enabled == 0) Blynk.virtualWrite(V35, 2);
        if (Wheel_Amp_Sensor_ON == 1) Blynk.virtualWrite(V36, 1);  
        if (Wheel_Amp_Sensor_ON == 0) Blynk.virtualWrite(V36, 2);


        if (Charge_Detected == 4)          Blynk.virtualWrite(V12, 1023);     // Charging LED ON
        if (Charge_Detected == 0)          Blynk.virtualWrite(V12, 0);        // Charging LED OFF
        if (Tracking_Wire == 1)            Blynk.virtualWrite(V9, 1023);      // Tracking LED ON
        if (Tracking_Wire == 0)            Blynk.virtualWrite(V9, 0);         // Tracking LED OFF
        if (Mower_Parked == 1)             Blynk.virtualWrite(V6, 1023);      // Parked LED
        if (Mower_Parked == 0)             Blynk.virtualWrite(V6, 0);         // Parked LED
        if (Mower_Running_Filter == 2)     Blynk.virtualWrite(V7, 1023);      // Mowing LED
        if (Mower_Running_Filter != 2)     Blynk.virtualWrite(V7, 0);         // Mowing LED
        if (Mower_Docked_Filter == 2)      Blynk.virtualWrite(V8, 1023);      // Docked LED
        if (Mower_Docked_Filter != 2)      Blynk.virtualWrite(V8, 0);         // Docked LED
        }





void Clear_APP() {
  Blynk.virtualWrite(V0, LOW);   // Start Mowing Buton OFF
  Blynk.virtualWrite(V1, LOW);  // Go-Home Button OFF
  Blynk.virtualWrite(V2, LOW);  // Stop Button OFF
  Blynk.virtualWrite(V3, 0);    // Reset Voltage
  Blynk.virtualWrite(V5, 0);    // Loops
  Blynk.virtualWrite(V7, 0);    // Mow LED
  Blynk.virtualWrite(V8, 0);    // Dock LED Off
  Blynk.virtualWrite(V9, 0);    // TrackingLED OFF
  Blynk.virtualWrite(V10, LOW); // Dock Button OFF
  Blynk.virtualWrite(V11, 0);   // Compass LED OFF  
  Blynk.virtualWrite(V12, 0);   // Charging LED OFF  
  Blynk.virtualWrite(V12, 0);   // Charging LED OFF  
}
