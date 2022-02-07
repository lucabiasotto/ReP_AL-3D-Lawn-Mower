// Send the various WIFI codes to the MEGA


void Pause_Mower() {
  Serial.println("");
  Serial.println("Pause/Stop");
  transmit_blynk_code = 1;
  Transmit_Blynk_Data_to_Mega();
  }

void Going_Home() {
  Serial.println("");
  Serial.println("Go to Dock");
  transmit_blynk_code = 2;
  Transmit_Blynk_Data_to_Mega();
  }

void StartMower() {
  Serial.println("");
  Serial.println("Quick Start");
  Mower_Parked = 0;
  Wire_ON;                                                            // Turn on Wire
  Update_Blynk_App_With_Status();
  transmit_blynk_code = 3;                                            // Code used to let MEGA know what to do
  Transmit_Blynk_Data_to_Mega();                                      // Transmits that code to the MEGA
  Docked_Counts = 0;
  Mower_Docked_Filter = 0;
  Sync_Settings_Now = 0;
  }

void Exit_Dock() {
  Serial.println("");
  Serial.println("Exit Dock");
  Wire_ON();                                                         // Switch on PCB Wire via NODEMCU 
  Mower_Docked = 0;                                                  // Forces the APP to show docked as false.
  Tracking_Wire = 1;                                                 // Otherwise the APP needs to wait for the next update round
  Update_Blynk_App_With_Status();
  Docked_Counts = 0;
  Mower_Docked_Filter = 0;
  transmit_blynk_code = 4;
  Transmit_Blynk_Data_to_Mega();
  Sync_Settings_Now = 0;
  }


void Set_To_Manuel_Mode() {
    Serial.println("");
    Serial.println("Manuel Mode Selected");
    Update_Blynk_App_With_Status();
    Mower_Docked = 0;
    Docked_Counts = 0;
    transmit_blynk_code = 5;
    Transmit_Blynk_Data_to_Mega();
    Manuel_Mode = 1;
    }

void Set_To_Automatic_Mode_Random() {
    Serial.println("");
    Serial.println("Automatic Mode RANDOM Selected");
    Update_Blynk_App_With_Status();
    transmit_blynk_code = 6;
    Transmit_Blynk_Data_to_Mega();
    Automatic_Mode = 1;
    }

void Set_To_Automatic_Mode_Spiral() {
    Serial.println("");
    Serial.println("Automatic Mode SPIRAL Selected");
    Update_Blynk_App_With_Status();
    transmit_blynk_code = 11;
    Transmit_Blynk_Data_to_Mega();
    Automatic_Mode = 2;
    }
 
void Set_To_Automatic_Mode_Parallel() {
    Serial.println("");
    Serial.println("Automatic Mode PARALLEL Selected");
    Update_Blynk_App_With_Status();
    transmit_blynk_code = 12;
    Transmit_Blynk_Data_to_Mega();
    Automatic_Mode = 2;
    }



    


  
