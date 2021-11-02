#include "menuButtons.h"

#include <EEPROM.h>

#include "adcman.h"
#include "batteryUtils.h"
#include "compassUtils.h"
#include "lcdDisplay.h"
#include "movementsUtils.h"
#include "robot.h"
#include "testSketches.h"

 const char *dockedMenu[4] = { TRS_START_CUT, TRS_START_CUT_NO_WIRE, TRS_MOW_THE_LINE, TRS_TEST_MENU };

/**
 *  Reads each of the membrane keys and detects if a key is pressed.
 */
void readMembraneKeys() {
    keyOKPressed = !digitalRead(Start_Key);
    keyUpPressed = !digitalRead(Plus_Key);
    keyDownPressed = !digitalRead(Minus_Key);
    keyBackPressed = !digitalRead(Stop_Key);
    //non mettere delay perchè questo viene chiamato anche mentre è in running
}

/************************************
 *      BUTTONS DOCKED MENU
 ************************************/

/**
 * Check usere interacion in menu options if the Mower is Docked
 */
void checkMembraneSwitchInputDocked() {
    readMembraneKeys();
    menuInteractionsCompleted = true;
    menuModeSelection = 0;
    menuCursorPosition = 0;

    if (keyOKPressed) {
        //start key is pressed, open menu
        menuInteractionsCompleted = false;  // Menu complete will return to the normal loop
        robot.lcdDisplay.clear();
        delay(500);

        char firstRow = 0;
        char menuCursorPositionOld = -1;
        while (menuInteractionsCompleted == false) {  
            // holds the program in a loop until a selection has been made in the membrane button menu

            if (menuCursorPositionOld != menuCursorPosition) {
                //refresh not needed
                //print menu
                robot.lcdDisplay.clear();
                firstRow = menuCursorPosition;  //lcd has 2 rows, print correct rows base of cursor position
                if (menuCursorPosition % 2 > 0) {
                    firstRow--;
                }
                robot.lcdDisplay.setCursor(2, 0);
                robot.lcdDisplay.print(dockedMenu[firstRow]);
                robot.lcdDisplay.setCursor(2, 1);
                robot.lcdDisplay.print(dockedMenu[firstRow+1]);

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
                processMenuDockedChoice();
            } else if (keyBackPressed) {
                Serial.println(F("Stop key is pressed"));
                menuInteractionsCompleted = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print(TRS_MENU_CANCELLED);
                delay(2000);
                robot.lcdDisplay.clear();
                menuCursorPosition = 0;
                menuModeSelection = 0;
            } else if (keyUpPressed) {
                Serial.println(F("+ key is pressed"));
                menuCursorPosition = menuCursorPosition - 1;
                if (menuCursorPosition < 0) {
                    menuCursorPosition = 0;
                }
            } else if (keyDownPressed) {
                menuCursorPosition = menuCursorPosition + 1;
                if (menuCursorPosition > sizeof(dockedMenu)) {
                    menuCursorPosition = sizeof(dockedMenu)-1;
                }
            } 
        }
    }
}

// Defines the actions when that option is selected with the keypad.
void processMenuDockedChoice() {
    if (menuModeSelection == 0 || menuModeSelection == 1) {
        /***************************
         *  Start cut or start cut no wire
         ***************************/
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(0, 0);
        if (menuModeSelection == 1) {
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
        if (menuModeSelection == 1) {
            //TODO disattiva cavo
        }
        leaveChargingStationAndStartCut();
    } else if (menuModeSelection == 2) {
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

        robot.bladeOverride = 1;
        robot.trackWireItterations = 6000;  //TODO perchè ?
        leaveChargingStationAndStartCut();
        Manouver_Start_Mower();  // Sets up the mower to go.
    } else if (menuModeSelection == 3) {
        /***************************
         * Open test menu
         ***************************/
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Test Mower Menu");
        Serial.println(F("Test Menu Selected"));
        menuModeSelection = 0;
        delay(1000);
        robot.lcdDisplay.clear();
        Print_Membrane_Switch_Input_Tests();
    }
}

/************************************
 *      BUTTONS PARKED MENU
 ************************************/

void checkMembraneSwitchInputParked() {
    //Menu Options if the Mower is Parked.
    //TODO if (USE_CHARGING_STATION == 0) TODO aggiungi opzione torna alla base
    //TODO passa parametro al checkMembraneSwitchInputDocked per avvio avvio senza uscire dalla base
   checkMembraneSwitchInputDocked();
}

/************************************
 *      BUTTONS RUNNING MENU
 ************************************/

void checkMembranSwitchRunning() {
    readMembraneKeys();
    if (keyBackPressed || keyOKPressed) {
        manouverParkTheMower();
        checkMembraneSwitchInputParked();
        menuModeSelection = 0;
    }
}

/************************************
 *       BUTTONS TEST MENU
 *************************************/

// Test to displyed on the LCD screen when using the membrane key menus
void Print_LCD_Menu_Tests(char laeveIndex) {
    if (laeveIndex == 1) robot.lcdDisplay.print("Wire Test");
    if (laeveIndex == 2) robot.lcdDisplay.print("Relay Test");
    if (laeveIndex == 3) robot.lcdDisplay.print("Wheel Test");
    if (laeveIndex == 4) robot.lcdDisplay.print("Blade Test");
    if (laeveIndex == 5) robot.lcdDisplay.print("Sonar Test");
    if (laeveIndex == 6) robot.lcdDisplay.print("Turn Test");
    if (laeveIndex == 7) robot.lcdDisplay.print("Volt Amp Test");
    if (laeveIndex == 8) robot.lcdDisplay.print("Compass Test");
    if (laeveIndex == 9) robot.lcdDisplay.print("-  ");
    if (laeveIndex == 10) robot.lcdDisplay.print("-  ");
    if (laeveIndex == 11) robot.lcdDisplay.print("-  ");
    if (laeveIndex == 12) robot.lcdDisplay.print("");  // Leave Blank
}

void Print_Membrane_Switch_Input_Tests() {
    //Menu Options if the Mower is Tests.
    readMembraneKeys();
    menuInteractionsCompleted = 1;
    menuModeSelection = 0;
    menuCursorPosition = 0;

    Serial.println();
    Serial.println(F("Test Menu Activated"));
    menuInteractionsCompleted = false;  // Menu complete will return to the normal loop
    robot.lcdDisplay.clear();
    delay(5);

    while (menuInteractionsCompleted == false) {  // holds the program in a loop until a selection has been made in the membrane button menu
        if (menuCursorPosition == 0) {
            robot.lcdDisplay.setCursor(2, 0);
            Print_LCD_Menu_Tests(1);
            robot.lcdDisplay.setCursor(2, 1);
            Print_LCD_Menu_Tests(2);
        }
        // Gets the values again from the keys
        readMembraneKeys();
        delay(100);

        if (keyOKPressed) {
            menuInteractionsCompleted = true;
            Serial.println(F("Start key is pressed"));
            robot.lcdDisplay.clear();
        }
        if (keyUpPressed) {
            Serial.println(F("+ key is pressed"));
            menuCursorPosition = menuCursorPosition - 1;
            Run_Menu_Order_Testing();
        }
        if (keyDownPressed) {
            menuCursorPosition = menuCursorPosition + 1;
            Run_Menu_Order_Testing();
        }
        if (keyBackPressed) {
            Serial.println(F("Stop key is pressed"));
            menuInteractionsCompleted = true;
            robot.lcdDisplay.clear();
            robot.lcdDisplay.setCursor(0, 0);
            robot.lcdDisplay.print("Menu Cancelled");
            delay(1000);
            robot.lcdDisplay.clear();
            menuModeSelection = 0;
        }
    }
    Activate_Menu_Option_Testing();
}

void Run_Menu_Order_Testing() {
    if (menuCursorPosition == 1) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(1);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(2);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        menuModeSelection = 1;
        Serial.print(F("Menu View : "));
        Serial.print(menuCursorPosition);
        Serial.print(F("| Menu Selection"));
        Serial.println(menuModeSelection);
        delay(100);
    }
    if (menuCursorPosition == 2) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(2);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(3);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        menuModeSelection = 2;
        Serial.print(F("Menu View : "));
        Serial.print(menuCursorPosition);
        Serial.print(F("| Menu Selection"));
        Serial.println(menuModeSelection);
        delay(100);
    }
    if (menuCursorPosition == 3) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(3);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(4);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        menuModeSelection = 3;
        Serial.print(F("Menu View : "));
        Serial.print(menuCursorPosition);
        Serial.print(F("| Menu Selection"));
        Serial.println(menuModeSelection);
        delay(100);
    }
    if (menuCursorPosition == 4) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(4);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(5);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        menuModeSelection = 4;
        Serial.print(F("Menu View : "));
        Serial.print(menuCursorPosition);
        Serial.print(F("| Menu Selection"));
        Serial.println(menuModeSelection);
        delay(100);
    }
    if (menuCursorPosition == 5) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(5);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(6);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        menuModeSelection = 5;
        Serial.print(F("Menu View : "));
        Serial.print(menuCursorPosition);
        Serial.print(F("| Menu Selection"));
        Serial.println(menuModeSelection);
        delay(100);
    }
    if (menuCursorPosition == 6) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(6);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(7);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        menuModeSelection = 6;
        Serial.print(F("Menu View : "));
        Serial.print(menuCursorPosition);
        Serial.print(F("| Menu Selection"));
        Serial.println(menuModeSelection);
        delay(100);
    }
    if (menuCursorPosition == 7) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(7);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(8);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        menuModeSelection = 7;
        Serial.print(F("Menu View : "));
        Serial.print(menuCursorPosition);
        Serial.print(F("| Menu Selection"));
        Serial.println(menuModeSelection);
        delay(100);
    }
    if (menuCursorPosition == 8) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(8);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(9);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        menuModeSelection = 8;
        Serial.print(F("Menu View : "));
        Serial.print(menuCursorPosition);
        Serial.print(F("| Menu Selection"));
        Serial.println(menuModeSelection);
        delay(100);
    }
    if (menuCursorPosition == 9) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(9);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(10);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        menuModeSelection = 9;
        Serial.print(F("Menu View : "));
        Serial.print(menuCursorPosition);
        Serial.print(F("| Menu Selection"));
        Serial.println(menuModeSelection);
        delay(100);
    }
    if (menuCursorPosition == 10) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(10);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(11);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        menuModeSelection = 10;
        Serial.print(F("Menu View : "));
        Serial.print(menuCursorPosition);
        Serial.print(F("| Menu Selection"));
        Serial.println(menuModeSelection);
        delay(100);
    }
    if (menuCursorPosition == 11) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(11);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(12);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        menuModeSelection = 10;
        Serial.print(F("Menu View : "));
        Serial.print(menuCursorPosition);
        Serial.print(F("| Menu Selection"));
        Serial.println(menuModeSelection);
        delay(100);
    }

    delay(100);
}

void Activate_Menu_Option_Testing() {
    if (menuModeSelection == 1) {
        // Perimeter Wire Test
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Wire Test");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Activated");
        Serial.println(F("Perimeter Wire Test Started"));
        delay(5000);
        robot.lcdDisplay.clear();
        menuModeSelection = 0;
        robot.lcdDisplay.clear();
        menuInteractionsCompleted = false;
        while (menuInteractionsCompleted == false) {
            Test_Mower_Check_Wire();
            readMembraneKeys();
            if (keyBackPressed) {
                Serial.println(F("Stop key is pressed"));
                menuInteractionsCompleted = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Test Stopped");
                delay(2000);
                robot.lcdDisplay.clear();
                menuModeSelection = 0;
            }
        }
    }

    if (menuModeSelection == 2) {
        // Relay Test
        menuModeSelection = 0;
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Relay Test");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Activated");
        delay(1000);
        robot.lcdDisplay.clear();
        menuModeSelection = 0;
        robot.lcdDisplay.clear();
        menuInteractionsCompleted = false;
        while (menuInteractionsCompleted == false) {
            Test_Relay();
            readMembraneKeys();
            if (keyBackPressed) {
                Serial.println(F("Stop key is pressed"));
                menuInteractionsCompleted = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Test Stopped");
                delay(2000);
                robot.lcdDisplay.clear();
                menuModeSelection = 0;
            }
        }
    }

    if (menuModeSelection == 3) {
        // Mower Motor Test
        menuModeSelection = 0;
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Wheel Motor Test");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Activated");
        delay(1000);
        robot.lcdDisplay.clear();
        menuModeSelection = 0;
        robot.lcdDisplay.clear();
        robot.I = 1;          // sets the itteration for the motor test
        Test_Wheel_Motors();  // starts the mowe motor test 1x
    }

    if (menuModeSelection == 4) {
        // Blade Motor Test
        menuModeSelection = 0;
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Blade Motor Test");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Activated");
        delay(1000);
        robot.lcdDisplay.clear();
        menuModeSelection = 0;
        robot.lcdDisplay.clear();
        Test_Mower_Blade_Motor();
    }

    if (menuModeSelection == 5) {
        menuModeSelection = 0;
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
                Serial.println(F("Stop key is pressed"));
                menuInteractionsCompleted = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Test Stopped");
                delay(2000);
                robot.lcdDisplay.clear();
                menuModeSelection = 0;
            }
        }
    }

    if (menuModeSelection == 6) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Pattern Mow");
        Serial.println(F("Slot 6 Selected"));
        menuModeSelection = 0;
        delay(3000);
        robot.lcdDisplay.clear();
        Test_Compass_Turn_Function();
    }

    if (menuModeSelection == 7) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Volt robot.amps Test");
        Serial.println(F("robot.volts and robot.amps Test"));
        menuModeSelection = 0;
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

            if (keyBackPressed) {
                Serial.println(F("Stop key is pressed"));
                menuInteractionsCompleted = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Test Stopped");
                delay(2000);
                robot.lcdDisplay.clear();
                menuModeSelection = 0;
            }
        }
    }
    if (menuModeSelection == 8) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Compass Test");
        Serial.println(F("Compass Test Selected"));
        menuModeSelection = 0;
        delay(3000);
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print("robot.heading:");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Degrees:");
        menuInteractionsCompleted = false;
        while (menuInteractionsCompleted == false) {
            // insert Test Code Here
            Get_Compass_Reading();
            Serial.print(F("Comp H:"));
            Serial.print(robot.heading);
            Serial.print("|");
            Serial.println("");
            robot.lcdDisplay.setCursor(9, 0);
            robot.lcdDisplay.print(robot.heading);
            robot.lcdDisplay.setCursor(9, 1);
            robot.lcdDisplay.print(robot.compassHeadingDegrees);
            delay(5);

            if (keyBackPressed) {
                Serial.println(F("Stop key is pressed"));
                menuInteractionsCompleted = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Test Stopped");
                delay(2000);
                robot.lcdDisplay.clear();
                menuModeSelection = 0;
            }
        }
    }

    if (menuModeSelection == 9) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Spare 9");
        Serial.println(F("Slot 9 Selected"));
        menuModeSelection = 0;
        delay(3000);
        robot.lcdDisplay.clear();
    }
    if (menuModeSelection == 10) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Spare 10");
        Serial.println(F("Slot 10 Selected"));
        menuModeSelection = 0;
        delay(3000);
        robot.lcdDisplay.clear();
    }
}
