#include "menuButtons.h"

#include <EEPROM.h>

#include "adcman.h"
#include "batteryUtils.h"
#include "compassUtils.h"
#include "lcdDisplay.h"
#include "movementsUtils.h"
#include "robot.h"
#include "testSketches.h"

// BUTTONS DOCKED MENU

// Reads each of the membrane keys and detects if a key is pressed.
void Read_Membrane_Keys() {
    Start_Key_X = digitalRead(Start_Key);
    Plus_Key_X = digitalRead(Plus_Key);
    Minus_Key_X = digitalRead(Minus_Key);
    Stop_Key_X = digitalRead(Stop_Key);
}

// Test to displyed on the LCD screen when using the membrane key menus
void Print_LCD_Menu_Docked(char LCD_Menu_Docked) {
    if (LCD_Menu_Docked == 1) robot.lcdDisplay.print("Exit Dock Z-1");
    if (LCD_Menu_Docked == 2) robot.lcdDisplay.print("Exit Dock Z-2");
    if (LCD_Menu_Docked == 3) robot.lcdDisplay.print("Quick Start");
    if (LCD_Menu_Docked == 4) robot.lcdDisplay.print("Trampoline Cut");
    if (LCD_Menu_Docked == 5) robot.lcdDisplay.print("Mow the Line");
    if (LCD_Menu_Docked == 6) robot.lcdDisplay.print("Test Menu");
    if (LCD_Menu_Docked == 7) robot.lcdDisplay.print("Setup Mower");
    if (LCD_Menu_Docked == 8) robot.lcdDisplay.print("-- Spare 8 --");
    if (LCD_Menu_Docked == 9) robot.lcdDisplay.print("-- Spare 9 --");
    if (LCD_Menu_Docked == 10) robot.lcdDisplay.print("-- Spare 10 --");
    if (LCD_Menu_Docked == 11) robot.lcdDisplay.print("-- Spare 11 --");
    if (LCD_Menu_Docked == 12) robot.lcdDisplay.print("-- Spare 12 --");
}

/**
 * Menu Options if the Mower is Docked
 */
void checkMembraneSwitchInputDocked() {
    Read_Membrane_Keys();
    Menu_Complete = 1;
    Menu_Mode_Selection = 0;
    Menu_View = 0;

    if (!Start_Key_X) {  // If the start key is pressed
        Serial.println();
        Serial.println(F("Start Key Pressed"));
        Menu_Complete = false;  // Menu complete will return to the normal loop
        robot.lcdDisplay.clear();
        delay(5);
        Serial.println();
        Serial.println(F("Docked Menu Activated"));
        delay(500);
        robot.lcdDisplay.clear();
        delay(5);

        while (Menu_Complete == false) {  // holds the program in a loop until a selection has been made in the membrane button menu
            if (Menu_View == 0) {
                robot.lcdDisplay.setCursor(2, 0);
                Print_LCD_Menu_Docked(1);
                robot.lcdDisplay.setCursor(2, 1);
                Print_LCD_Menu_Docked(2);
            }
            // Gets the values again from the keys
            Read_Membrane_Keys();
            delay(100);

            if (!Start_Key_X) {
                Menu_Complete = true;
                Serial.println(F("Start key is pressed"));
                robot.lcdDisplay.clear();
            }
            if (!Plus_Key_X) {
                Serial.println(F("+ key is pressed"));
                Menu_View = Menu_View - 1;
                Run_Menu_Order_Docked();
            }
            if (!Minus_Key_X) {
                Menu_View = Menu_View + 1;
                Run_Menu_Order_Docked();
            }
            if (!Stop_Key_X) {
                Serial.println(F("Stop key is pressed"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Menu Cancelled");
                delay(1000);
                robot.lcdDisplay.clear();
                Menu_Mode_Selection = 0;
            }
        }
        Activate_Menu_Option_Docked();
    }
}

// Code to scroll the menu and print the menu options in the LCD
void Run_Menu_Order_Docked() {
    if (Menu_View == 1) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Docked(1);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Docked(2);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 1;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 2) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Docked(2);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Docked(3);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 2;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 3) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Docked(3);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Docked(4);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 3;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 4) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Docked(4);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Docked(5);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 4;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 5) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Docked(5);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Docked(6);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 5;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 6) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Docked(6);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Docked(7);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 6;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 7) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Docked(7);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Docked(8);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 7;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 8) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Docked(8);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Docked(9);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 8;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 9) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Docked(9);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Docked(10);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 9;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 10) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Docked(10);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Docked(11);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 10;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 11) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Docked(11);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Docked(12);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 10;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }

    delay(100);
}

// Defines the actions when that option is selected with the keypad.
void Activate_Menu_Option_Docked() {
    if (Menu_Mode_Selection == 1) {
        // Exit the mower from the Garage and go to Zone 1;
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Manuel Start");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Exit Dock Z1");
        Serial.println(F("Exit to Zone 1 - Free Mow"));
        delay(1000);
        robot.lcdDisplay.clear();
        Print_Membrane_Switch_Input_Timing();
        Menu_Mode_Selection = 0;
        delay(1000);
        robot.lcdDisplay.clear();
        /*
        if (Mow_Time_Set == 1) {
            //Enter here the code to go to zone 1 from dock
            robot.exitZone = 1;
            robot.trackWireItterations = TRACK_WIRE_ZONE_1_CYCLES;
            robot.leaveChargingStation();
        }
        */
    }

    if (Menu_Mode_Selection == 2) {
        // Exit the mower from the Garage and go to Zone 2;
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Manuel Start");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Exit Dock Z2");
        delay(1000);
        robot.lcdDisplay.clear();
        Print_Membrane_Switch_Input_Timing();  // Changes the menu to select the mow time
        Menu_Mode_Selection = 0;
        delay(1000);
        robot.lcdDisplay.clear();
        /*
        if (Mow_Time_Set == 1) {
            robot.exitZone = 2;
            robot.trackWireItterations = TRACK_WIRE_ZONE_2_CYCLES;
            robot.leaveChargingStation();
        }*/
    }

    if (Menu_Mode_Selection == 3) {
        // Quick Start the Mower in the middle of the Garden;
        Serial.println("Quick Start Selected");
        Print_Membrane_Switch_Input_Timing();  // Changes the menu to select the mow time
        Menu_Mode_Selection = 0;
        delay(1000);
        //if (Mow_Time_Set == 1) Manouver_Start_Mower();
        robot.lcdDisplay.clear();
    }

    if (Menu_Mode_Selection == 4) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Trampoline Cut!");
        Serial.println(F("Mower Set to Cut under Trampoline"));
        Menu_Mode_Selection = 0;
        delay(3000);
        robot.lcdDisplay.clear();
        Special_Cut_Under_Trampoline_Function();
    }

    if (Menu_Mode_Selection == 5) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Blade will spin");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Mow the Line");
        delay(1000);
        robot.lcdDisplay.clear();
        Print_Membrane_Switch_Input_Timing();  // Changes the menu to select the mow time
        /*
        if (Mow_Time_Set == 1) {
            Serial.println(F("Cutting the grass on the boundary wire"));
            robot.bladeOverride = 1;
            robot.trackWireItterations = 6000;
            robot.exitZone = 3;
            robot.leaveChargingStation();
            Manouver_Start_Mower();  // Sets up the mower to go.
        }*/
    }

    if (Menu_Mode_Selection == 6) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Test Mower Menu");
        Serial.println(F("Test Menu Selected"));
        Menu_Mode_Selection = 0;
        delay(1000);
        robot.lcdDisplay.clear();
        Print_Membrane_Switch_Input_Tests();
    }
    if (Menu_Mode_Selection == 7) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Mower Setup");
        Serial.println(F("Mower Setup Selected"));
        Menu_Mode_Selection = 0;
        delay(1000);
        robot.lcdDisplay.clear();
        Print_Membrane_Switch_Input_Settings();
    }
}

// BUTTONS PARKED MENU

// Test to displyed on the LCD screen when using the membrane key menus
void Print_LCD_Menu_Parked(char LCD_Menu_Parked) {
    if (LCD_Menu_Parked == 1) robot.lcdDisplay.print("Mow Grass     ");
    if (LCD_Menu_Parked == 2) {
        if (USE_CHARGING_STATION == 1) robot.lcdDisplay.print("Go To Dock");
        if (USE_CHARGING_STATION == 0) robot.lcdDisplay.print("Dock Removed");
    }
    if (LCD_Menu_Parked == 3) robot.lcdDisplay.print("Trampoline Cut");
    if (LCD_Menu_Parked == 4) robot.lcdDisplay.print("Test Menu");
    if (LCD_Menu_Parked == 5) robot.lcdDisplay.print("Setup Mower");
    if (LCD_Menu_Parked == 6) robot.lcdDisplay.print("-- SPARE 6 ---");
    if (LCD_Menu_Parked == 7) robot.lcdDisplay.print("-- SPARE 7 ---");
    if (LCD_Menu_Parked == 8) robot.lcdDisplay.print("-- SPARE 8 ---");
    if (LCD_Menu_Parked == 9) robot.lcdDisplay.print("-- SPARE 9 ---");
    if (LCD_Menu_Parked == 10) robot.lcdDisplay.print("-- SPARE 10 ---");
    if (LCD_Menu_Parked == 11) robot.lcdDisplay.print("-- SPARE 11 ---");
    if (LCD_Menu_Parked == 12) robot.lcdDisplay.print("");  // Leave Blank
}

void checkMembraneSwitchInputParked() {
    //Menu Options if the Mower is Parked.
    Read_Membrane_Keys();
    Menu_Complete = 1;
    Menu_Mode_Selection = 0;
    Menu_View = 0;

    if (!Start_Key_X) {  // If the start key is pressed
        Serial.println();
        Serial.println(F("Start Key Pressed"));
        Menu_Complete = false;  // Menu complete will return to the normal loop
        robot.lcdDisplay.clear();
        delay(5);
        Serial.println();
        Serial.println(F("Parked Menu Activated"));
        delay(500);

        while (Menu_Complete == false) {  // holds the program in a loop until a selection has been made in the membrane button menu
            if (Menu_View == 0) {
                robot.lcdDisplay.setCursor(2, 0);
                Print_LCD_Menu_Parked(1);
                robot.lcdDisplay.setCursor(2, 1);
                Print_LCD_Menu_Parked(2);
            }
            // Gets the values again from the keys
            Read_Membrane_Keys();
            delay(100);

            if (!Start_Key_X) {
                Menu_Complete = true;
                Serial.println(F("Start key is pressed"));
                robot.lcdDisplay.clear();
            }
            if (!Plus_Key_X) {
                Serial.println(F("+ key is pressed"));
                Menu_View = Menu_View - 1;
                Run_Menu_Order_Parked();
            }
            if (!Minus_Key_X) {
                Menu_View = Menu_View + 1;
                Run_Menu_Order_Parked();
            }
            if (!Stop_Key_X) {
                Serial.println(F("Stop key is pressed"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Menu Cancelled");
                delay(1000);
                robot.lcdDisplay.clear();
                Menu_Mode_Selection = 0;
            }
        }
        Activate_Menu_Option_Parked();
    }
}

void Run_Menu_Order_Parked() {
    if (Menu_View == 1) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Parked(1);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Parked(2);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 1;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 2) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Parked(2);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Parked(3);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 2;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 3) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Parked(3);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Parked(4);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 3;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 4) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Parked(4);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Parked(5);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 4;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 5) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Parked(5);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Parked(6);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 5;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 6) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Parked(6);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Parked(7);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 6;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 7) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Parked(7);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Parked(8);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 7;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 8) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Parked(8);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Parked(9);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 8;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 9) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Parked(9);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Parked(10);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 9;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 10) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Parked(10);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Parked(11);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 10;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 11) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Parked(11);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Parked(12);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 10;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }

    delay(100);
}

// Defines the actions when that option is selected with the keypad.
void Activate_Menu_Option_Parked() {
    if (Menu_Mode_Selection == 1) {
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        Print_Membrane_Switch_Input_Timing();
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Mow Re-Starting");
        Serial.println(F("Mower Starting"));
        delay(2000);
        Manouver_Start_Mower();  // Restarts the mower again from standing position
        robot.lcdDisplay.clear();
    }

    if (Menu_Mode_Selection == 2) {
        if (USE_CHARGING_STATION == 1) {
            robot.lcdDisplay.clear();
            robot.lcdDisplay.print("Returning Home");
            Serial.println(F("Sending Mower Home"));
            delay(100);
            Menu_Mode_Selection = 0;  // Releases the loop in the membrane button section.
            delay(1000);
            robot.lcdDisplay.clear();
            goToChargingStation();
        }
        if (USE_CHARGING_STATION == 0) {
            robot.lcdDisplay.clear();
            robot.lcdDisplay.print("No Dock Active");
            Serial.println(F("Activate Docking Station in Settings"));
            delay(100);
            Menu_Mode_Selection = 0;  // Releases the loop in the membrane button section.
            delay(1000);
            robot.lcdDisplay.clear();
        }
    }

    if (Menu_Mode_Selection == 3) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Trampoline Cut!");
        Serial.println(F("Mower Set to Cut under Trampoline"));
        Menu_Mode_Selection = 0;
        delay(3000);
        robot.lcdDisplay.clear();
        Special_Cut_Under_Trampoline_Function();
    }

    if (Menu_Mode_Selection == 4) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Test Mower Menu");
        Serial.println(F("Test Menu Selected"));
        Menu_Mode_Selection = 0;
        delay(3000);
        robot.lcdDisplay.clear();
        Print_Membrane_Switch_Input_Tests();
    }

    if (Menu_Mode_Selection == 5) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Setup Mower");
        Serial.println(F("Mower Setup Selected"));
        Menu_Mode_Selection = 0;
        delay(3000);
        robot.lcdDisplay.clear();
        Print_Membrane_Switch_Input_Settings();
    }

    if (Menu_Mode_Selection == 6) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Slot 6 - Empty");
        Serial.println(F("Slot 6 Selected"));
        Menu_Mode_Selection = 0;
        delay(3000);
        robot.lcdDisplay.clear();
        // Insert Function Here();
    }
}

void Check_Membrane_Keys_Running() {
    Read_Membrane_Keys();
    if (!Stop_Key_X) {
        manouverParkTheMower();
        checkMembraneSwitchInputParked();
        Menu_Mode_Selection = 0;
    }
}

// Test to displyed on the LCD screen when using the membrane key menus

// BUTTONS TIMING MENU

void Print_Membrane_Switch_Input_Timing() {
    //Menu Options if the Mower is Timing.
    Read_Membrane_Keys();
    Menu_Complete = 1;
    Menu_Mode_Selection = 0;
    Menu_View = 0;

    Serial.println();
    Serial.println(F("Test Menu Activated"));
    Menu_Complete = false;  // Menu complete will return to the normal loop
    robot.lcdDisplay.clear();
    delay(5);

    while (Menu_Complete == false) {  // holds the program in a loop until a selection has been made in the membrane button menu
        if (Menu_View == 0) {
            robot.lcdDisplay.setCursor(2, 0);
            Print_LCD_Menu_Timing(1);
            robot.lcdDisplay.setCursor(2, 1);
            Print_LCD_Menu_Timing(2);
        }
        // Gets the values again from the keys
        Read_Membrane_Keys();
        delay(100);

        if (!Start_Key_X) {
            Menu_Complete = true;
            Serial.println(F("Start key is pressed"));
            robot.lcdDisplay.clear();
        }
        if (!Plus_Key_X) {
            Serial.println(F("+ key is pressed"));
            Menu_View = Menu_View - 1;
            Run_Menu_Order_Timing();
        }
        if (!Minus_Key_X) {
            Menu_View = Menu_View + 1;
            Run_Menu_Order_Timing();
        }
        if (!Stop_Key_X) {
            Serial.println(F("Stop key is pressed"));
            Menu_Complete = true;
            robot.lcdDisplay.clear();
            robot.lcdDisplay.setCursor(0, 0);
            robot.lcdDisplay.print("Menu Cancelled");
            delay(1000);
            robot.lcdDisplay.clear();
            Menu_Mode_Selection = 0;
        }
    }
    Activate_Menu_Option_Timing();
}

void Run_Menu_Order_Timing() {
    if (Menu_View == 1) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Timing(1);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Timing(2);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 1;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 2) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Timing(2);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Timing(3);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 2;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 3) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Timing(3);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Timing(4);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 3;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 4) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Timing(4);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Timing(5);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 4;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }

    delay(100);
}

void Activate_Menu_Option_Timing() {
    if (Menu_Mode_Selection == 1) {
        // Maximum Mower Timing
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Max Mow");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Selected");
        Serial.println(F("Maximum Mow Time Selected"));
        delay(5000);
        robot.lcdDisplay.clear();
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        Menu_Complete = false;
        // Add actions here
        robot.alarmTimedMowON = 0;
        //Mow_Time_Set = 1;
    }

    if (Menu_Mode_Selection == 2) {
        // 1hr Mowing
        Serial.println(F("1 hr Mow Time Selected"));
        robot.lcdDisplay.clear();
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        Menu_Complete = false;

        // Ations Here
        //TODO RTC
        /*
        Time t = rtc.time();
        robot.alarmTimedMowON = 1;           // Activate the Mow Timer Alarm
        robot.alarmTimedMowHour = t.hr + 1;  // Sets time to 1 hour later.
        robot.alarmTimedMowMinute = t.min;   // Minutes are the same
        */

        // Displays the Finish time on the Serial Monitor
        Serial.print(F("Finish Time set to : "));
        Serial.print(robot.alarmTimedMowHour);
        Serial.print(F(":"));
        if (robot.alarmTimedMowMinute < 10) Serial.print("0");
        Serial.println(robot.alarmTimedMowMinute);

        robot.lcdDisplay.print("1hr Mow Selected");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Ends: ");
        robot.lcdDisplay.print(robot.alarmTimedMowHour);
        robot.lcdDisplay.print(":");
        if (robot.alarmTimedMowMinute < 10) robot.lcdDisplay.print("0");
        robot.lcdDisplay.print(robot.alarmTimedMowMinute);
        //Mow_Time_Set = 1;
        delay(2000);
    }

    if (Menu_Mode_Selection == 3) {
        // 2hr Mowing
        Serial.println(F("2 hr Mow Time Selected"));
        robot.lcdDisplay.clear();
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        Menu_Complete = false;

        // Ations Here
        //TODO RTC
        /*
        Time t = rtc.time();
        robot.alarmTimedMowON = 1;           // Activate the Mow Timer Alarm
        robot.alarmTimedMowHour = t.hr + 2;  // Sets time to 1 hour later.
        robot.alarmTimedMowMinute = t.min;   // Minutes are the same
        */

        // Displays the Finish time on the Serial Monitor
        Serial.print(F("Finish Time set to : "));
        Serial.print(robot.alarmTimedMowHour);
        Serial.print(F(":"));
        if (robot.alarmTimedMowMinute < 10) Serial.print("0");
        Serial.println(robot.alarmTimedMowMinute);

        robot.lcdDisplay.print("2hr Mow Selected");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Ends: ");
        robot.lcdDisplay.print(robot.alarmTimedMowHour);
        robot.lcdDisplay.print(":");
        if (robot.alarmTimedMowMinute < 10) robot.lcdDisplay.print("0");
        robot.lcdDisplay.print(robot.alarmTimedMowMinute);
        //Mow_Time_Set = 1;
        delay(2000);
    }
}

// BUTTONS SETTINGS MENU

// Test to displyed on the LCD screen when using the membrane key menus
void Print_LCD_Menu_Settings(char LCD_Menu_Settings) {
    if (LCD_Menu_Settings == 1) robot.lcdDisplay.print("Alarm 1");
    if (LCD_Menu_Settings == 2) robot.lcdDisplay.print("Alarm 2");
    if (LCD_Menu_Settings == 3) robot.lcdDisplay.print("Alarm 3");
    if (LCD_Menu_Settings == 4) robot.lcdDisplay.print("Wheel Speed LH");
    if (LCD_Menu_Settings == 5) robot.lcdDisplay.print("Wheel Speed RH");
    if (LCD_Menu_Settings == 6) robot.lcdDisplay.print("Blade Speed");
    if (LCD_Menu_Settings == 7) robot.lcdDisplay.print("Compass ON/OFF");
    if (LCD_Menu_Settings == 8) robot.lcdDisplay.print("Tracking PID");
    if (LCD_Menu_Settings == 9) robot.lcdDisplay.print("Set Clock");
    if (LCD_Menu_Settings == 10) robot.lcdDisplay.print("-----");
    if (LCD_Menu_Settings == 11) robot.lcdDisplay.print("CLEAR EEPROM");
    if (LCD_Menu_Settings == 12) robot.lcdDisplay.print("");  // Leave Blank
}

void Print_Membrane_Switch_Input_Settings() {
    //Menu Options if the Mower is Settings.
    Read_Membrane_Keys();
    Menu_Complete = 1;
    Menu_Mode_Selection = 0;
    Menu_View = 0;

    Serial.println();
    Serial.println(F("Settings Menu Activated"));
    Menu_Complete = false;  // Menu complete will return to the normal loop
    robot.lcdDisplay.clear();
    delay(5);

    while (Menu_Complete == false) {  // holds the program in a loop until a selection has been made in the membrane button menu
        if (Menu_View == 0) {
            robot.lcdDisplay.setCursor(2, 0);
            Print_LCD_Menu_Settings(1);
            robot.lcdDisplay.setCursor(2, 1);
            Print_LCD_Menu_Settings(2);
        }
        // Gets the values again from the keys
        Read_Membrane_Keys();
        delay(100);

        if (!Start_Key_X) {
            Menu_Complete = true;
            Serial.println(F("Start key is pressed"));
            robot.lcdDisplay.clear();
            Activate_Menu_Option_Settings();
        }
        if (!Plus_Key_X) {
            Serial.println(F("+ key is pressed"));
            Menu_View = Menu_View - 1;
            Run_Menu_Order_Settings();
        }
        if (!Minus_Key_X) {
            Menu_View = Menu_View + 1;
            Run_Menu_Order_Settings();
        }
        if (!Stop_Key_X) {
            Serial.println(F("Stop key is pressed"));
            Menu_Complete = true;
            robot.lcdDisplay.clear();
            robot.lcdDisplay.setCursor(0, 0);
            robot.lcdDisplay.print("Menu Cancelled");
            delay(1000);
            robot.lcdDisplay.clear();
            Menu_Mode_Selection = 0;
        }
    }
}

void Run_Menu_Order_Settings() {
    if (Menu_View == 1) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Settings(1);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Settings(2);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 1;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 2) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Settings(2);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Settings(3);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 2;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 3) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Settings(3);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Settings(4);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 3;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 4) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Settings(4);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Settings(5);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 4;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 5) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Settings(5);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Settings(6);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 5;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 6) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Settings(6);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Settings(7);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 6;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 7) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Settings(7);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Settings(8);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 7;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 8) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Settings(8);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Settings(9);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 8;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 9) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Settings(9);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Settings(10);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 9;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 10) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Settings(10);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Settings(11);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 10;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 11) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Settings(11);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Settings(12);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 11;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }

    delay(100);
}

void Activate_Menu_Option_Settings() {
    /*
    if (Menu_Mode_Selection == 1) {
        // Alarm 1 Settings
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print("Alarm_1 : ");
        robot.lcdDisplay.print(Alarm_1_Hour);
        robot.lcdDisplay.print(F(":"));
        if (Alarm_1_Minute < 10) robot.lcdDisplay.print("0");
        robot.lcdDisplay.print(Alarm_1_Minute);
        Serial.print("Alarm 1 Status : ");
        Serial.println(Alarm_1_ON);
        robot.lcdDisplay.setCursor(0, 1);
        if (Alarm_1_ON == 1) robot.lcdDisplay.print("Active");
        if (Alarm_1_ON == 0) robot.lcdDisplay.print("OFF");
        Menu_Complete = false;
        delay(500);
        while (Menu_Complete == false) {
            Read_Membrane_Keys();
            delay(100);
            //Enter Code Here to Cycle until stop key is pressed.
            if (!Start_Key_X) {
                Serial.println(F("Settings Saved"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Alarm_1 :");
                robot.lcdDisplay.print(Alarm_1_Hour);
                robot.lcdDisplay.print(F(":"));
                if (Alarm_1_Minute < 10) robot.lcdDisplay.print("0");
                robot.lcdDisplay.print(Alarm_1_Minute);
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("ON  SAVED");
                //Alarm_1_ON = 1;
                delay(2000);
                robot.lcdDisplay.clear();
                Menu_Mode_Selection = 0;
                EEPROM.write(1, 1);
                EEPROM.write(2, Alarm_1_Hour);
                EEPROM.write(3, Alarm_1_Minute);
                EEPROM.write(4, Alarm_1_ON);
            }
            if (!Plus_Key_X) {
                //Alarm_1_Minute = Alarm_1_Minute + 1;
                if (Alarm_1_Minute > 59) {
                    //Alarm_1_Minute = 0;
                    //Alarm_1_Hour = Alarm_1_Hour + 1;
                }
                //if (Alarm_1_Hour > 23) Alarm_1_Hour = 0;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Alarm_1 : ");
                robot.lcdDisplay.print(Alarm_1_Hour);
                robot.lcdDisplay.print(F(":"));
                if (Alarm_1_Minute < 10) robot.lcdDisplay.print("0");
                robot.lcdDisplay.print(Alarm_1_Minute);
            }
            if (!Minus_Key_X) {
               // Alarm_1_Minute = Alarm_1_Minute - 1;
                if (Alarm_1_Minute < 0) {
                    //Alarm_1_Minute = 59;
                    //Alarm_1_Hour = Alarm_1_Hour - 1;
                    //if (Alarm_1_Hour < 0) Alarm_1_Hour = 23;
                }

                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Alarm_1 : ");
                //robot.lcdDisplay.print(Alarm_1_Hour);
                robot.lcdDisplay.print(F(":"));
                if (Alarm_1_Minute < 10) robot.lcdDisplay.print("0");
                robot.lcdDisplay.print(Alarm_1_Minute);
            }
            if (!Stop_Key_X) {
                Serial.println(F("Alarm Cancelled"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Alarm_1 :");
                robot.lcdDisplay.print(Alarm_1_Hour);
                robot.lcdDisplay.print(F(":"));
                //if (Alarm_1_Minute < 10) robot.lcdDisplay.print("0");
                robot.lcdDisplay.print(Alarm_1_Minute);
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("Alarm OFF");
                //Alarm_1_ON = 0;
                delay(2000);
                robot.lcdDisplay.clear();
                Menu_Mode_Selection = 0;
                EEPROM.write(1, 1);           // Show the Alarm has been saved to the EEPROM
                EEPROM.write(4, Alarm_1_ON);  // Saves that the alarm has been cancelled.
            }
        }
        Serial.print("Alarm 1 Status : ");
        Serial.println(Alarm_1_ON);
        delay(1000);
    }

    if (Menu_Mode_Selection == 2) {
        // Alarm 2 Settings
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print("Alarm_2 : ");
        robot.lcdDisplay.print(Alarm_2_Hour);
        robot.lcdDisplay.print(F(":"));
        if (Alarm_2_Minute < 10) robot.lcdDisplay.print("0");
        robot.lcdDisplay.print(Alarm_2_Minute);
        Serial.print("Alarm 2 Status : ");
        Serial.println(Alarm_2_ON);
        robot.lcdDisplay.setCursor(0, 1);
        if (Alarm_2_ON == 1) robot.lcdDisplay.print("Active");
        if (Alarm_2_ON == 0) robot.lcdDisplay.print("OFF");
        Menu_Complete = false;
        delay(500);
        while (Menu_Complete == false) {
            Read_Membrane_Keys();
            delay(100);
            //Enter Code Here to Cycle until stop key is pressed.
            if (!Start_Key_X) {
                Serial.println(F("Settings Saved"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Alarm_2 :");
                robot.lcdDisplay.print(Alarm_2_Hour);
                robot.lcdDisplay.print(F(":"));
                if (Alarm_2_Minute < 10) robot.lcdDisplay.print("0");
                robot.lcdDisplay.print(Alarm_2_Minute);
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("ON  SAVED");
                //Alarm_2_ON = 1;
                delay(2000);
                robot.lcdDisplay.clear();
                Menu_Mode_Selection = 0;
                EEPROM.write(5, 1);
                EEPROM.write(6, Alarm_2_Hour);
                EEPROM.write(7, Alarm_2_Minute);
                EEPROM.write(8, Alarm_2_ON);
            }
            if (!Plus_Key_X) {
               // Alarm_2_Minute = Alarm_2_Minute + 1;
                if (Alarm_2_Minute > 59) {
                    //Alarm_2_Minute = 0;
                    //Alarm_2_Hour = Alarm_2_Hour + 1;
                }
                //if (Alarm_2_Hour > 23) Alarm_2_Hour = 0;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Alarm_2 : ");
                robot.lcdDisplay.print(Alarm_2_Hour);
                robot.lcdDisplay.print(F(":"));
                if (Alarm_2_Minute < 10) robot.lcdDisplay.print("0");
                robot.lcdDisplay.print(Alarm_2_Minute);
            }
            if (!Minus_Key_X) {
              //  Alarm_2_Minute = Alarm_2_Minute - 1;
                if (Alarm_2_Minute < 0) {
                   // Alarm_2_Minute = 59;
                    //Alarm_2_Hour = Alarm_2_Hour - 1;
                    //if (Alarm_2_Hour < 0) Alarm_2_Hour = 23;
                }

                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Alarm_2 : ");
                robot.lcdDisplay.print(Alarm_2_Hour);
                robot.lcdDisplay.print(F(":"));
                if (Alarm_2_Minute < 10) robot.lcdDisplay.print("0");
                robot.lcdDisplay.print(Alarm_2_Minute);
            }
            if (!Stop_Key_X) {
                Serial.println(F("Alarm Cancelled"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Alarm_2 :");
                robot.lcdDisplay.print(Alarm_2_Hour);
                robot.lcdDisplay.print(F(":"));
                if (Alarm_2_Minute < 10) robot.lcdDisplay.print("0");
                robot.lcdDisplay.print(Alarm_2_Minute);
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("Alarm OFF");
                //Alarm_2_ON = 0;
                delay(2000);
                robot.lcdDisplay.clear();
                Menu_Mode_Selection = 0;
                EEPROM.write(5, 1);
                EEPROM.write(8, Alarm_2_ON);
            }
        }
        Serial.print("Alarm 2 Status : ");
        Serial.println(Alarm_2_ON);
        delay(1000);
    }

    if (Menu_Mode_Selection == 3) {
        // Alarm 3 Settings
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print("Alarm_3 : ");
        robot.lcdDisplay.print(Alarm_3_Hour);
        robot.lcdDisplay.print(F(":"));
        if (Alarm_3_Minute < 10) robot.lcdDisplay.print("0");
        robot.lcdDisplay.print(Alarm_3_Minute);
        Serial.print("Alarm 3 Status : ");
        Serial.println(Alarm_3_ON);
        robot.lcdDisplay.setCursor(0, 1);
        if (Alarm_3_ON == 1) robot.lcdDisplay.print("Active");
        if (Alarm_3_ON == 0) robot.lcdDisplay.print("OFF");
        Menu_Complete = false;
        delay(500);
        while (Menu_Complete == false) {
            Read_Membrane_Keys();
            delay(100);
            //Enter Code Here to Cycle until stop key is pressed.
            if (!Start_Key_X) {
                Serial.println(F("Settings Saved"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Alarm_3 :");
                robot.lcdDisplay.print(Alarm_3_Hour);
                robot.lcdDisplay.print(F(":"));
                if (Alarm_3_Minute < 10) robot.lcdDisplay.print("0");
                robot.lcdDisplay.print(Alarm_3_Minute);
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("ON  SAVED");
               // Alarm_3_ON = 1;
                delay(2000);
                robot.lcdDisplay.clear();
                Menu_Mode_Selection = 0;
                EEPROM.write(9, 1);
                EEPROM.write(10, Alarm_3_Hour);
                EEPROM.write(11, Alarm_3_Minute);
                EEPROM.write(12, Alarm_3_ON);
            }
            if (!Plus_Key_X) {
                //Alarm_3_Minute = Alarm_3_Minute + 1;
                if (Alarm_3_Minute > 59) {
                    //Alarm_3_Minute = 0;
                    //Alarm_3_Hour = Alarm_3_Hour + 1;
                }
                //if (Alarm_3_Hour > 23) Alarm_3_Hour = 0;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Alarm_3 : ");
                robot.lcdDisplay.print(Alarm_3_Hour);
                robot.lcdDisplay.print(F(":"));
                if (Alarm_3_Minute < 10) robot.lcdDisplay.print("0");
                robot.lcdDisplay.print(Alarm_3_Minute);
            }
            if (!Minus_Key_X) {
                //Alarm_3_Minute = Alarm_3_Minute - 1;
                if (Alarm_3_Minute < 0) {
                   // Alarm_3_Minute = 59;
                   // Alarm_3_Hour = Alarm_3_Hour - 1;
                   // if (Alarm_3_Hour < 0) Alarm_3_Hour = 23;
                }

                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Alarm_3 : ");
                robot.lcdDisplay.print(Alarm_3_Hour);
                robot.lcdDisplay.print(F(":"));
                if (Alarm_3_Minute < 10) robot.lcdDisplay.print("0");
                robot.lcdDisplay.print(Alarm_3_Minute);
            }

            if (!Stop_Key_X) {
                Serial.println(F("Alarm Cancelled"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Alarm_3 :");
                robot.lcdDisplay.print(Alarm_3_Hour);
                robot.lcdDisplay.print(F(":"));
                if (Alarm_3_Minute < 10) robot.lcdDisplay.print("0");
                robot.lcdDisplay.print(Alarm_3_Minute);
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("Alarm OFF");
                //Alarm_3_ON = 0;
                delay(2000);
                robot.lcdDisplay.clear();
                Menu_Mode_Selection = 0;
                EEPROM.write(9, 1);
                EEPROM.write(12, Alarm_3_ON);
            }
        }
        Serial.print("Alarm 3 Status : ");
        Serial.println(Alarm_3_ON);
        delay(1000);
    }
*/
    if (Menu_Mode_Selection == 4) {
        // Wheel PWM Settings
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        delay(100);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print("Wheel Speed LH:");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print(PWM_MAX_SPEED_LH);
        Serial.print(F("PWM_L:"));
        Serial.println(PWM_MAX_SPEED_LH);
        Menu_Complete = false;
        while (Menu_Complete == false) {
            Read_Membrane_Keys();
            delay(100);
            //Enter Code Here to Cycle until stop key is pressed.
            if (!Start_Key_X) {
                Serial.println(F("Settings Saved"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("PWM_L:");
                robot.lcdDisplay.print(PWM_MAX_SPEED_LH);
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("SAVED");
                delay(2000);
                robot.lcdDisplay.clear();

                Menu_Mode_Selection = 0;
            }
            if (!Plus_Key_X) {
                //TODO WTF? PWM_MaxSpeed_LH = PWM_MaxSpeed_LH + 1;
                //TODO WTF? if (PWM_MaxSpeed_LH > 255) PWM_MaxSpeed_LH = 255;
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("      ");  // Fully clear the number to stop reminants of a previous number from being left behind
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print(PWM_MAX_SPEED_LH);
                Serial.print(F("PWM_L:"));
                Serial.println(PWM_MAX_SPEED_LH);
            }
            if (!Minus_Key_X) {
                //TODO WTF? PWM_MaxSpeed_LH = PWM_MaxSpeed_LH - 1;
                //TODO WTF? if (PWM_MaxSpeed_LH < 0) PWM_MaxSpeed_LH = 0;
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("      ");  // Fully clear the number to stop reminants of a previous number from being left behind
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print(PWM_MAX_SPEED_LH);
                Serial.print(F("PWM_L:"));
                Serial.println(PWM_MAX_SPEED_LH);
            }
        }
    }

    if (Menu_Mode_Selection == 5) {
        // Wheel RH PWM Settings
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        delay(100);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print("Wheel Speed RH:");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print(PWM_MAX_SPEED_RH);
        Serial.print(F("PWM_R:"));
        Serial.println(PWM_MAX_SPEED_RH);
        Menu_Complete = false;
        while (Menu_Complete == false) {
            Read_Membrane_Keys();
            delay(100);
            //Enter Code Here to Cycle until stop key is pressed.
            if (!Start_Key_X) {
                Serial.println(F("Settings Saved"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("PWM_R:");
                robot.lcdDisplay.print(PWM_MAX_SPEED_RH);
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("SAVED");
                delay(2000);
                robot.lcdDisplay.clear();
                EEPROM.write(15, 1);
                EEPROM.write(16, PWM_MAX_SPEED_RH);
                Menu_Mode_Selection = 0;
            }
            if (!Plus_Key_X) {
                //PWM_MaxSpeed_RH = PWM_MaxSpeed_RH + 1;
                //if (PWM_MaxSpeed_RH > 255) PWM_MaxSpeed_RH = 255;
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("      ");  // Fully clear the number to stop reminants of a previous number from being left behind
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print(PWM_MAX_SPEED_RH);
                Serial.print(F("PWM_R:"));
                Serial.println(PWM_MAX_SPEED_RH);
            }
            if (!Minus_Key_X) {
                //PWM_MaxSpeed_RH = PWM_MaxSpeed_RH - 1;
                //if (PWM_MaxSpeed_RH < 0) PWM_MaxSpeed_RH = 0;
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("      ");  // Fully clear the number to stop reminants of a previous number from being left behind
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print(PWM_MAX_SPEED_RH);
                Serial.print(F("PWM_R:"));
                Serial.println(PWM_MAX_SPEED_RH);
            }
        }
    }

    if (Menu_Mode_Selection == 6) {
        /*
        // Blade PWM Settings
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        delay(500);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print("Blade Speed:");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print(PWM_Blade_Speed);
        Serial.print(F("Blade PWM:"));
        Serial.println(PWM_Blade_Speed);
        Menu_Complete = false;
        while (Menu_Complete == false) {
            Read_Membrane_Keys();
            delay(100);
            //Enter Code Here to Cycle until stop key is pressed.
            if (!Start_Key_X) {
                Serial.println(F("Settings Saved"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Blade PWM:");
                robot.lcdDisplay.print(PWM_Blade_Speed);
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("SAVED");
                delay(2000);
                robot.lcdDisplay.clear();
                EEPROM.write(17, 1);
                EEPROM.write(18, PWM_Blade_Speed);
                Menu_Mode_Selection = 0;
            }
            if (!Plus_Key_X) {
               // PWM_Blade_Speed = PWM_Blade_Speed + 1;
                //if (PWM_Blade_Speed > 255) PWM_Blade_Speed = 255;
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("      ");  // Fully clear the number to stop reminants of a previous number from being left behind
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print(PWM_Blade_Speed);
                Serial.print(F("Blade PWM:"));
                Serial.println(PWM_Blade_Speed);
            }
            if (!Minus_Key_X) {
                //PWM_Blade_Speed = PWM_Blade_Speed - 1;
                //if (PWM_Blade_Speed < 0) PWM_Blade_Speed = 0;
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("      ");  // Fully clear the number to stop reminants of a previous number from being left behind
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print(PWM_Blade_Speed);
                Serial.print(F("Blade PWM:"));
                Serial.println(PWM_Blade_Speed);
            }
        }
        */
    }

    if (Menu_Mode_Selection == 7) {
        // Compass setup
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Compass Setup");
        delay(1000);
        robot.lcdDisplay.clear();
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print("Compass ON/OFF");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Status : ");
        if (COMPASS_ACTIVATE == 1) robot.lcdDisplay.print("ON ");
        if (COMPASS_ACTIVATE == 0) robot.lcdDisplay.print("OFF");

        Menu_Complete = false;
        while (Menu_Complete == false) {
            Read_Membrane_Keys();
            delay(100);
            //Enter Code Here to Cycle until stop key is pressed.
            if (!Start_Key_X) {
                Serial.println(F("Compass Settings Saved"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Compass Saved");
                Serial.print(F("Compass:"));
                Serial.println(COMPASS_ACTIVATE);
                delay(2000);
                robot.lcdDisplay.clear();
                EEPROM.write(19, 1);
                EEPROM.write(20, COMPASS_ACTIVATE);
                if (COMPASS_ACTIVATE == 1) Setup_Compass();
                Menu_Mode_Selection = 0;
            }
            if (!Plus_Key_X) {
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("Status : ");
                // COMPASS_ACTIVATE = 1;
                robot.lcdDisplay.print("ON ");
                Serial.print(F("Compass:"));
                Serial.println(COMPASS_ACTIVATE);
                delay(100);
            }
            if (!Minus_Key_X) {
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("Status : ");
                // COMPASS_ACTIVATE = 0;
                robot.lcdDisplay.print("OFF");
                Serial.print(F("Compass:"));
                Serial.println(COMPASS_ACTIVATE);
                delay(100);
            }
        }
    }

    if (Menu_Mode_Selection == 8) {
        // Tracking PID Settings
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        delay(500);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print("Tracking PID:");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("P = ");
        robot.lcdDisplay.print(robot.P);
        Serial.print(F("Tracking PID P = :"));
        Serial.println(robot.P);
        Menu_Complete = false;
        while (Menu_Complete == false) {
            Read_Membrane_Keys();
            delay(100);
            //Enter Code Here to Cycle until stop key is pressed.
            if (!Start_Key_X) {
                Serial.println(F("Settings Saved"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("P : ");
                robot.lcdDisplay.print(robot.P);
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("SAVED");
                delay(2000);
                robot.lcdDisplay.clear();
                EEPROM.write(21, 1);
                EEPROM.write(22, (robot.P * 100));
                Menu_Mode_Selection = 0;
            }
            if (!Plus_Key_X) {
                robot.P = robot.P + 0.01;
                if (robot.P > 10) robot.P = 10;
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("      ");  // Fully clear the number to stop reminants of a previous number from being left behind
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("P : ");
                robot.lcdDisplay.print(robot.P);
                Serial.print(F("Tracking PID P = :"));
                Serial.println(robot.P);
            }
            if (!Minus_Key_X) {
                robot.P = robot.P - 0.01;
                if (robot.P < 0) robot.P = 0;
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("      ");  // Fully clear the number to stop reminants of a previous number from being left behind
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("P : ");
                robot.lcdDisplay.print(robot.P);
                Serial.print(F("Tracking PID P = :"));
                Serial.println(robot.P);
            }
        }
    }

    // Set Mower CLock

    if (Menu_Mode_Selection == 9) {
        // Set Mower Clock
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(0, 0);  // Spaces to the right & down
        /*
        Time t = rtc.time();
        int set_hour = t.hr;
        int set_min = t.min;
        robot.lcdDisplay.print("Time : ");
        robot.lcdDisplay.print(set_hour);
        robot.lcdDisplay.print(":");
        if (set_min < 10) robot.lcdDisplay.print("0");
        robot.lcdDisplay.print(set_min);
        Serial.print("Time : ");
        Serial.print(set_hour);
        Serial.print(":");
        if (set_min < 10) Serial.print("0");
        Serial.println(set_min);

        Menu_Complete = false;
        delay(500);
        while (Menu_Complete == false) {
            Read_Membrane_Keys();
            delay(100);
            //Enter Code Here to Cycle until stop key is pressed.
            if (!Start_Key_X) {
                Serial.println(F("Settings Saved"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.print("Time : ");
                robot.lcdDisplay.print(set_hour);
                robot.lcdDisplay.print(":");
                if (set_min < 10) robot.lcdDisplay.print("0");
                robot.lcdDisplay.print(set_min);
                robot.lcdDisplay.setCursor(0, 1);
                rtc.writeProtect(false);
                rtc.halt(false);
                Serial.print("Clock : ");
                Serial.print(set_hour);
                Serial.print(":");
                if (set_min < 10) Serial.print("0");
                Serial.println(set_min);
                Time t(2019, 07, 19, set_hour, set_min, 00, Time::kFriday);  // Year XXXX, Month XX, Day XX, Hour XX, Minute XX, Second, kXYZday
                rtc.time(t);
                delay(200);
                rtc.writeProtect(true);
                rtc.halt(true);
                rtc.time(t);
                robot.lcdDisplay.print("TIME SAVED");
                delay(2000);
                robot.lcdDisplay.clear();
                Menu_Mode_Selection = 0;
            }
            if (!Plus_Key_X) {
                set_min = set_min + 1;
                if (set_min > 59) {
                    set_min = 0;
                    set_hour = set_hour + 1;
                }
                if (set_hour > 23) set_hour = 0;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.print("Time : ");
                robot.lcdDisplay.print(t.hr);
                robot.lcdDisplay.print(":");
                if (set_min < 10) robot.lcdDisplay.print("0");
                robot.lcdDisplay.print(set_min);
            }
            if (!Minus_Key_X) {
                set_min = set_min - 1;
                if (set_min < 0) {
                    set_min = 59;
                    set_hour = set_hour - 1;
                }
                if (set_hour < 0) set_hour = 23;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.print("Time : ");
                robot.lcdDisplay.print(set_hour);
                robot.lcdDisplay.print(":");
                if (set_min < 10) robot.lcdDisplay.print("0");
                robot.lcdDisplay.print(set_min);
            }
            if (!Stop_Key_X) {
                Serial.println(F("Time Set Cancelled"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Time Set");
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("Cancelled");
                delay(2000);
                robot.lcdDisplay.clear();
                Menu_Mode_Selection = 0;
            }
        }
        Serial.print("Time : ");
        delay(1000);
        */
    }

    if (Menu_Mode_Selection == 10) {
        // Enter Settings Here
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("..........");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Activated");
        Serial.println(F("...................."));
        delay(5000);
        robot.lcdDisplay.clear();
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        Menu_Complete = false;
        while (Menu_Complete == false) {
            //Enter Code Here to Cycle until stop key is pressed.
            if (!Stop_Key_X) {
                Serial.println(F("Stop key is pressed"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Test Stopped");
                delay(2000);
                robot.lcdDisplay.clear();
                Menu_Mode_Selection = 0;
            }
        }
    }

    // Menu Option to CLEAR EEPROM
    //-----------------------------

    if (Menu_Mode_Selection == 11) {
        // EEPORM Clear MENU
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("EEPROM Clear ?");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Yes/No");
        Serial.println(F("Clear EEPROM Yes/No   - Press Up for Yes and Down for No.  Start to Confirm"));
        Menu_Mode_Selection = 0;
        Menu_Complete = false;
        int Answer = 0;
        while (Menu_Complete == false) {
            //Enter Code Here to Cycle until stop key is pressed.
            Read_Membrane_Keys();
            delay(100);
            if (!Start_Key_X) {
                Serial.println(F("Stop key is pressed"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                if (Answer == 1) {
                    robot.lcdDisplay.clear();
                    robot.lcdDisplay.print("EEPROM Cleared");
                    robot.lcdDisplay.setCursor(0, 1);
                    robot.lcdDisplay.print("Power OFF Mower");
                }
                if (Answer == 0) {
                    robot.lcdDisplay.clear();
                    robot.lcdDisplay.print("Cancelled");
                    robot.lcdDisplay.setCursor(0, 1);
                }
                delay(2000);
                robot.lcdDisplay.clear();
                Menu_Mode_Selection = 0;
            }
            if (!Plus_Key_X) {
                Answer = 1;
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("Yes    ");
                Serial.println(F("Clear EEPROM = YES....  Press Start to Confirm"));
                delay(100);
            }
            if (!Minus_Key_X) {
                Answer = 0;
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("No     ");
                Serial.println(F("Clear EEPROM = No....Press Start to Confirm"));
                delay(100);
            }
        }
    }
    Print_Membrane_Switch_Input_Settings();
}

// BUTTONS TEST MENU

// Test to displyed on the LCD screen when using the membrane key menus
void Print_LCD_Menu_Tests(char LCD_Menu_Tests) {
    if (LCD_Menu_Tests == 1) robot.lcdDisplay.print("Wire Test");
    if (LCD_Menu_Tests == 2) robot.lcdDisplay.print("Relay Test");
    if (LCD_Menu_Tests == 3) robot.lcdDisplay.print("Wheel Test");
    if (LCD_Menu_Tests == 4) robot.lcdDisplay.print("Blade Test");
    if (LCD_Menu_Tests == 5) robot.lcdDisplay.print("Sonar Test");
    if (LCD_Menu_Tests == 6) robot.lcdDisplay.print("Turn Test");
    if (LCD_Menu_Tests == 7) robot.lcdDisplay.print("Volt Amp Test");
    if (LCD_Menu_Tests == 8) robot.lcdDisplay.print("Compass Test");
    if (LCD_Menu_Tests == 9) robot.lcdDisplay.print("-  ");
    if (LCD_Menu_Tests == 10) robot.lcdDisplay.print("-  ");
    if (LCD_Menu_Tests == 11) robot.lcdDisplay.print("-  ");
    if (LCD_Menu_Tests == 12) robot.lcdDisplay.print("");  // Leave Blank
}

void Print_Membrane_Switch_Input_Tests() {
    //Menu Options if the Mower is Tests.
    Read_Membrane_Keys();
    Menu_Complete = 1;
    Menu_Mode_Selection = 0;
    Menu_View = 0;

    Serial.println();
    Serial.println(F("Test Menu Activated"));
    Menu_Complete = false;  // Menu complete will return to the normal loop
    robot.lcdDisplay.clear();
    delay(5);

    while (Menu_Complete == false) {  // holds the program in a loop until a selection has been made in the membrane button menu
        if (Menu_View == 0) {
            robot.lcdDisplay.setCursor(2, 0);
            Print_LCD_Menu_Tests(1);
            robot.lcdDisplay.setCursor(2, 1);
            Print_LCD_Menu_Tests(2);
        }
        // Gets the values again from the keys
        Read_Membrane_Keys();
        delay(100);

        if (!Start_Key_X) {
            Menu_Complete = true;
            Serial.println(F("Start key is pressed"));
            robot.lcdDisplay.clear();
        }
        if (!Plus_Key_X) {
            Serial.println(F("+ key is pressed"));
            Menu_View = Menu_View - 1;
            Run_Menu_Order_Testing();
        }
        if (!Minus_Key_X) {
            Menu_View = Menu_View + 1;
            Run_Menu_Order_Testing();
        }
        if (!Stop_Key_X) {
            Serial.println(F("Stop key is pressed"));
            Menu_Complete = true;
            robot.lcdDisplay.clear();
            robot.lcdDisplay.setCursor(0, 0);
            robot.lcdDisplay.print("Menu Cancelled");
            delay(1000);
            robot.lcdDisplay.clear();
            Menu_Mode_Selection = 0;
        }
    }
    Activate_Menu_Option_Testing();
}

void Run_Menu_Order_Testing() {
    if (Menu_View == 1) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(1);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(2);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 1;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 2) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(2);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(3);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 2;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 3) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(3);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(4);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 3;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 4) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(4);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(5);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 4;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 5) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(5);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(6);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 5;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 6) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(6);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(7);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 6;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 7) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(7);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(8);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 7;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 8) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(8);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(9);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 8;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 9) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(9);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(10);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 9;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 10) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(10);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(11);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 10;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }
    if (Menu_View == 11) {
        Serial.print(F("- key is pressed "));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(2, 0);
        Print_LCD_Menu_Tests(11);
        robot.lcdDisplay.setCursor(2, 1);
        Print_LCD_Menu_Tests(12);
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print(">");
        Menu_Mode_Selection = 10;
        Serial.print(F("Menu View : "));
        Serial.print(Menu_View);
        Serial.print(F("| Menu Selection"));
        Serial.println(Menu_Mode_Selection);
        delay(100);
    }

    delay(100);
}

void Activate_Menu_Option_Testing() {
    if (Menu_Mode_Selection == 1) {
        // Perimeter Wire Test
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Wire Test");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Activated");
        Serial.println(F("Perimeter Wire Test Started"));
        delay(5000);
        robot.lcdDisplay.clear();
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        Menu_Complete = false;
        while (Menu_Complete == false) {
            Test_Mower_Check_Wire();
            Read_Membrane_Keys();
            if (!Stop_Key_X) {
                Serial.println(F("Stop key is pressed"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Test Stopped");
                delay(2000);
                robot.lcdDisplay.clear();
                Menu_Mode_Selection = 0;
            }
        }
    }

    if (Menu_Mode_Selection == 2) {
        // Relay Test
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Relay Test");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Activated");
        delay(1000);
        robot.lcdDisplay.clear();
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        Menu_Complete = false;
        while (Menu_Complete == false) {
            Test_Relay();
            Read_Membrane_Keys();
            if (!Stop_Key_X) {
                Serial.println(F("Stop key is pressed"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Test Stopped");
                delay(2000);
                robot.lcdDisplay.clear();
                Menu_Mode_Selection = 0;
            }
        }
    }

    if (Menu_Mode_Selection == 3) {
        // Mower Motor Test
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Wheel Motor Test");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Activated");
        delay(1000);
        robot.lcdDisplay.clear();
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        robot.I = 1;          // sets the itteration for the motor test
        Test_Wheel_Motors();  // starts the mowe motor test 1x
    }

    if (Menu_Mode_Selection == 4) {
        // Blade Motor Test
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Blade Motor Test");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Activated");
        delay(1000);
        robot.lcdDisplay.clear();
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        Test_Mower_Blade_Motor();
    }

    if (Menu_Mode_Selection == 5) {
        Menu_Mode_Selection = 0;
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Sonar Test");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Activated");
        delay(1000);
        robot.lcdDisplay.clear();
        Menu_Complete = false;
        while (Menu_Complete == false) {
            Test_Sonar_Array();
            Read_Membrane_Keys();
            if (!Stop_Key_X) {
                Serial.println(F("Stop key is pressed"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Test Stopped");
                delay(2000);
                robot.lcdDisplay.clear();
                Menu_Mode_Selection = 0;
            }
        }
    }

    if (Menu_Mode_Selection == 6) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Pattern Mow");
        Serial.println(F("Slot 6 Selected"));
        Menu_Mode_Selection = 0;
        delay(3000);
        robot.lcdDisplay.clear();
        Test_Compass_Turn_Function();
    }

    if (Menu_Mode_Selection == 7) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Volt robot.amps Test");
        Serial.println(F("robot.volts and robot.amps Test"));
        Menu_Mode_Selection = 0;
        delay(1000);
        robot.lcdDisplay.clear();
        Menu_Complete = false;
        while (Menu_Complete == false) {
            readVoltAmp();
            delay(100);
            robot.lcdDisplay.setCursor(0, 0);
            robot.lcdDisplay.print("Volt:");
            robot.lcdDisplay.print(robot.volts);
            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.print("robot.amps:");
            robot.lcdDisplay.print(robot.amps);
            Serial.println("");

            if (!Stop_Key_X) {
                Serial.println(F("Stop key is pressed"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Test Stopped");
                delay(2000);
                robot.lcdDisplay.clear();
                Menu_Mode_Selection = 0;
            }
        }
    }
    if (Menu_Mode_Selection == 8) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Compass Test");
        Serial.println(F("Compass Test Selected"));
        Menu_Mode_Selection = 0;
        delay(3000);
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print("robot.heading:");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Degrees:");
        Menu_Complete = false;
        while (Menu_Complete == false) {
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

            if (!Stop_Key_X) {
                Serial.println(F("Stop key is pressed"));
                Menu_Complete = true;
                robot.lcdDisplay.clear();
                robot.lcdDisplay.setCursor(0, 0);
                robot.lcdDisplay.print("Test Stopped");
                delay(2000);
                robot.lcdDisplay.clear();
                Menu_Mode_Selection = 0;
            }
        }
    }

    if (Menu_Mode_Selection == 9) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Spare 9");
        Serial.println(F("Slot 9 Selected"));
        Menu_Mode_Selection = 0;
        delay(3000);
        robot.lcdDisplay.clear();
    }
    if (Menu_Mode_Selection == 10) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Spare 10");
        Serial.println(F("Slot 10 Selected"));
        Menu_Mode_Selection = 0;
        delay(3000);
        robot.lcdDisplay.clear();
    }
}
