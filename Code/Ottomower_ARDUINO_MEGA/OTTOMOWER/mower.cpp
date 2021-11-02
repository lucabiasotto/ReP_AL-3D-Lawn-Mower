#include "mower.h"

#include "adcman.h"
#include "batteryUtils.h"
#include "compassUtils.h"
#include "lcdDisplay.h"
#include "menuButtons.h"
#include "motorsController.h"
#include "movementsUtils.h"
#include "perimeter.h"
#include "rainUtils.h"
#include "sonar.h"
#include "wireDetection.h"

Mower robot;

Mower::Mower() {}

void Mower::setup() {
    //Open Serial port 1 for the nano communication
    //2021-10-23 disattivato nano Serial1.begin(1200);

    Console.println(" ");
    Console.println(" ");
    Console.print("Ottomower Robot :");
    Console.println(Version);
    Console.println("===================");
    Console.println("");
    Console.println("Starting Mower Setup");
    Console.println("===================");

    //TODO carica dati da eepronm Load_EEPROM_Saved_Data();
    /*
    TODO ora non gestita
    if (SET_TIME) {
        Console.print("Setting Time");
        Set_Time_On_RTC();
    }
    DisplayTime();
    Console.println("");
    */

    //Logia per gestione cavo manuale
    //pinMode(USE_CABLE_PIN, INPUT);
    //settings.isPerimeterWireEnabled = digitalRead(USE_CABLE_PIN);
    Console.print("Use cable: ");
    Console.println(PERIMETER_WIRE_ENABLED);

    Prepare_Mower_from_Settings();
    Setup_Run_LCD_Intro();
    Setup_Relays();
    Setup_Membrane_Buttons();
    Setup_Motor_Pins();
    Setup_Compass();
    Setup_ADCMan();
}

void Mower::loop() {
    readVoltAmp();     //update volt and amp info
    readRainSensor();  // Checks if the water sensor detects Rain

    robot.logMowerStatus();  // Update the Serial monitor with the current mower status.

    lcdUpdateScreen();  //Update LCD Info

    if (robot.mowerDocked == 1) {
        /*********************************************************
         * Mower is docked waiting for a command to leave and mow.
         *********************************************************/
        checkMembraneSwitchInputDocked();  // Check the membrane buttons for any input

    } else if (robot.mowerParked == 1 || robot.mowerParkedLowBatt == 1 || robot.mowerError == 1) {
        /*********************************************************
         *  Mower is Parked ready to be started / re-started.
         * Mower is Parked with Low Battery needing manuel charging.
         * Lost mower is put into standby mode.
         *********************************************************/
        checkMembraneSwitchInputParked();  // Check the membrane buttons for any input

    } else if (robot.mowerParked == 1) {
        /*********************************************************
         * Mower is parked
         *********************************************************/
        manouverParkTheMower();

    } else if (robot.mowerRunning == 1) {
        /*********************************************************
         * Mower is running cutting the grass.
         *********************************************************/
        bool wireOn = isWireOn();

        //TODO perchè non processare sempre i volt? forse solo quando gira perchè deve fermare
        Process_Volt_Information();     // Take action based on the voltage readings
        checkMembranSwitchRunning();  // Check to see if the mower needs to be stopped via keypad
        //TODOCheck_Timed_Mow();              // Check to see if the time to go home has come.

        if (wireOn) {
            Check_Wire_In_Out();  // Test if the mower is in or out of the wire fence.

            if (robot.outsideWire == 0) {
                //in inside perimeter
                readSonarSensor();  // If the mower is  the boundary wire check the sonars for obsticles and prints to the LCD
                if (robot.sonarHit == 0) {
                    robotMoveAroundTheGarden();  // Inputs to the wheel motors / blade motors according to surroundings
                } else {
                    Manouver_Turn_Around_Sonar();  // If sonar hit is detected and mower is  the wire, manouver around obsticle
                }

            } else {
                //Is outside!!!
                robotReverseDirection();  // If outside the wire turn around
            }
        } else if (robot.rainHitDetected >= RAIN_TOTAL_HITS_GO_HOME) {
            /*********************************************************
             * It's rain!!
             *********************************************************/
            Serial.println();
            Serial.println("Its raining!");
            Motor_Action_Stop_Motors();
            delay(2000);
            if (USE_CHARGING_STATION == 1) {
                // Stops the mowing and sends the mower back to the charging station via the permieter wire
                goToChargingStation();
            } else {
                // Parks the mower
            }
        }else if (robot.lowBatteryDetectedCount > LOW_BATTERY_INSTANCES_CHG) {
            /*********************************************************
             * Low Battery Detected!
             *********************************************************/
            Serial.println();
            Serial.println("Low Battery Detected!");
            if (USE_CHARGING_STATION == 1) {
                // Stops the mowing and sends the mower back to the charging station via the permieter wire
                goToChargingStation();
            } else {
                // Parks the mower with a low battery warning
                Manouver_Park_The_Mower_Low_Batt();
            }
        }
    }

    Serial.println();
}

void Mower::logMowerStatus() {
    if (robot.mowerDocked == 1) Serial.print("Docked:1|");
    if (robot.mowerParked == 1) Serial.print("Parked:1|");
    if (robot.mowerRunning == 1) Serial.print("Running:1|");
    if (robot.manuelMode == 1) Serial.print("Manuel Mode:1|");
    if (robot.mowerParkedLowBatt == 1) Serial.print("Park_Low_Batt:1|");
    if (robot.mowerError == 1) Serial.print("Mower Error:1|");
    if (RAIN_SENSOR_INSTALLED == 1) {
        Serial.print("Rain:");
        Serial.print(robot.rainDetected);
        Serial.print("|");

        Serial.print("RainHit:");
        Serial.print(robot.rainHitDetected);
        Serial.print("|");
    }

    Serial.print("VLow:");
    Serial.print(robot.lowBatteryDetectedCount);
    Serial.print("|");

    Serial.print(F("Loop:"));
    Serial.print(robot.loopCycleMowing);
    Serial.print("|");

    Serial.print("Charging:");
    Serial.print(robot.charging);
    Serial.print("|");

    if (robot.mowerDocked == 1) {
        //TODO RTC
        /*
        Serial.print("Time:");
        Time t = rtc.time();
        Serial.print(t.hr);
        Serial.print(":");
        if (t.min < 10) Serial.print("0");
        Serial.print(t.min);
        Serial.print(".");
        if (t.sec < 10) Serial.print("0");
        Serial.print(t.sec);

        //log alarm data
        if (Alarm_1_ON == 1) {
            Serial.print("|Alarm 1:");
            Serial.print(Alarm_1_Hour);
            Serial.print(F(":"));
            if (Alarm_1_Minute < 10) Serial.print("0");
            Serial.print(Alarm_1_Minute);
            Serial.print("|");
        } else {
            Serial.print("|Alarm 1 OFF");
        }

        if (Alarm_2_ON == 1) {
            Serial.print("|Alarm 2:");
            Serial.print(Alarm_2_Hour);
            Serial.print(F(":"));
            if (Alarm_2_Minute < 10) Serial.print("0");
            Serial.print(Alarm_2_Minute);
            Serial.print("|");
        } else {
            Serial.print("|Alarm 2 OFF");
        }

        if (Alarm_3_ON == 1) {
            Serial.print("|Alarm 3:");
            Serial.print(Alarm_3_Hour);
            Serial.print(F(":"));
            if (Alarm_3_Minute < 10) Serial.print("0");
            Serial.print(Alarm_3_Minute);
            Serial.print("|");
        } else {
            Serial.print("|Alarm 3 OFF");
        }
        */
    }
}

void Mower::Setup_Relays() {
    Console.println("Setup Relays");
    pinMode(Relay_Motors, OUTPUT);
    delay(5);
    robot.Turn_Off_Relay();
    delay(5);
}

void Mower::Prepare_Mower_from_Settings() {
    if (USE_CHARGING_STATION == 1) {
        robot.mowerDocked = 1;
        robot.mowerParked = 0;
        robot.mowerRunning = 0;
    }else{
        robot.mowerDocked = 0;
        robot.mowerParked = 1;
        robot.mowerRunning = 0;
    }
}

void Mower::Setup_Motor_Pins() {
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

void Mower::Turn_On_Relay() {
    Serial.print("Relay:ON|");
    digitalWrite(Relay_Motors, LOW);  // Turn of the relay for the main battery power
}

void Mower::Turn_Off_Relay() {
    Serial.print("Relay:Off|");
    digitalWrite(Relay_Motors, HIGH);  // Turn of the relay for the main battery power
}

void Mower::Setup_Membrane_Buttons() {
    Serial.println("Setup Membrane Keys");
    pinMode(Start_Key, INPUT_PULLUP);  // set pin as input
    pinMode(Plus_Key, INPUT_PULLUP);   // set pin as input
    pinMode(Minus_Key, INPUT_PULLUP);  // set pin as input
    pinMode(Stop_Key, INPUT_PULLUP);   // set pin as input
}

void Mower::Setup_ADCMan() {
    Serial.println("ADCMAN");
    ADCMan.init();
    robot.perimeter.setPins(pinPerimeterLeft, pinPerimeterRight);
    robot.perimeter.useDifferentialPerimeterSignal = true;
    robot.perimeter.speedTest();
    ADCMan.setCapture(A2, 1, false);  //TODO copia da ardumower
    ADCMan.run();
}
