void Prepare_Mower_from_Settings() {
  if (Use_Charging_Station == 1) {
    Mower_Docked  = 1;
    Mower_Parked  = 0;
    Mower_Running = 0;
  }

  if (Use_Charging_Station == 0) {
    Mower_Docked  = 0;
    Mower_Parked  = 1;
    Mower_Running = 0;
  }
}

void Setup_Compass() {
  if (Compass_Activate == 1) {
    lcd.clear();
    lcd.print("Compass  ");
    lcd.setCursor(0, 1);
    lcd.print("Setup");
    Serial.println("Setup Compass");
    while (!compass.begin())
    {
      Serial.println(F("Could not find a valid QMC5883 sensor, check wiring!"));
      delay(500);
    }

    if (compass.isHMC()) {
      Serial.println(F("Initialize HMC5883"));
      compass.setRange(HMC5883L_RANGE_1_3GA);
      compass.setMeasurementMode(HMC5883L_CONTINOUS);
      compass.setDataRate(HMC5883L_DATARATE_15HZ);
      compass.setSamples(HMC5883L_SAMPLES_8);
    }
    else if (compass.isQMC()) {
      Serial.println(F("Initialize QMC5883"));
      compass.setRange(QMC5883_RANGE_2GA);
      compass.setMeasurementMode(QMC5883_CONTINOUS);
      compass.setDataRate(QMC5883_DATARATE_50HZ);
      compass.setSamples(QMC5883_SAMPLES_8);
    }
    lcd.clear();
    lcd.print("Compass Setup ");
    lcd.setCursor(0, 1);
    lcd.print("Done!             ");
    delay(500);
    lcd.clear();
  }

  if (Compass_Activate == 0) {
    Serial.println(F("Compass Switched off - Select 1 in setup to switch on."));
  }
}


void Setup_Relays() {
  Serial.println("Setup Relays");
  pinMode(Relay_Motors, OUTPUT);
  delay(5);
  Turn_Off_Relay();
  delay(5);
}

void Setup_Motor_Pins() {
  Serial.println("Setup Motor Pins");
  pinMode(L_EN, OUTPUT);
  pinMode(R_EN, OUTPUT);
  //tolto perchè uso analogWrite(RPWM, 255); pinMode(RPWM, OUTPUT);


  /*Sets up the wheel motor Pins*/
  pinMode(ENAPin, OUTPUT);
  pinMode(ENBPin, OUTPUT);
  pinMode(IN1Pin, OUTPUT);
  pinMode(IN2Pin, OUTPUT);
  pinMode(IN3Pin, OUTPUT);
  pinMode(IN4Pin, OUTPUT);

}

void Turn_On_Relay() {
  Serial.print("Relay:ON|");
  digitalWrite(Relay_Motors, LOW);// Turn of the relay for the main battery power
}

void Turn_Off_Relay() {
  Serial.print("Relay:Off|");
  digitalWrite(Relay_Motors, HIGH);                         // Turn of the relay for the main battery power
}

void Setup_Membrane_Buttons() {
  Serial.println("Setup Membrane Keys");
  pinMode(Start_Key, INPUT_PULLUP);            // set pin as input
  pinMode(Plus_Key, INPUT_PULLUP);            // set pin as input
  pinMode(Minus_Key, INPUT_PULLUP);            // set pin as input
  pinMode(Stop_Key, INPUT_PULLUP);            // set pin as input
}

void Setup_ADCMan() {
  Serial.println("ADCMAN");
  ADCMan.init();
  perimeter.setPins(pinPerimeterLeft, pinPerimeterRight);
  perimeter.useDifferentialPerimeterSignal = true;
  perimeter.speedTest();
  ADCMan.setCapture(A2, 1, false);
  ADCMan.run();
}
