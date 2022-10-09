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

    Serial.println(" ");
    Serial.println(" ");
    Serial.print("Ottomower Robot :");
    Serial.println(Version);
    Serial.println("===================");
    Serial.println("");
    Serial.println("Starting Mower Setup");
    Serial.println("===================");
    Serial.print("Use cable: ");
    Serial.println(robot.isPerimeterWireEnable);

    Prepare_Mower_from_Settings();
    lcdInit();
    Setup_Relays();
    Setup_Membrane_Buttons();
    Setup_Motor_Pins();
    setupRainPins();
    compassInit();
    Setup_ADCMan();
    
}

void Mower::loop() {
    Serial.print(">");
    Serial.print(millis());

    ADCMan.run(); //2021-11-26 read analog value for battery and water

    readVoltAmp();  //update volt and amp info
    
    if (robot.mowerRunning == 1) {
        readRainSensor();    // Checks if the water sensor detects Rain
        readSonarSensors();  // If the mower is  the boundary wire check the sonars for obsticles and prints to the LCD
    }
    if (COMPASS_ACTIVATE == 1) {
        readRobotCompassDegrees();  // Gets the latest compass reading
    }

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
        checkMembranSwitchRunning();  // Check user interation to see if the mower needs to be stopped via keypad

        if (robot.rainHitDetected >= RAIN_TOTAL_HITS_GO_HOME) {
            /*********************************************************
             * It's rain!!
             *********************************************************/
            Serial.println();
            Serial.println("Its raining!");
            motorsStopWheelMotors();
            delay(2000);
            if (USE_CHARGING_STATION == 1 && robot.isPerimeterWireEnable) {
                // Stops the mowing and sends the mower back to the charging station via the permieter wire
                goToChargingStation();
            } else {
                // Parks the mower
            }
        } else if (robot.lowBatteryDetectedCount > LOW_BATTERY_INSTANCES_CHG) {
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

                motorsStopWheelMotors();
                motorsStopSpinBlades();
            }
        } else if (robot.SonarHit1Total >= SONAR_MAX_HIT || robot.SonarHit2Total >= SONAR_MAX_HIT || robot.SonarHit3Total >= SONAR_MAX_HIT) {
            /*********************************************************
             * Sonar obstacle
             **********************************************************/
            // If sonar hit is detected and mower is the wire, manouver around obsticle
            avoidSonarObstacle();  //TODO prima lo faceva solo se era dentro al perimetro....
        } else if (robot.isPerimeterWireEnable && isWireOn()) {
            Check_Wire_In_Out();  // Test if the mower is in or out of the wire fence.
            if (robot.outsideWire == 0) {
                //in inside perimeter
                robotMoveAroundTheGarden();  // Inputs to the wheel motors / blade motors according to surroundings
            } else {
                //Is outside!!!
                robotReverseDirection();  // If outside the wire turn around
            }
        }else{
            //isPerimeterWireEnable false, move without calbe
            robotMoveAroundTheGarden();
        }
    }

    Serial.println("<");
}

void Mower::logMowerStatus() {
    if (robot.mowerDocked == 1) {
        Serial.print("Docked:1|");
    }
    if (robot.mowerParked == 1) {
        Serial.print("Parked:1|");
    }
    if (robot.mowerRunning == 1) {
        Serial.print("Running:1|");
    }
    if (robot.mowerParkedLowBatt == 1) {
        Serial.print("Park_Low_Batt:1|");
    }
    if (robot.mowerError == 1) {
        Serial.print("Mower Error:1|");
    }

    Serial.print(F("Loop:"));
    Serial.print(robot.loopCycleMowing);

    Serial.print(F("|V:"));
    Serial.print(robot.volts);

    Serial.print("|VLow:");
    Serial.print(robot.lowBatteryDetectedCount);

    Serial.print(F("|A:"));
    Serial.print(robot.amps);

    if (RAIN_SENSOR_INSTALLED == 1) {
        Serial.print("|Rain:");
        Serial.print(robot.rainDetected);
        Serial.print("|RainHit:");
        Serial.print(robot.rainHitDetected);
    }

    if (SONAR_1_ACTIVATE == 1) {
        //CENTER sonar
        Serial.print(F("|Sonar_1:"));
        Serial.print(robot.distance1);
        Serial.print(F("cm-TotalHit:"));
        Serial.print(robot.SonarHit1Total);
    }

    if (SONAR_2_ACTIVATE == 1) {
        //CENTER sonar
        Serial.print(F("|Sonar_2:"));
        Serial.print(robot.distance2);
        Serial.print(F("cm-TotalHit:"));
        Serial.print(robot.SonarHit2Total);
    }

    if (SONAR_3_ACTIVATE == 1) {
        //CENTER sonar
        Serial.print(F("|Sonar_3:"));
        Serial.print(robot.distance3);
        Serial.print(F("cm-TotalHit:"));
        Serial.print(robot.SonarHit3Total);
    }

    if (COMPASS_ACTIVATE == 1) {
        Serial.print(F("|Comp°:"));
        Serial.print(robot.compassHeadingDegrees);
        Serial.print("|");
    }

    if (robot.mowerDocked == 1) {
        //TODO logga qua lo schedulatore
    }
}

void Mower::Setup_Relays() {
    Serial.println("Setup Relays");
    pinMode(RELAY_MOTORS_PIN, OUTPUT);
    delay(5);
    robot.turnOffMotorsRelay();
    delay(5);
}

void Mower::Prepare_Mower_from_Settings() {
    if (USE_CHARGING_STATION == 1) {
        robot.mowerDocked = 1;
        robot.mowerParked = 0;
        robot.mowerRunning = 0;
    } else {
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

/**
 * Turn of the relay for the main battery power
 */
void Mower::turnOnMotorsRelay() {
    Serial.print("Relay:ON|");
    digitalWrite(RELAY_MOTORS_PIN, LOW);
}

/**
 * Turn of the relay for the main battery power
 */
void Mower::turnOffMotorsRelay() {
    Serial.print("Relay:Off|");
    digitalWrite(RELAY_MOTORS_PIN, HIGH);
}

void Mower::Setup_Membrane_Buttons() {
    Serial.println("Setup Membrane Keys");
    pinMode(SWITCH_OK_KEY_PIN, INPUT_PULLUP);     // set pin as input
    pinMode(SWITCH_PLUS_KEY_PIN, INPUT_PULLUP);   // set pin as input
    pinMode(SWITCH_MINUS_KEY_PIN, INPUT_PULLUP);  // set pin as input
    pinMode(SWITCH_STOP_KEY_PIN, INPUT_PULLUP);   // set pin as input
}

void Mower::Setup_ADCMan() {
    Serial.println("ADCMAN");
    ADCMan.init();

    ADCMan.setCapture(VOLT_PIN, 1, false);  //copiato da ardumower
    if (RAIN_SENSOR_INSTALLED == 1) {
        ADCMan.setCapture(RAIN_PIN, 1, false);  
    }

    robot.perimeter.setPins(pinPerimeterLeft, pinPerimeterRight);
    robot.perimeter.useDifferentialPerimeterSignal = true;
    robot.perimeter.speedTest();
    
    ADCMan.run();
}

void Mower::setupRainPins() {
    /* todo cava
    if (RAIN_SENSOR_INSTALLED == 1) {
        Serial.println("RAIN PIN");    
        pinMode(RAIN_PIN, INPUT);
    }
    */
}