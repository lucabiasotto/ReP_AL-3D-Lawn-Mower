/**
 *  Print mower status on LCD screen
 */
#include "lcdDisplay.h"
#include "robot.h"


void lcdUpdateScreen() {
    /**********************
     * RIGA 1
     **********************/
    lcd.setCursor(0, 0);
    if (robot.mowerDocked == 1 && robot.chargeDetectedMEGA == 1) {
        lcd.print(TRS_CHARGING);  //is charging
    } else if (robot.rainDetected == 1) {
        lcd.print(TRS_RAIN);
    } else {
        lcd.print("        ");
        //stampa l'orario
        //TODO RTC
        /*
        Time t = rtc.time();
        lcd.print(t.hr);
        lcd.print(":");
        if (t.min < 10) lcd.print("0");
        lcd.print(t.min);
        */
    }
    //stampa voltaggio
    lcd.setCursor(10, 0);
    lcd.print("V:");
    lcd.setCursor(12, 0);
    lcd.print(robot.volts);

    /**********************
     * RIGA 2
     **********************/
    lcd.setCursor(0, 1);
    if (robot.mowerDocked == 1) {
        lcd.print(TRS_PARKED);
    } else if (robot.mowerParked == 1) {
        lcd.print(TRS_PARKED);
    } else if (robot.mowerParkedLowBatt == 1) {
        lcd.print(TRS_RECHARG_BAT);
    } else if (robot.mowerError == 1) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ERROR");
        if (robot.wireOffCounter > MAX_WIRE_FAIL) {  //TODO migliorare questo iff?
            lcd.setCursor(0, 1);
            lcd.print(TRS_WIRE_OFF);
        }
    } else if (robot.mowerRunning == 1) {
        lcd.print(TRS_MOVING__);
    }
}

void Setup_Run_LCD_Intro() {
    Serial.println("Setup LCD");
    lcd.begin(16, 2);
    lcd.clear();
    lcd.print("ReP_AL Robot");
    lcd.setCursor(0, 1);
    lcd.print(Version);
    delay(1000);
    lcd.clear();
    Serial.println("LCD Setup OK");
}

void Print_LCD_Wire() {
    lcd.setCursor(0, 1);
    lcd.print("WIRE Detect ");
}

void Print_LCD_Mowing() {
    if (robot.alarmTimedMowON == 0) {
        lcd.setCursor(0, 1);
        lcd.print("Mowing..    ");
    }
    if (robot.alarmTimedMowON == 1) {
        lcd.setCursor(0, 1);
        lcd.print("Timer:");
        lcd.print(robot.alarmTimedMowHour);
        lcd.print(":");
        if (robot.alarmTimedMowMinute < 10) lcd.print("0");
        lcd.print(robot.alarmTimedMowMinute);
        //Mow_Time_Set = 1;
    }
}

void Print_LCD_Compass_Mowing() {
    if (robot.compassHeadingLocked == 1) {
        lcd.setCursor(0, 1);
        if (robot.pwmRight > robot.pwmLeft) lcd.print("<H-Lock Mow ");
        if (robot.pwmLeft > robot.pwmRight) lcd.print(" H-Lock Mow>");
        if (robot.pwmLeft == robot.pwmRight) lcd.print("|H-Lock Mow|");
    }
    if (robot.compassHeadingLocked == 0) {
        lcd.setCursor(0, 1);
        lcd.print("            ");
    }
}

void Print_LCD_Info_Manuel() {
    lcd.setCursor(0, 1);
    lcd.print("Manuel");
}

void Print_LCD_NO_Wire() {
    if ((robot.mowerDocked == 1) || (robot.mowerParked == 1)) {
        lcd.setCursor(7, 1);
        lcd.print(":WIRE OFF");
        robot.wireOnPrinted = 0;
    }
    if ((robot.mowerDocked == 0) && (robot.mowerParked == 0)) {
        lcd.setCursor(0, 1);
        lcd.print(":WIRE OFF        ");
        robot.wireOnPrinted = 0;
    }
}

void Print_LCD_Wire_ON() {
    if ((robot.mowerDocked == 1) || (robot.mowerParked == 1) && (robot.wireOnPrinted = 0)) {
        lcd.setCursor(7, 1);
        lcd.print(":               ");
        robot.wireOnPrinted = 1;
    }
    if ((robot.mowerDocked == 0) && (robot.mowerParked == 0) && (robot.wireOnPrinted = 0)) {
        lcd.setCursor(0, 1);
        lcd.print(":               ");
        robot.wireOnPrinted = 1;
    }
}

void Print_Sonar_Hit() {
    lcd.setCursor(0, 1);
    lcd.print("Sonar Object ");
}

void Print_LCD_Compass_Home() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Compass Home");
    if (robot.rainDetected == 1) {
        lcd.setCursor(0, 1);
        lcd.print("RAINING");
    }
}

void Print_LCD_Heading_for_Home() {
    lcd.setCursor(0, 1);
    lcd.print("Target:");
    lcd.print(((robot.headingLowerLimitCompass - robot.headingLowerLimitCompass) / 2) + robot.headingLowerLimitCompass);
}

void Print_LCD_Menu_Timing(char LCD_Menu_Timing) {
    if (LCD_Menu_Timing == 1) lcd.print("Max Mow Time");
    if (LCD_Menu_Timing == 2) lcd.print("1hr Mow Time");
    if (LCD_Menu_Timing == 3) lcd.print("2hr Mow Time");
    if (LCD_Menu_Timing == 4) lcd.print("  ");  //leave blank
}

void printLoop(byte lCount) {
    lcd.setCursor(13, 1);
    lcd.print(robot.loopCycleMowing);
}

void printFindFire() {
    lcd.clear();
    lcd.print(TRS_FIND_WIRE);
}

void printMessage(char* msg){
     lcd.clear();
    lcd.print(msg);
}