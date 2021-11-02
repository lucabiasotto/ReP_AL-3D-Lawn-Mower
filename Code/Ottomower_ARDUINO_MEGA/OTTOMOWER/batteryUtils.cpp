#include "batteryUtils.h"
#include "adcman.h"
#include "motorsController.h"
#include "movementsUtils.h"
#include "robot.h"

void readVoltAmp() {
    //robot.rawValueAmp = analogRead(A1);
    robot.rawValueVolt = ADCMan.read(A2) * 4;  //analogRead(A2); TODO, perchè 4?
    Serial.println(robot.rawValueVolt);        //todo debug log

    if (LOG_BATTERY_DATA == 1) {
        Serial.print("Amp:");
        Serial.print(robot.rawValueAmp);
        Serial.print("|");
        Serial.print("Volt:");
        Serial.print(robot.rawValueVolt);
        Serial.print("|");
        Serial.print("Rain:");
        Serial.print(robot.rainDetected);
        Serial.print("|");
    }

    // Calculate robot.amps from NANO RX Data
    int mVperAmp = 185;
    int ACSoffset = 2500;
    //TODO double robot.voltageAmp = 0;
    //TODO double robot.amps = 0;
    robot.voltageAmp = (robot.rawValueAmp / 1024.0) * 5000;  // Gets you mV
    robot.amps = ((robot.voltageAmp - ACSoffset) / mVperAmp);

    Serial.print(F("A:"));
    Serial.print(robot.amps);
    Serial.print(F("|"));

    // Calculate Voltage from NANO RX Data

    if (robot.rawValueVolt > 100) {
        float vout = 0.0;
        float R1 = 30000;                            // 30000 Mower 2    Mower 1 30000
        float R2 = 7000;                             // 7300 Mower 2     Mower 1 7500
        vout = (robot.rawValueVolt * 5.0) / 1024.0;  // see text
        robot.volts = vout / (R2 / (R1 + R2));
        robot.voltsLast = robot.volts;
        robot.zeroVolts = 0;
    } else {
        robot.volts = robot.voltsLast;
        robot.zeroVolts = robot.zeroVolts + 1;
        if (robot.zeroVolts > 5) {
            robot.volts = 0;
        }
    }

    Serial.print(F("V:"));
    Serial.print(robot.volts);
    Serial.print(F("|"));

    if (robot.amps < 0.4) {
        robot.chargeDetectedMEGA = 0;
    }
    if (robot.amps > 0.4) {
        robot.chargeDetectedMEGA = 1;
    }
}

void Process_Volt_Information() {
    //  Logic for how the battery readings should be handled

    if (robot.volts < BATTERY_MIN) {
        //potrebbe essere un momento sottosforzo, lo mando in carica  solo dopo che ho rilevato un po' di sottosforzo

        //fermo un attimo
        //Motor_Action_Stop_Motors();
        //delay(2000);

        robot.lowBatteryDetectedCount = (robot.lowBatteryDetectedCount + 1);

    } else {
        //la batteria è sana
        robot.lowBatteryDetectedCount = 0;
    }
}

// checks to see if the mower is on the charging station
void Check_if_Charging() {
    //TODO sistema sto if
    if (robot.charging == 4) {  // If the value recieved is equal to 1 or 0 as expected then print the value to the serial monitor
        robot.chargeDetectedMEGA = 1;
    }else{
         robot.chargeDetectedMEGA = 0;// If the value recieved is equal to 1 or 0 as expected then print the value to the serial monitor
    }
}

void Check_if_Docked() { //TODO rename
    if (robot.chargeDetectedMEGA == 1) {  // if robot.amps are between this there is a charge detected.  robot.amps above 4 are discounted as a miscommunication
        Motor_Action_Stop_Motors();
        Serial.println(F("Charging Current detected"));
        Serial.println(F("Mower Docked"));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(TRS_DOCKED_IN);
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print(TRS_CHARGING_STATION);  // Prints info to LCD display
        delay(2000);
        manouverDockTheMower();  // Shuts down the Mower ready for charging and mowing again.
        robot.lcdDisplay.clear();
    }
    Serial.println("");
}

void Calculate_Volt_Amp_Charge() {
    if (LOG_BATTERY_DATA == 1) {
        Serial.print("Amp:");
        Serial.print(robot.rawValueAmp);
        Serial.print("|");
        Serial.print("Volt:");
        Serial.print(robot.rawValueVolt);
        Serial.print("|");
        Serial.print("Rain:");
        Serial.print(robot.rainDetected);
        Serial.print("|");
    }

    // Calculate Amps from NANO RX Data
    int mVperAmp = 185;
    int ACSoffset = 2500;
    double VoltageAmp = 0;
    double Amps = 0;
    VoltageAmp = (robot.rawValueAmp / 1024.0) * 5000;  // Gets you mV
    Amps = ((VoltageAmp - ACSoffset) / mVperAmp);

    Serial.print(F("A:"));
    Serial.print(Amps);
    Serial.print(F("|"));

    // Calculate Voltage from NANO RX Data

    if (robot.rawValueVolt > 100) {
        float vout = 0.0;
        float R1 = 30000;                      // 30000 Mower 2    Mower 1 30000
        float R2 = 7000;                       // 7300 Mower 2     Mower 1 7500
        vout = (robot.rawValueVolt * 5.0) / 1024.0;  // see text
        robot.volts = vout / (R2 / (R1 + R2));
        robot.voltsLast = robot.volts;
        robot.zeroVolts = 0;
    }else {
        robot.volts = robot.voltsLast;
        robot.zeroVolts = robot.zeroVolts + 1;
        if (robot.zeroVolts > 5) robot.volts = 0;
    }

    Serial.print(F("V:"));
    Serial.print(robot.volts);
    Serial.print(F("|"));

    if (Amps < 0.4) robot.charging = 0;
    if (Amps > 0.4) robot.charging = 4;
    //Serial.print("Charging = ");
    //Serial.print(Charging);
    //Serial.print("|");
}