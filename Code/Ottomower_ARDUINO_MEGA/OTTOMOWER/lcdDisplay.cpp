/**
 *  Print mower status on LCD screen
 */
#include "lcdDisplay.h"
#include "robot.h"

void Print_LCD_Menu_Timing(char LCD_Menu_Timing) {
    if (LCD_Menu_Timing == 1) robot.lcdDisplay.print("Max Mow Time");
    if (LCD_Menu_Timing == 2) robot.lcdDisplay.print("1hr Mow Time");
    if (LCD_Menu_Timing == 3) robot.lcdDisplay.print("2hr Mow Time");
    if (LCD_Menu_Timing == 4) robot.lcdDisplay.print("  ");  //leave blank
}

void lcdUpdateScreen() {
    /**********************
     * RIGA 1
     **********************/
    robot.lcdDisplay.setCursor(0, 0);
    if (robot.mowerDocked == 1 && robot.chargeDetectedMEGA == 1) {
        robot.lcdDisplay.print(TRS_CHARGING);  //is charging
    } else if (robot.rainDetected == 1) {
        robot.lcdDisplay.print(TRS_RAIN);
    } else {
        robot.lcdDisplay.print("        ");
        //stampa l'orario
        //TODO RTC
        /*
        Time t = rtc.time();
        robot.lcdDisplay.print(t.hr);
        robot.lcdDisplay.print(":");
        if (t.min < 10) robot.lcdDisplay.print("0");
        robot.lcdDisplay.print(t.min);
        */
    }
    //stampa voltaggio
    robot.lcdDisplay.setCursor(10, 0);
    robot.lcdDisplay.print("V:");
    robot.lcdDisplay.setCursor(12, 0);
    robot.lcdDisplay.print(robot.volts);

    /**********************
     * RIGA 2
     **********************/
    robot.lcdDisplay.setCursor(0, 1);
    if (robot.mowerDocked == 1) {
        robot.lcdDisplay.print(TRS_PARKED);
    } else if (robot.mowerParked == 1) {
        robot.lcdDisplay.print(TRS_PARKED);
    } else if (robot.mowerParkedLowBatt == 1) {
        robot.lcdDisplay.print(TRS_RECHARG_BAT);
    } else if (robot.mowerError == 1) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print("ERROR");
        if (robot.wireOffCounter > MAX_WIRE_FAIL) {  //TODO migliorare questo iff?
            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.print(TRS_WIRE_OFF);
        }
    } else if (robot.mowerRunning == 1) {
        robot.lcdDisplay.print(TRS_MOVING__);
    }
}

void Setup_Run_LCD_Intro() {
    Serial.println("Setup LCD");
    robot.lcdDisplay.begin(16, 2);
    robot.lcdDisplay.clear();
    robot.lcdDisplay.print("ReP_AL Robot");
    robot.lcdDisplay.setCursor(0, 1);
    robot.lcdDisplay.print(Version);
    delay(1000);
    robot.lcdDisplay.clear();
    Serial.println("LCD Setup OK");
}

void Print_LCD_Wire() {
    robot.lcdDisplay.setCursor(0, 1);
    robot.lcdDisplay.print("WIRE Detect ");
}

void Print_LCD_Mowing() {
    if (robot.alarmTimedMowON == 0) {
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Mowing..    ");
    }
    if (robot.alarmTimedMowON == 1) {
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Timer:");
        robot.lcdDisplay.print(robot.alarmTimedMowHour);
        robot.lcdDisplay.print(":");
        if (robot.alarmTimedMowMinute < 10) robot.lcdDisplay.print("0");
        robot.lcdDisplay.print(robot.alarmTimedMowMinute);
        //Mow_Time_Set = 1;
    }
}

void Print_LCD_Compass_Mowing() {
    if (robot.compassHeadingLocked == 1) {
        robot.lcdDisplay.setCursor(0, 1);
        if (robot.pwmRight > robot.pwmLeft) robot.lcdDisplay.print("<H-Lock Mow ");
        if (robot.pwmLeft > robot.pwmRight) robot.lcdDisplay.print(" H-Lock Mow>");
        if (robot.pwmLeft == robot.pwmRight) robot.lcdDisplay.print("|H-Lock Mow|");
    }
    if (robot.compassHeadingLocked == 0) {
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("            ");
    }
}

void Print_LCD_Info_Manuel() {
    robot.lcdDisplay.setCursor(0, 1);
    robot.lcdDisplay.print("Manuel");
}

void Print_LCD_NO_Wire() {
    if ((robot.mowerDocked == 1) || (robot.mowerParked == 1)) {
        robot.lcdDisplay.setCursor(7, 1);
        robot.lcdDisplay.print(":WIRE OFF");
        robot.wireOnPrinted = 0;
    }
    if ((robot.mowerDocked == 0) && (robot.mowerParked == 0)) {
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print(":WIRE OFF        ");
        robot.wireOnPrinted = 0;
    }
}

void Print_LCD_Wire_ON() {
    if ((robot.mowerDocked == 1) || (robot.mowerParked == 1) && (robot.wireOnPrinted = 0)) {
        robot.lcdDisplay.setCursor(7, 1);
        robot.lcdDisplay.print(":               ");
        robot.wireOnPrinted = 1;
    }
    if ((robot.mowerDocked == 0) && (robot.mowerParked == 0) && (robot.wireOnPrinted = 0)) {
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print(":               ");
        robot.wireOnPrinted = 1;
    }
}

void Print_Sonar_Hit() {
    robot.lcdDisplay.setCursor(0, 1);
    robot.lcdDisplay.print("Sonar Object ");
}

void Print_LCD_Compass_Home() {
    robot.lcdDisplay.clear();
    robot.lcdDisplay.setCursor(0, 0);
    robot.lcdDisplay.print("Compass Home");
    if (robot.rainDetected == 1) {
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("RAINING");
    }
}

void Print_LCD_Heading_for_Home() {
    robot.lcdDisplay.setCursor(0, 1);
    robot.lcdDisplay.print("Target:");
    robot.lcdDisplay.print(((robot.headingLowerLimitCompass - robot.headingLowerLimitCompass) / 2) + robot.headingLowerLimitCompass);
}

void printLoop(byte lCount) {
    robot.lcdDisplay.setCursor(13, 1);
    robot.lcdDisplay.print(robot.loopCycleMowing);
}

void printFindFire() {
    robot.lcdDisplay.clear();
    robot.lcdDisplay.print(TRS_FIND_WIRE);
}

void printMessage(char* msg){
     robot.lcdDisplay.clear();
    robot.lcdDisplay.print(msg);
}