void Execute_Fake_It() {
      
      Serial.print("Fake:ON|");
      Volts = 12.6;
      Amps = 0.2;
      Outside_Wire = 0;                                               // Outside wire variable is tuend off
      Outside_Wire_Count = 0;                                         // The number of outside wire counts is reset to 0
      Wire_Refind_Tries = 0;  
      MAG_Now = -1500;
      Wire_Off = 0;
      Wire_Detected = 1;
      Rain_Sensor_Installed = 0;

      Fake_Loops = Fake_Loops++;
      if (Fake_Loops  > 50) {
        Volts = Battery_Min - 0.2 ;
      }
      }
