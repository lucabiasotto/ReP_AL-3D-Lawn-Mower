void Load_EEPROM_Values() {
      
      Serial.println(F("Loading EEPROM Values"));
      
      
      int Draw_Pictures_EEPROM = EEPROM.read(2);
      if (Draw_Pictures_EEPROM == 1) {
          Draw_Pictures =  EEPROM.read(3);
          if (Robot_Type == 1) Serial.println(F("EEPROM: Pictures = ON"));
          if (Robot_Type == 0) Serial.println(F("EEPROM: Pictures = OFF"));     
          }

       int Robot_Type_EEPROM = EEPROM.read(4);
       if (Robot_Type_EEPROM == 1) {
          Robot_Type = EEPROM.read(5);       
          if (Robot_Type == 1) Serial.println(F("EEPROM: Robot = Mower"));
          if (Robot_Type == 2) Serial.println(F("EEPROM: Robot = Aerator"));      
          }
        if (Robot_Type_EEPROM  != 1) {
          if (Robot_Type == 1) Serial.println(F("Settings: Robot = Mower"));
          if (Robot_Type == 2) Serial.println(F("Settings: Robot = Aerator"));      
          }

  Serial.println(F("EEPROM Done."));
  }


void Clear_EEPROM_Setings() {
      EEPROM.write(2,0);
      EEPROM.write(4,0);
}
