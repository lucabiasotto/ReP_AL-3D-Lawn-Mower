/**
 * Read volt and amp from sensors
 */
void readVoltAmp() {
    //RawValueAmp = analogRead(A1);
    RawValueVolt = ADCMan.read(A2) * 4;  //analogRead(A2); TODO, perchè 4?
    Serial.println(RawValueVolt);        //todo debug log

    if (Show_TX_Data == 1) {
        Serial.print("Amp:");
        Serial.print(RawValueAmp);
        Serial.print("|");
        Serial.print("Volt:");
        Serial.print(RawValueVolt);
        Serial.print("|");
        Serial.print("Rain:");
        Serial.print(Rain_Detected);
        Serial.print("|");
    }

    // Calculate Amps from NANO RX Data
    int mVperAmp = 185;
    int ACSoffset = 2500;
    double VoltageAmp = 0;
    double Amps = 0;
    VoltageAmp = (RawValueAmp / 1024.0) * 5000;  // Gets you mV
    Amps = ((VoltageAmp - ACSoffset) / mVperAmp);

    Serial.print(F("A:"));
    Serial.print(Amps);
    Serial.print(F("|"));

    // Calculate Voltage from NANO RX Data

    if (RawValueVolt > 100) {
        float vout = 0.0;
        float R1 = 30000;                      // 30000 Mower 2    Mower 1 30000
        float R2 = 7000;                       // 7300 Mower 2     Mower 1 7500
        vout = (RawValueVolt * 5.0) / 1024.0;  // see text
        Volts = vout / (R2 / (R1 + R2));
        Volts_Last = Volts;
        Zero_Volts = 0;
    } else {
        Volts = Volts_Last;
        Zero_Volts = Zero_Volts + 1;
        if (Zero_Volts > 5) {
            Volts = 0;
        }
    }

    Serial.print(F("V:"));
    Serial.print(Volts);
    Serial.print(F("|"));

    if (Amps < 0.4) {
        Charge_Detected_MEGA = 0;
    }
    if (Amps > 0.4) {
        Charge_Detected_MEGA = 1;
    }
}



void Process_Volt_Information() {
    //  Logic for how the battery readings should be handled

    if (Volts < Battery_Min) {
        //potrebbe essere un momento sottosforzo, lo mando in carica  solo dopo che ho rilevato un po' di sottosforzo

        //fermo un attimo
        //Motor_Action_Stop_Motors();
        //delay(2000);

        Low_Battery_Detected = (Low_Battery_Detected + 1);
        Serial.print("VLow:");
        Serial.print(Low_Battery_Detected);
        Serial.print("|");
        if (Low_Battery_Detected > Low_Battery_Instances_Chg) {
            Serial.println("Low Battery Detected");

            if (Use_Charging_Station == 1) {
                // Stops the mowing and sends the mower back to the charging station via the permieter wire
                goToChargingStation();
            } else {
                // Parks the mower with a low battery warning
                Manouver_Park_The_Mower_Low_Batt();
            }
        }

    } else {
        //la batteria è sana
        Serial.print("Batteria ok");
        //Serial.print(Low_Battery_Detected);
        //Serial.print("|");
        Low_Battery_Detected = 0;
    }
}

void Check_if_Docked() {
    if (Charge_Detected_MEGA == 1) {  // if Amps are between this there is a charge detected.  Amps above 4 are discounted as a miscommunication
        Motor_Action_Stop_Motors();
        Serial.println(F("Charging Current detected"));
        Serial.println(F("Mower Docked"));
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(TRS_DOCKED_IN);
        lcd.setCursor(0, 1);
        lcd.print(TRS_CHARGING_STATION);  // Prints info to LCD display
        delay(2000);
        Mower_Docked = 1;
        manouverDockTheMower();  // Shuts down the Mower ready for charging and mowing again.
        lcd.clear();
    }
    Serial.println("");
}
