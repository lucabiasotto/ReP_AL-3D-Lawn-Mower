


void Sync_Settings() {
  
  if ((Mower_Docked_Filter == 2) && (Mower_Running != 1)) {
      
      Sync_Settings_Now ++;
      Serial.print("|Setup Sync:");
      Serial.print(Sync_Settings_Now);
      
      if (Sync_Settings_Now == 20) {
        Blynk.virtualWrite(V23, 2);
        Sync_App_Settings_1();
        Blynk.virtualWrite(V23, 0);
        }
      
      if (Sync_Settings_Now == 25) {
        Blynk.virtualWrite(V23, 2);
        Sync_App_Settings_2();
        Blynk.virtualWrite(V23, 1);


      }

      if (Sync_Settings_Now > 40) Blynk.virtualWrite(V23, 0);
      if (Sync_Settings_Now > 250) Sync_Settings_Now = 0;
      }
  
  }


void Set_PCB_Wire_Status() {

    if (Auto_Wire == 1) {
      
      Serial.print("|Wire Status:");
      if (Wire_Status_ON_OFF == 0) Serial.print("OFF");
      if (Wire_Status_ON_OFF == 1) Serial.print("ON");
      
      
    if ((Mower_Docked_Filter == 2) || (Charge_Detected == 1))   {
     Serial.print("|Docked Counts Wire:");
     Serial.print(Docked_Counts);

      // 20 continuous docked status feeds required to turn off wire
      if ( Docked_Counts >= 20) {
        Serial.print("|Wire OFF due to docked");
        Wire_OFF();
        }

    Docked_Counts++;
    if (Docked_Counts > 20) Docked_Counts = 0;
      
    }
    
    if ((Mower_Running_Filter == 2) || (Tracking_Wire == 1)) {
      Serial.println("Wire ON due to running");
      Wire_ON();
      Docked_Counts = 0;
      }

  }

}




void Wire_OFF() {
  if (Auto_Wire == 1) {
    PCBWire_WEMOSD1.virtualWrite(V1, 2, 1000);
    Wire_Status_ON_OFF = 0;
    }

}


void Wire_ON() {
  if (Auto_Wire == 1) {
    PCBWire_WEMOSD1.virtualWrite(V1, 1, 1000);
    Wire_Status_ON_OFF = 1;
    }
  
}
