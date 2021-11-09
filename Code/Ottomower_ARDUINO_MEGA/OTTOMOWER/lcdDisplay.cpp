/**
 *  Print mower status on LCD screen
 */
#include "lcdDisplay.h"

#include "robot.h"

// make some custom characters:
byte robotIcon[8] = {
    0b00000,
    0b00100,
    0b01110,
    0b11111,
    0b11111,
    0b11111,
    0b00000,
    0b00000};
byte forwardIcon[8] = {
    0b00000,
    0b00100,
    0b01110,
    0b10101,
    0b00100,
    0b00100,
    0b00100,
    0b00000};

byte leftIcon[8] = {
    0b00000,
    0b00010,
    0b00100,
    0b01000,
    0b11111,
    0b01000,
    0b00100,
    0b00010};

byte rightIcon[8] = {
    0b00000,
    0b01000,
    0b00100,
    0b00010,
    0b11111,
    0b00010,
    0b00100,
    0b01000};

byte backwarddIcon[8] = {
    0b00000,
    0b00100,
    0b00100,
    0b00100,
    0b10101,
    0b01110,
    0b00100,
    0b00000};

void lcdInit() {
    Serial.println("Setup LCD");
    robot.lcdDisplay.begin(16, 2);

    robot.lcdDisplay.createChar(robot.CHAR_ROBOT, robotIcon);
    robot.lcdDisplay.createChar(robot.CHAR_FORWARD, forwardIcon);
    robot.lcdDisplay.createChar(robot.CHAR_LEFT_ARROW, leftIcon);
    robot.lcdDisplay.createChar(robot.CHAR_RIGHT_ARROW, rightIcon);
    robot.lcdDisplay.createChar(robot.CHAR_BACK_ARROW, backwarddIcon);

    robot.lcdDisplay.clear();
    robot.lcdDisplay.print("Ottomower");
    robot.lcdDisplay.setCursor(0, 1);
    robot.lcdDisplay.print(Version);
    delay(1000);
    robot.lcdDisplay.clear();
    Serial.println("LCD Setup OK");
}

void lcdUpdateScreen() {
    /**********************
     * RIGA 1
     * 
     * ┌────────────────┐
     * │0123456789012345│
     * │xxx        v12.2│
     * │^o^             │
     * └────────────────┘
     * 
     **********************/

    robot.lcdDisplay.setCursor(0, 0);
    if (robot.mowerDocked == 1 && robot.chargeDetectedMEGA == 1) {
        robot.lcdDisplay.print("                ");
        robot.lcdDisplay.print(TRS_CHARGING);  //is charging
    } else if (robot.rainDetected == 1) {
        robot.lcdDisplay.print("                ");
        robot.lcdDisplay.print(TRS_RAIN);
    } else if (robot.mowerError == 1) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print("ERROR");
    } else {
        //first 3 char are for sonar obstacle, write in readSonarSensors methods
        //robot.lcdDisplay.print("   ");
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
        if (robot.wireOffCounter > MAX_WIRE_FAIL) {
            //TODO
            //robot.lcdDisplay.setCursor(0, 1);
            //robot.lcdDisplay.print(TRS_WIRE_OFF); CAVO KO
        }
    } else if (robot.mowerRunning == 1 || robot.trackingWire == 1 || robot.searchingWire == 1) {
        //log robot direction
        if (robot.SonarHit1Total >= SONAR_MAX_HIT || robot.SonarHit2Total >= SONAR_MAX_HIT || robot.SonarHit3Total >= SONAR_MAX_HIT) {
            //sonar hit
            if (robot.distance1 < SONAR_MAX_DISTANCE || robot.distance2 < SONAR_MAX_DISTANCE) {
                //-->
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print(" -         SONAR");
                robot.lcdDisplay.setCursor(2, 1);
                robot.lcdDisplay.write(robot.CHAR_RIGHT_ARROW);
            } else if (robot.distance3 < SONAR_MAX_DISTANCE) {
                //<--
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print(" -         SONAR");
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.write(robot.CHAR_LEFT_ARROW);
            }
        } else if (robot.loopCycleMowing < CYCLE_FOR_STABILEZE || COMPASS_HEADING_HOLD_ENABLED == 0 || COMPASS_ACTIVATE == 0) {
            //^-^
            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.print(" -              ");
            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.write(robot.CHAR_FORWARD);
            robot.lcdDisplay.setCursor(2, 1);
            robot.lcdDisplay.write(robot.CHAR_FORWARD);
        } else {
            //compass stabilization
            robot.lcdDisplay.setCursor(0, 1);
            if (robot.pwmRight > robot.pwmLeft) {
                //<--
                robot.lcdDisplay.print(" -          GYRO");
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.write(robot.CHAR_FORWARD);
            } else if (robot.pwmLeft > robot.pwmRight) {
                //-->
                robot.lcdDisplay.print(" -          GYRO");
                robot.lcdDisplay.setCursor(2, 1);
                robot.lcdDisplay.write(robot.CHAR_FORWARD);
            } else {
                //^-^
                robot.lcdDisplay.print(" -          GYRO");
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.write(robot.CHAR_FORWARD);
                robot.lcdDisplay.setCursor(2, 1);
                robot.lcdDisplay.write(robot.CHAR_FORWARD);
            }
        }

        if(robot.searchingWire){
            //is searching wire, pring ->WIRE
            robot.lcdDisplay.setCursor(12, 1);
            robot.lcdDisplay.print(TRS_WIRE);
            robot.lcdDisplay.setCursor(11, 1);
            robot.lcdDisplay.write(robot.CHAR_RIGHT_ARROW);
        }

        robot.lcdDisplay.setCursor(1, 1);
        robot.lcdDisplay.write(robot.CHAR_ROBOT);
    }

    //TODO stampare il loop?robot.loopCycleMowing
}

void lcdPrintWireDetected() {
    //TODO tira via fai tutto nello stampa lcd info
    robot.lcdDisplay.setCursor(0, 11);
    robot.lcdDisplay.print(TRS_WIRE);
}

void lcdPrintWireLost() {
    if (robot.mowerDocked == 1 || robot.mowerParked == 1) {
        robot.lcdDisplay.setCursor(8, 1);
        robot.lcdDisplay.print(TRS_NO_WIRE);
    }
    if (robot.mowerDocked == 0 && robot.mowerParked == 0) {
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(TRS_NO_WIRE);
    }
}

void lcdPrintSearchBoxDirectionWithCompass() {
    robot.lcdDisplay.clear();
    robot.lcdDisplay.setCursor(0, 0);
    robot.lcdDisplay.print(TRS_COMPASS_HOME);
    if (robot.rainDetected == 1) {
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print(TRS_RAINING);
    }
}

void printMessage(char* msg) {
    robot.lcdDisplay.clear();
    robot.lcdDisplay.print(msg);
}