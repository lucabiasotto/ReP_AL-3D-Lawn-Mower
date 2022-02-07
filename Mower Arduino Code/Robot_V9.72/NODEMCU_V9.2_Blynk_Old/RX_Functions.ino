// All the recieved information functions

void Receive_All_From_MEGA () {

  String NodeMCU_RX_Value  = "";                                              // changed to string

  while (NodeMCU.available() > 0) {
    
    char recieved = NodeMCU.read();
    if ( recieved != '\g'  && recieved != '\c' && recieved != '\d' && recieved != '\z' && recieved != '\y' && recieved != '\o' && recieved != '\m' ) {   
      NodeMCU_RX_Value = NodeMCU_RX_Value +  (char)recieved;          // hack to joining chars in correct way to Serial1_Rx_Value
      } 

      else if (recieved == '\g') {
      val_VoltNow = NodeMCU_RX_Value.toFloat();                                 // if end of value found, set AmpsTX and clear Serial1_Rx_Value temp var Serial1_Rx_Value used for holding value until \q or \j
      NodeMCU_RX_Value = ""; // changed to string
      } 
      else if (recieved == '\c') {
      Loop_Cycle_Mowing = NodeMCU_RX_Value.toInt();                                // updates loop cycle 
      NodeMCU_RX_Value = "";
      } 
      else if (recieved == '\d') {
      Mower_Docked = NodeMCU_RX_Value.toInt();                               // updates if mower is docked
      NodeMCU_RX_Value = "";
      } 
      else if (recieved == '\z') {
      Mower_Running = NodeMCU_RX_Value.toInt();                               // updates is mower is running
      NodeMCU_RX_Value = "";
      } 
      else if (recieved == '\y') {
      Mower_Parked = NodeMCU_RX_Value.toInt();                               // updates if mower is parked
      NodeMCU_RX_Value = "";
      } 
      else if (recieved == '\o') {
      Charge_Detected = NodeMCU_RX_Value.toInt();                               // updates if mower is charging
      NodeMCU_RX_Value = "";
      } 
      else if (recieved == '\m') {
      Tracking_Wire = NodeMCU_RX_Value.toInt();                               // updates if tracking
      NodeMCU_RX_Value = "";
      } 
    else Serial.print("No Data Received|");
  }
        BatteryVoltage = val_VoltNow;
        Serial.print("Volt:");
        Serial.print(BatteryVoltage);
        Serial.print("|");

        Serial.print("Loop:");
        Serial.print(Loop_Cycle_Mowing);  
        Serial.print("|");  

        Serial.print("Docked:");
        Serial.print(Mower_Docked);
        Serial.print("|");  
        
        Serial.print("Running:");
        Serial.print(Mower_Running);
        Serial.print("|");  

        Serial.print("Parked:");
        Serial.print(Mower_Parked);  
        Serial.print("|");

        Serial.print("Charge:");
        Serial.print(Charge_Detected);  
        Serial.print("|");

        Serial.print("Tracking:");
        Serial.print(Tracking_Wire);  
        Serial.print("|");
  
  Calculate_Filtered_Mower_Status();
}


void Calculate_Filtered_Mower_Status() {

    if (Mower_Docked == 1) {
      Mower_Docked_Filter = Mower_Docked_Filter + 1;
      if (Mower_Docked_Filter == 3) Mower_Docked_Filter = 2;
      if (Auto_Wire == 1) PCBWire_WEMOSD1.virtualWrite(V1, 2, 1000);
    }
    else Mower_Docked_Filter = 0;
    
    if (Mower_Running == 1) {
      Mower_Running_Filter = Mower_Running_Filter + 1;
      if (Mower_Running_Filter == 3) Mower_Running_Filter = 2;
      if (Auto_Wire == 1) PCBWire_WEMOSD1.virtualWrite(V1, 1, 1000);
    }
    else Mower_Running_Filter = 0;
    
}




void Recieve_App_Button_Status_1 () {

  String NodeMCU_RX_Value  = "";                                              // changed to string

  while (NodeMCU.available() > 0) {
    
    char recieved = NodeMCU.read();
    if ( recieved != '\a'  && recieved != '\b' && recieved != '\c' &&
    recieved != '\d'  && recieved != '\e' && recieved != '\f' && recieved != '\g' ) {   
      NodeMCU_RX_Value = NodeMCU_RX_Value +  (char)recieved;          // hack to joining chars in correct way to Serial1_Rx_Value
      } 

      else if (recieved == '\a') {
      Compass_Activate = NodeMCU_RX_Value.toInt();                                 // if end of value found, set AmpsTX and clear Serial1_Rx_Value temp var Serial1_Rx_Value used for holding value until \q or \j
      NodeMCU_RX_Value = ""; // changed to string
      } 
      else if (recieved == '\b') {
      Compass_Heading_Hold_Enabled = NodeMCU_RX_Value.toInt();                                // same as upper but for VoltsTX, 
      NodeMCU_RX_Value = "";
      } 
      else if (recieved == '\c') {
      GYRO_Enabled = NodeMCU_RX_Value.toInt();                               // same as upper but for VoltsTX
      NodeMCU_RX_Value = "";
      }
      else if (recieved == '\d') {
      Sonar_1_Activate = NodeMCU_RX_Value.toInt();                               // same as upper but for VoltsTX
      NodeMCU_RX_Value = "";
      }
      else if (recieved == '\e') {
      Sonar_2_Activate = NodeMCU_RX_Value.toInt();                               // same as upper but for VoltsTX
      NodeMCU_RX_Value = "";
      }
      else if (recieved == '\f') {
      Sonar_3_Activate = NodeMCU_RX_Value.toInt();                               // same as upper but for VoltsTX
      NodeMCU_RX_Value = "";
      }
      else if (recieved == '\g') {
      Bumper_Activate_Frnt = NodeMCU_RX_Value.toInt();                               // same as upper but for VoltsTX
      NodeMCU_RX_Value = "";
      }
    
    
    else Serial.print("No Data Received|");
  }
        Serial.println("");
        Serial.print("Compass_Activate :");
        Serial.println(Compass_Activate);
        Serial.print("Heading Hold TX :");
        Serial.println(Compass_Heading_Hold_Enabled);  
        Serial.print("GYRO TX :");
        Serial.println(GYRO_Enabled);
        Serial.print("Sonar 1 ON :");
        Serial.println(Sonar_1_Activate);
        Serial.print("Sonar 2 ON :");
        Serial.println(Sonar_2_Activate);
        Serial.print("Sonar 3 ON :");
        Serial.println(Sonar_3_Activate);
        Serial.print("Bumper ON:");
        Serial.println(Bumper_Activate_Frnt);
        Serial.println(""); 

}






// Second Group of APP Settings Received from Mower
void Recieve_App_Button_Status_2 () {

  String NodeMCU_RX_Value  = "";                                              // changed to string

  while (NodeMCU.available() > 0) {
    
    char recieved = NodeMCU.read();
    if ( recieved != '\a'  && recieved != '\b' && recieved != '\c' &&
    recieved != '\d'  && recieved != '\e' && recieved != '\f' && recieved != '\g' ) {   
      NodeMCU_RX_Value = NodeMCU_RX_Value +  (char)recieved;          // hack to joining chars in correct way to Serial1_Rx_Value
      } 

      else if (recieved == '\a') {
      Alarm_1_ON = NodeMCU_RX_Value.toInt();                                 // if end of value found, set AmpsTX and clear Serial1_Rx_Value temp var Serial1_Rx_Value used for holding value until \q or \j
      NodeMCU_RX_Value = ""; // changed to string
      } 
      else if (recieved == '\b') {
      Alarm_2_ON  = NodeMCU_RX_Value.toInt();                                // same as upper but for VoltsTX, 
      NodeMCU_RX_Value = "";
      } 
      else if (recieved == '\c') {
      Alarm_3_ON  = NodeMCU_RX_Value.toInt();                               // same as upper but for VoltsTX
      NodeMCU_RX_Value = "";
      }
      else if (recieved == '\d') {
      GPS_Enabled = NodeMCU_RX_Value.toInt();                               // same as upper but for VoltsTX
      NodeMCU_RX_Value = "";
      }
      else if (recieved == '\e') {
      Angle_Sensor_Enabled  = NodeMCU_RX_Value.toInt();                               // same as upper but for VoltsTX
      NodeMCU_RX_Value = "";
      }
      else if (recieved == '\f') {
      Tip_Over_Sensor_Enabled = NodeMCU_RX_Value.toInt();                               // same as upper but for VoltsTX
      NodeMCU_RX_Value = "";
      }
      else if (recieved == '\g') {
      Wheel_Amp_Sensor_ON = NodeMCU_RX_Value.toInt();                               // same as upper but for VoltsTX
      NodeMCU_RX_Value = "";
      }
    
    
    else Serial.print("No Data Received|");
  }
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
