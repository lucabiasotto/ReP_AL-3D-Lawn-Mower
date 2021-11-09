#include "menuButtons.h"

#include <EEPROM.h>

#include "adcman.h"
#include "batteryUtils.h"
#include "compassUtils.h"
#include "lcdDisplay.h"
#include "movementsUtils.h"
#include "robot.h"
#include "testSketches.h"

const char dockMenuCount = 5;
const char *dockedMenu[dockMenuCount] = {TRS_START_CUT, TRS_START_CUT_NO_WIRE, TRS_MOW_THE_LINE, TRS_GO_HOME, TRS_TEST_MENU};
const Callbacks dockedMenuCallbacks[5] = {START_CUT, START_CUT_NO_WIRE, MOW_THE_LINE, GO_HOME, TEST_MENU};
const char testMenuCount = 8;
const char *testdMenu[8] = {"Wire Test", "Relay Test", "Wheel Test", "Blade Test", "Sonar Test", "Turn Test", "Volt Amp Test", "Compass Test"};
const Callbacks testdMenuCallbacks[8] = {WIRE_TEST, RELAY_TEST, WHEEL_TEST, BLADE_TEST, SONAR_TEST, TURN_TEST, VOLT_AMP_TEST, COMPASS_TEST};

/**
 *  Reads each of the membrane keys and detects if a key is pressed.
 */
void readMembraneKeys() {
    keyOKPressed = !digitalRead(SWITCH_OK_KEY_PIN);
    keyUpPressed = !digitalRead(SWITCH_PLUS_KEY_PIN);
    keyDownPressed = !digitalRead(SWITCH_MINUS_KEY_PIN);
    keyBackPressed = !digitalRead(SWITCH_STOP_KEY_PIN);
    //non mettere delay perchè questo viene chiamato anche mentre è in running
}

void openMenu(const char *menuItems[], const Callbacks menuCallbacks[], const char leaveCount) {
    //start key is pressed, open menu
    bool menuInteractionsCompleted = false;  // Menu complete will return to the normal loop
    char firstRow = 0;
    char menuCursorPositionOld = -1;
    char menuCursorPosition = 0;
    robot.lcdDisplay.clear();
    delay(500);

    while (menuInteractionsCompleted == false) {
        // holds the program in a loop until a selection has been made in the membrane button menu

        if (menuCursorPositionOld != menuCursorPosition) {
            //refresh not needed
            //update menu base of cursor position
            robot.lcdDisplay.clear();
            delay(100);
            firstRow = menuCursorPosition;  //lcd has 2 rows, print correct rows base of cursor position
            if (menuCursorPosition % 2 > 0) {
                firstRow--;
            }
            robot.lcdDisplay.setCursor(2, 0);
            robot.lcdDisplay.print(menuItems[firstRow]);
            if (firstRow + 1 < leaveCount) {
                robot.lcdDisplay.setCursor(2, 1);
                robot.lcdDisplay.print(menuItems[firstRow + 1]);
            } else {
                robot.lcdDisplay.setCursor(2, 1);
                robot.lcdDisplay.print("                ");
            }
            //print cursor position
            robot.lcdDisplay.setCursor(0, menuCursorPosition % 2);
            robot.lcdDisplay.print(">");
            menuCursorPositionOld = menuCursorPosition;
        }

        // Gets the values again from the keys
        readMembraneKeys();
        delay(100);

        if (keyOKPressed) {
            Serial.println(F("Start key is pressed"));
            menuInteractionsCompleted = true;
            robot.lcdDisplay.clear();
            processCallback(menuCallbacks[menuCursorPosition]);
        } else if (keyBackPressed) {
            Serial.println(F("Stop key is pressed"));
            menuInteractionsCompleted = true;
            robot.lcdDisplay.clear();
            robot.lcdDisplay.setCursor(0, 0);
            robot.lcdDisplay.print(TRS_MENU_CANCELLED);
            delay(2000);
            robot.lcdDisplay.clear();
            menuCursorPosition = 0;
        } else if (keyUpPressed) {
            Serial.println(F("+ key is pressed"));
            menuCursorPosition = menuCursorPosition - 1;
            if (menuCursorPosition < 0) {
                menuCursorPosition = 0;
            }
        } else if (keyDownPressed) {
            menuCursorPosition = menuCursorPosition + 1;
            if (menuCursorPosition >= leaveCount) {
                menuCursorPosition = leaveCount - 1;
            }
        }
    }
}

void closeMenu() {
    robot.lcdDisplay.clear();
    delay(1000);
}

/**
 * Process passed menu leave callback
 */
void processCallback(Callbacks callback) {
    bool menuInteractionsCompleted = false;

    long tmpTimer = 0;

    switch (callback) {
        case START_CUT:
        case START_CUT_NO_WIRE:

            /***************************
             *  Start cut or start cut no wire
             ***************************/
            robot.lcdDisplay.clear();
            robot.lcdDisplay.setCursor(0, 0);
            if (callback == START_CUT) {
                robot.lcdDisplay.print(TRS_START_CUT);
            } else {
                robot.lcdDisplay.print(TRS_START_CUT_NO_WIRE);
            }
            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.print("*****");
            for (int i = 5; i >= 0; i--) {
                robot.lcdDisplay.setCursor(i, 1);
                robot.lcdDisplay.print(" ");
                delay(1000);
            }

            //TODO BUZZ!!!
            robot.lcdDisplay.clear();
            delay(10);

            robot.trackWireItterations = TRACK_WIRE_ZONE_CYCLES;
            if (callback == START_CUT_NO_WIRE) {
                //user disable cable
                robot.isPerimeterWireEnable = false;
            } else {
                robot.isPerimeterWireEnable = true;
            }
            leaveChargingStation(FOLLOW_WIRE_WHEN_LEAVE_DOCK);

            // Sets up the mower to go.
            robot.mowerDocked = 0;
            robot.mowerParked = 0;
            robot.mowerRunning = 1;
            robot.mowerParkedLowBatt = 0;
            robot.mowerTrackToCharge = 0;
            robot.rainHitDetected = 0;
            robot.mowerError = 0;
            robot.loopCycleMowing = 0;
            robot.turnOnMotorsRelay();

            break;
        case MOW_THE_LINE:
            /***************************
             * Follow line
             ***************************/
            robot.lcdDisplay.clear();
            robot.lcdDisplay.setCursor(0, 0);
            robot.lcdDisplay.print(TRS_MOW_THE_LINE);
            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.print("*****");
            delay(1000);
            for (int i = 5; i >= 0; i--) {
                robot.lcdDisplay.setCursor(i, 1);
                robot.lcdDisplay.print(" ");
                delay(1000);
            }

            //TODO BUZZ!!!
            robot.lcdDisplay.clear();

            robot.isPerimeterWireEnable = true;
            robot.bladeOverride = 1;
            robot.trackWireItterations = 6000;  //TODO per quante iterazioni segue il cavo...mmmm
            leaveChargingStation(true);
            break;
        case GO_HOME:
            robot.lcdDisplay.clear();
            robot.lcdDisplay.print(TRS_GO_HOME);
            Serial.println(F("Go home Selected"));
            delay(1000);
            robot.lcdDisplay.clear();
            goToChargingStation();
            break;
        case TEST_MENU:
            /***************************
             * Open test menu
             ***************************/
            robot.lcdDisplay.clear();
            robot.lcdDisplay.print("Test Mower Menu");
            Serial.println(F("Test Menu Selected"));
            delay(1000);
            robot.lcdDisplay.clear();
            openTestsMenu();
            break;
        case WIRE_TEST:
            /***************************
             *  Perimeter Wire Test
             ***************************/
            robot.lcdDisplay.clear();
            robot.lcdDisplay.print("Wire Test");
            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.print("Activated");
            Serial.println(F("Perimeter Wire Test Started"));
            delay(5000);
            robot.lcdDisplay.clear();
            robot.lcdDisplay.clear();
            while (menuInteractionsCompleted == false) {
                Test_Mower_Check_Wire();
                readMembraneKeys();
                if (keyBackPressed) {
                    menuInteractionsCompleted = true;
                    closeMenu();
                }
            }
            break;
        case RELAY_TEST:
            /*************************** 
             * Relay Test
             ****************************/
            robot.lcdDisplay.clear();
            robot.lcdDisplay.print("Relay Test");
            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.print("Activated");
            delay(1000);
            robot.lcdDisplay.clear();
            menuInteractionsCompleted = false;
            while (menuInteractionsCompleted == false) {
                Test_Relay();
                readMembraneKeys();
                if (keyBackPressed) {
                    menuInteractionsCompleted = true;
                    closeMenu();
                }
            }
            break;
        case WHEEL_TEST:
            /*************************** 
             *  Mower Motor Test
             ****************************/
            robot.lcdDisplay.clear();
            robot.lcdDisplay.print("Wheel Motor Test");
            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.print("Activated");
            delay(1000);
            robot.lcdDisplay.clear();
            Test_Wheel_Motors();  // starts the mowe motor test 1x
            break;
        case BLADE_TEST:
            /*************************** 
             *  Blade Motor Test
             ****************************/
            robot.lcdDisplay.clear();
            robot.lcdDisplay.print("Blade Motor Test");
            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.print("Activated");
            delay(1000);
            robot.lcdDisplay.clear();
            Test_Mower_Blade_Motor();
            break;
        case SONAR_TEST:
            /*************************** 
             * Sonar test
             ****************************/
            robot.lcdDisplay.clear();
            robot.lcdDisplay.print("Sonar Test");
            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.print("Activated");
            delay(1000);
            robot.lcdDisplay.clear();
            menuInteractionsCompleted = false;
            while (menuInteractionsCompleted == false) {
                Test_Sonar_Array();
                readMembraneKeys();
                if (keyBackPressed) {
                    menuInteractionsCompleted = true;
                    closeMenu();
                }
            }
            break;
        case TURN_TEST:
            /*************************** 
             * Turn test
             ****************************/
            robot.lcdDisplay.clear();
            robot.lcdDisplay.print("Turn test");
            delay(3000);
            robot.lcdDisplay.clear();
            Test_Compass_Turn_Function();
            break;
        case VOLT_AMP_TEST:
            /*************************** 
             * Volt Amp test
             ***************************/
            robot.lcdDisplay.clear();
            robot.lcdDisplay.print("Volt/Amp Test");
            Serial.println(F("Volt and Amp Test"));
            delay(1000);
            robot.lcdDisplay.clear();
            menuInteractionsCompleted = false;
            while (menuInteractionsCompleted == false) {
                readVoltAmp();
                delay(100);
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Volt:");
                robot.lcdDisplay.print(robot.volts);
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("robot.amps:");
                robot.lcdDisplay.print(robot.amps);
                Serial.println("");

                readMembraneKeys();
                if (keyBackPressed) {
                    menuInteractionsCompleted = true;
                    closeMenu();
                }
            }
            break;
        case COMPASS_TEST:
            /*************************** 
             * Compass test
             ***************************/
            robot.lcdDisplay.clear();
            robot.lcdDisplay.print("Compass Test");
            Serial.println(F("Compass Test Selected"));
            delay(3000);
            robot.lcdDisplay.clear();
            robot.lcdDisplay.setCursor(0, 0);
            //robot.lcdDisplay.print("Heading:");
            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.print("Degrees:");
            menuInteractionsCompleted = false;
            
            while (menuInteractionsCompleted == false) {
                // insert Test Code Here
                readRobotCompassDegrees();

                if (millis() - tmpTimer > 500) {
                    robot.lcdDisplay.setCursor(9, 1);
                    robot.lcdDisplay.print((int)robot.compassHeadingDegrees);
                    tmpTimer = millis();
                }
                readMembraneKeys();
                if (keyBackPressed) {
                    menuInteractionsCompleted = true;
                    closeMenu();
                }
            }
            break;
        default:
            break;
    }
}

/************************************
 *      BUTTONS DOCKED MENU
 ************************************/

/**
 * Check usere interacion in menu options if the Mower is Docked
 */
void checkMembraneSwitchInputDocked() {
    readMembraneKeys();
    if (keyOKPressed) {
        //start key is pressed, open menu
        openMenu(dockedMenu, dockedMenuCallbacks, dockMenuCount);
    }
}

/************************************
 *      BUTTONS PARKED MENU
 ************************************/

void checkMembraneSwitchInputParked() {
    //Menu Options if the Mower is Parked.
    //TODO if (USE_CHARGING_STATION == 0) TODO aggiungi opzione torna alla base
    //TODO passa parametro al checkMembraneSwitchInputDocked per avvio avvio senza uscire dalla base
    readMembraneKeys();
    if (keyOKPressed) {
        //start key is pressed, open menu
        openMenu(dockedMenu, dockedMenuCallbacks, dockMenuCount);
    }
}

/************************************
 *      BUTTONS RUNNING MENU
 ************************************/

void checkMembranSwitchRunning() {
    readMembraneKeys();
    if (keyBackPressed || keyOKPressed) {
        manouverParkTheMower();  //put robot in park status
    }
}

/************************************
 *       BUTTONS TEST MENU
 *************************************/

void openTestsMenu() {
    openMenu(testdMenu, testdMenuCallbacks, testMenuCount);
}