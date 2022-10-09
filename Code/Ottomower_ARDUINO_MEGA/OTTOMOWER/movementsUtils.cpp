#include "movementsUtils.h"

#include "adcman.h"
#include "batteryUtils.h"
#include "compassUtils.h"
#include "lcdDisplay.h"
#include "motorsController.h"
#include "perimeter.h"
#include "robot.h"
#include "sonar.h"
#include "wireDetection.h"
#include "wireTracking.h"

/**
 * Manouvers are a set of motion functions or motor actions that are regulary called
 * e.g. Turn the mower around
 */

/**
 *  Moves the Mower Forwards in the garden and activates compass features if enabled
 */
void robotMoveAroundTheGarden() {
    motorsActivateSpinBlades();
    motorsSetPinsToGoForwards();

    robot.loopCycleMowing = (robot.loopCycleMowing + 1);

    if (COMPASS_ACTIVATE == 1) {
        readRobotCompassDegrees();  // Gets the latest compass reading
    }

    if (robot.loopCycleMowing < CYCLE_FOR_STABILEZE || COMPASS_HEADING_HOLD_ENABLED == 0 || COMPASS_ACTIVATE == 0) {
        //da al robot 5 cicli per stabilizzarsi
        Serial.print(F("Compass-Lock:OFF|"));
        motorsSetFullSpeed();
    } else if (robot.loopCycleMowing == CYCLE_FOR_STABILEZE) {
        //al 5 ciclo si salva i gradi da mantenere
        robot.headingLock = robot.compassHeadingDegrees;  //  use the current heading as the lock
    } else if (robot.loopCycleMowing > CYCLE_FOR_STABILEZE && COMPASS_HEADING_HOLD_ENABLED == 1) {
        //dal 5 ciclo in poi cerca di mantenere i gradi
        // if the Mower is tracking using the compass steer here
        Serial.print(F("Compass-Lock:ON|"));
        keepDirectionWithCompassWheelCompensation();
        motorsSetDynamicSteeringSpeed();  // Removes the full speed function if the mower is trying to hold to the compass heading.
    }

    if (robot.loopCycleMowing > MAX_CYCLES) {
        // MAX_CYCLES the max length for my garden. Adjust accordingly
        Serial.println("");
        Serial.println("Loop Cycle at Max");
        Serial.println("");
        robotReverseDirection();    // Turn around the mower
        robot.loopCycleMowing = 0;  // Restes the loop cycle to start again.
    }
}

/**
 * Reverse robot direction when loopCycleMowing reach max or if robot is outside the perimeter
 */
void robotReverseDirection() {
    if (COMPASS_ACTIVATE == 1) {
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.write(robot.CHAR_BACK_ARROW);
        robot.lcdDisplay.setCursor(2, 1);
        robot.lcdDisplay.write(robot.CHAR_BACK_ARROW);

        motorsStopWheelMotors();
        motorsStopSpinBlades();  // Stop the blades from spinning
        delay(500);

        motorsSetPinsToGoBackwards();
        motorsSetFullSpeed();
        delay(1000);

        motorsStopWheelMotors();
        delay(500);

        Serial.println("");
        Serial.print("Compas_Direction:");
        Serial.print(robot.endCycleCompassDirection);
        Serial.print("|Turning 90°|");

        //TODO valutare se fare il codice di seguito 2 votle per fare 2 turnToCompassTarget

        //read robot degree
        readRobotCompassDegrees();
        float compassTarget = robot.compassHeadingDegrees;
        if (robot.endCycleCompassDirection == 0) {
            compassTarget += COMPASS_MOW_DIRECTION;
        } else {
            compassTarget -= COMPASS_MOW_DIRECTION;
        }
        if (compassTarget > 360) {
            compassTarget = compassTarget - 360;
        } else if (compassTarget < 0) {
            compassTarget = compassTarget + 360;
        }

        turnToCompassTarget(compassTarget);

        //Set the compass leg to the next stage.
        robot.endCycleCompassDirection = !robot.endCycleCompassDirection;
    }

    motorsStopWheelMotors();
    Check_Wire_In_Out();
    delay(200);  //TODO sicuro?
    Check_Wire_In_Out();
    robot.loopCycleMowing = 1;
    robot.SonarHit1Total = 0;
    robot.SonarHit2Total = 0;
    robot.SonarHit3Total = 0;
    robot.lcdDisplay.clear();  //TODO perch qua e non nel metodo comune?
}

/**
 * Search the wire
 * The logic change if mover is cutting o leaving base
 */
void findWire() {
    Serial.println(F("Find Wire Track Function Activated"));
    robot.searchingWire = 1;
    motorsStopSpinBlades();
    motorsStopWheelMotors();  // Stop all wheel motion
    delay(1000);

    robot.wireFound = 0;

    bool wireOn = isWireOn();  // Check to see that the wire is on.
    while (!wireOn) {
        //TODO mettiamo un limite?
        //TODO lo spostiamo di poco?
        
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print(TRS_NO_WIRE);
        delay(100);

        wireOn = isWireOn();
    }

    lcdUpdateScreen();

    readWireSensor();
    if (robot.inside) {
        //go forward searching wire

        motorsSetPinsToGoForwards();
        motorsSetFullSpeed();
        long start = millis();
        while (robot.inside && (millis() - start) < 120000) {  //search cable max for 2 mins
            Serial.print(millis());
            Serial.print(">");

            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.write(robot.CHAR_FORWARD);
            robot.lcdDisplay.setCursor(2, 1);
            robot.lcdDisplay.write(robot.CHAR_FORWARD);

            //stop where robot exit premiter
            readWireSensor();  //check inside/outside perimeter and update inside
            logBoundaryWireStatus();

            readVoltAmp();  //update volt and amp info
            readSonarSensors();
            lcdUpdateScreen();  //Update LCD Info
            if (robot.SonarHit1Total >= SONAR_MAX_HIT || robot.SonarHit2Total >= SONAR_MAX_HIT || robot.SonarHit3Total >= SONAR_MAX_HIT) {
                avoidSonarObstacle();  //TODO prima lo faceva solo se era dentro al perimetro....
                //restart motor after avoid
                motorsSetPinsToGoForwards();
                motorsSetFullSpeed();
            }
            Serial.println("|");
        }

        if (!robot.inside) {
            robot.wireFound = 1;
        }

    } else {
        //go back searching wire
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.write(robot.CHAR_BACK_ARROW);
        robot.lcdDisplay.setCursor(2, 1);
        robot.lcdDisplay.write(robot.CHAR_BACK_ARROW);

        motorsSetPinsToGoBackwards();
        motorsSetFullSpeed();
        lcdUpdateScreen();  //Update LCD Info
        long start = millis();
        while (!robot.inside && (millis() - start) < 60000) {  //max forware for 1 minuts, then stop robot
            Serial.print(millis());
            Serial.print(">");
            //stop when enter perimter
            readWireSensor();  //check inside/outside perimeter and update inside
            logBoundaryWireStatus();
            Serial.println("|");
        }

        if (robot.inside) {
            robot.wireFound = 1;
        }
    }

    Serial.println("|");
    Serial.print("Wire found:");
    Serial.println(robot.wireFound);

    motorsStopWheelMotors();
    delay(1000);

    // Position the mower further over the wire so it has space to turn 90° onto the wire.
    if (robot.wireFound == 1) {
        motorsSetPinsToGoForwards();
        delay(100);
        motorsSetFullSpeed(); //va avanti di poco per essere sicuro di essere fuori e avere spazio per girare
        delay(500);
        motorsStopWheelMotors();

        if (robot.mowerTrackToCharge == 1) {
            //mower is goin to charge..

            if (CW_TRACKING_SEARCHING_CHARGE == 1) {
                // Track perimeter wire in a Clockwise Direction to the charging station
                motorsSetPinsToTurnRight();
                Serial.println(F("CW Tracking to Charger"));
            } else {
                motorsSetPinsToTurnLeft();
                Serial.println(F("CCW Tracking toCharger"));
            }
        } else if (robot.mowerTrackToExit == 1) {
            //mower il leaving home..

            if (CW_TRACKING_EXITING_CHARGE == 1) {
                // Track perimeter wire in a Clockwise Direction to the charging station
                motorsSetPinsToTurnRight();
                Serial.println(F("CW Tracking to Exit"));
            } else {
                motorsSetPinsToTurnLeft();
                Serial.println(F("CCW Tracking to Exit"));
            }
        }

        //position the robot on the wire
        readWireSensor();
        while (robot.inside == false) {
            // Do this loop until mower is back  the wire fence
            //motorsSetFullSpeed();     // Go full speed (in the case turning as set by the previous logic)
            motorsReduceTurnSpeed(100);
            logBoundaryWireStatus();  // Prints of the status of the wire sensor readings.
            readWireSensor();         // Read the wire sensor and see of the mower is now  or outside the wire
        }
        motorsStopWheelMotors();  // Stop the mower on the wire facing the correct direction.

        // Spins the mower over the wire in the driection of tracking

    } else {
        //abort wire find
        manouverParkTheMower();
    }

    robot.searchingWire = 0;
}

/**
 * Avoid sonar obstacle
 */
void avoidSonarObstacle() {
    motorsStopWheelMotors();
    delay(500);

    motorsSetPinsToGoBackwards();
    motorsSetFullSpeed();
    delay(SONAR_REVERSE_DELAY);

    motorsStopWheelMotors();

    if (robot.distance1 < SONAR_MAX_DISTANCE || robot.distance2 < SONAR_MAX_DISTANCE) {
        motorsSetPinsToTurnRight();
        motorsReduceTurnSpeed(0);
        delay(SONAR_TURN_OBSTACLE_DELAY_TIME);
    } else if (robot.distance3 < SONAR_MAX_DISTANCE) {
        motorsSetPinsToTurnLeft();
        motorsReduceTurnSpeed(0);
        delay(SONAR_TURN_OBSTACLE_DELAY_TIME);
    }

    motorsStopWheelMotors();

    robot.SonarHit1Total = 0;
    robot.SonarHit2Total = 0;
    robot.SonarHit3Total = 0;
    robot.loopCycleMowing = 0;
}

/**
 * Set the mower in dock status
 */
void manouverDockTheMower() {
    robot.mowerDocked = 1;
    robot.mowerParked = 0;
    robot.mowerRunning = 0;
    robot.mowerParkedLowBatt = 0;
    robot.mowerTrackToExit = 0;
    robot.mowerTrackToCharge = 0;
    robot.mowerError = 0;
    robot.loopCycleMowing = 0;
    motorsStopWheelMotors();
    motorsStopSpinBlades();
    robot.turnOffMotorsRelay();
    //TODO serve? lcdUpdateScreen();
    robot.chargeDetectedMEGA = 0;
}

// Mower is a parked position and needs manual charging
void Manouver_Park_The_Mower_Low_Batt() {
    robot.mowerDocked = 0;
    robot.mowerParked = 0;
    robot.mowerRunning = 0;
    robot.mowerParkedLowBatt = 1;
    robot.mowerTrackToCharge = 0;
    robot.mowerError = 0;
    robot.loopCycleMowing = 0;
}

// Mower is in a parked or paused potion ready to restart
void manouverParkTheMower() {
    if (robot.mowerParked == 0) {
        robot.lcdDisplay.clear();
    }
    robot.mowerDocked = 0;
    robot.mowerParked = 1;
    robot.mowerRunning = 0;
    robot.mowerParkedLowBatt = 0;
    robot.mowerTrackToCharge = 0;
    robot.trackingWire = 0;
    robot.mowerTrackToExit = 0;
    robot.mowerError = 0;
    robot.loopCycleMowing = 0;
    motorsStopWheelMotors();
    motorsStopSpinBlades();
    robot.turnOffMotorsRelay();
}

void Manouver_Hibernate_Mower() {
    robot.mowerDocked = 0;
    robot.mowerParked = 0;
    robot.mowerRunning = 0;
    robot.mowerParkedLowBatt = 0;
    robot.mowerTrackToCharge = 0;
    robot.trackingWire = 0;
    robot.mowerTrackToExit = 0;
    robot.mowerError = 1;
    robot.loopCycleMowing = 0;
    motorsStopWheelMotors();
    motorsStopSpinBlades();
    robot.turnOffMotorsRelay();
}

/**
 * Mower is sent to the charging station after low volts are detected or mebrane key input.
 */
void goToChargingStation() {
    robot.mowerDocked = 0;
    robot.mowerParked = 0;  //<-- DIVENTA 1 quando arriva alla base di ricarica
    robot.mowerRunning = 0;
    robot.mowerParkedLowBatt = 0;
    robot.mowerTrackToCharge = 1;
    robot.trackingWire = 1;
    robot.mowerTrackToExit = 0;
    robot.mowerError = 0;
    robot.loopCycleMowing = 0;
    robot.wireFound = 0;

    delay(5);
    motorsStopSpinBlades();
    motorsStopWheelMotors();
    delay(2000);
    robot.turnOnMotorsRelay();
    delay(500);

    // 1.gira a cazzo di cane per posizionarsi giusto verso la base sfruttando il campo magnetico
    if (COMPASS_ACTIVATE == 1) {
        compassTurnMowerToHomeDirection();
    }

    //2. corre finchè trova il cavo
    findWire();

    //3. segue il filo
    if (robot.wireFound == 1) {
        followingWireToDock();
    }

    //4. si è perso...ricomincia
    if (robot.wireFound == 1) {  //TODO è giusta questa if?
        //ricorsivo
        goToChargingStation();
        //TODO quando si arrende?
    }
}

void leaveChargingStation(bool followWireWhenExit) {
    // Zone 1 or Zone 2 and the Wire itterations are set on the Membrane Buttons.
    // These values are then crried into the following functions.
    robot.turnOnMotorsRelay();
    delay(1000);

    robot.mowerDocked = 0;
    robot.mowerParked = 0;
    robot.mowerRunning = 0;
    robot.mowerParkedLowBatt = 0;
    robot.rainHitDetected = 0;
    robot.mowerError = 0;
    robot.trackingWire = 0;

    // Move the Mower into position backing out of the docking station
    robot.lcdDisplay.clear();
    robot.lcdDisplay.print("Exiting Garage");
    delay(2000);

    motorsSetPinsToGoBackwards();  // Prepare motors pins to go Backwards
    motorsSetFullSpeed();          // Turn the wheels
    delay(4000);                   // Backwards time

    motorsStopWheelMotors();     // Stop
    motorsSetPinsToTurnRight();  // Prepare motors to turn left
    motorsReduceTurnSpeed(0);    // Turn the wheels
    delay(1000);                 // Turn time

    motorsStopWheelMotors();       // Stop
    motorsSetPinsToGoBackwards();  // Set again to go backwards
    motorsSetFullSpeed();          // Turn the wheels
    delay(1000);                   // Backwards Time

    motorsSetPinsToTurnRight();  // Set to go left
    motorsReduceTurnSpeed(0);    // Turn the wheels
    delay(500);                  // Turning time

    motorsStopWheelMotors();      // Stop
    motorsSetPinsToGoForwards();  // Set to go wheel motor pins to go forwards
    motorsStopWheelMotors();      // Stop / Park the mower here

    robot.lcdDisplay.clear();  // Clears the LCD display

    if (robot.isPerimeterWireEnable) {
        if (isWireOn()) {
            //Function to follow the wire for a specific amount of time set by the itterations 'I'
            if (followWireWhenExit) {
                robot.mowerTrackToExit = 1;
                //Located the boundary wire
                findWire();
                trackWireFromDockToGarden();
                moveAwayFromWire();
            }
        }
    }

    //stop the mower ready to go
    manouverParkTheMower();
}

// Function to re-find the wire if the mower looses the wire while mowing
// 3 outside the wires ativates this function. Sonar and wire function is then used to re-find the wire.
void Manouver_Outside_Wire_ReFind_Function() {
    motorsStopSpinBlades();
    robot.lcdDisplay.clear();
    robot.lcdDisplay.print("Trying to find");
    robot.lcdDisplay.setCursor(0, 1);
    robot.lcdDisplay.print("Wire Again...");
    readWireSensor();
    delay(20);
    logBoundaryWireStatus();
    readWireSensor();
    delay(20);
    logBoundaryWireStatus();
    while (robot.inside == false) {  // If the mower is outside the wire then run the following code.
        ADCMan.run();
        readWireSensor();
        logBoundaryWireStatus();
        //Check_Wire_In_Out();
        delay(500);
        robot.distanceBlockage = getSonarDistance(trigPin1, echoPin1, 1);
        delay(500);
        Serial.print("Distance measured from sonar :");
        Serial.println(robot.distanceBlockage);

        // if the sonar is measuring an opening as the distance is greater than 300cm then move forward in that direction.
        if (robot.distanceBlockage > 400) {
            while ((robot.inside == false) && (robot.distanceBlockage > 400)) {
                motorsSetPinsToGoForwards();
                motorsSetFullSpeed();
                delay(500);
                readWireSensor();
                logBoundaryWireStatus();
                robot.distanceBlockage = getSonarDistance(trigPin1, echoPin1, 1);
                delay(10);
                //Check_Wire_In_Out();
            }
            motorsStopWheelMotors;
            readWireSensor();
            logBoundaryWireStatus();
        }

        // if the sonar is measuring something less than 300cm then turn to the left and measure again
        // keep turning 'while function' until a path is open.
        while (robot.distanceBlockage < 300) {
            motorsSetPinsToTurnLeft();
            delay(200);
            motorsReduceTurnSpeed(0);
            delay(100);
            robot.distanceBlockage = getSonarDistance(trigPin1, echoPin1, 1);
            delay(10);
        }
        motorsStopWheelMotors;
        readWireSensor();
        logBoundaryWireStatus();
    }

    Serial.println("Mower is now back inside the wire......?");
    robot.lcdDisplay.clear();
    robot.lcdDisplay.print("Mower now");
    robot.lcdDisplay.setCursor(0, 1);
    robot.lcdDisplay.print("Inside Wire?");
    robot.outsideWireCount = 0;
    //FindWireTrack();
}

/**
 * After wire tracking the code to bering the mower to a sensible position away from the wire
 * This poart of the code should eb modified to fit your garden needs
 */
void moveAwayFromWire() {
    Serial.println("start mower code here");
    motorsStopWheelMotors();    // Stop the wheel motors
    motorsSetPinsToTurnLeft();  // Turn left
    motorsSetFullSpeed();
    delay(1400);                  // Turn left for 0.7seconds
    motorsStopWheelMotors();      // Stop the wheel motors.
    motorsSetPinsToGoForwards();  // Get ready to move off
}

void Specials_Find_Wire_Track() {
    Serial.println(F("Lost Mower - find wire Track"));
    robot.lcdDisplay.clear();
    robot.lcdDisplay.print("Finding Wire...  ");
    motorsStopSpinBlades();
    delay(5);
    robot.wireFound = 0;
    bool wireOn = isWireOn();  // TODO, non viene mai usato...

    for (int i = 0; i <= 1; i++) {
        Serial.print(F("Position Try = "));
        Serial.println(i);
        readWireSensor();
        delay(20);
        readWireSensor();
        delay(20);
        Serial.println(F("before if loops start"));
        logBoundaryWireStatus();

        // First go backwards if the mower is outside the wire
        if (robot.inside == false) {  // If the mower is outside the wire then run the following code.
            readWireSensor();
            logBoundaryWireStatus();
            motorsStopWheelMotors();  // Stop all wheel motion
            delay(1000);
            motorsSetPinsToGoBackwards();  // Set the mower to back up
            delay(100);
            robot.lcdDisplay.clear();
            robot.lcdDisplay.print("Backwards Try...  ");
            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.print("Finding Wire  ");
            delay(100);
            while (robot.inside != true) {  // While the mower is still outside the fence run this code
                motorsSetFullSpeed();       // Go full speed (in this case backwards)
                readWireSensor();           // Read the wire sensor and see of the mower is now  or outside the wire
                logBoundaryWireStatus();    // Prints of the status of the wire sensor readings.
                Serial.println("");
            }
        }

        motorsStopWheelMotors();
        delay(5);
    }

    // Code to go forwards until the mower is outside/ON the wire
    if (robot.inside == true) {  // If the Mower is situated  the wire then run the following code.
        readWireSensor();
        Serial.println(F("CODE POSITION - MOTOR FORWARDS LOOP:  If statements"));
        logBoundaryWireStatus();
        motorsStopWheelMotors();
        delay(1000);
        motorsSetPinsToGoForwards();  // Set the motors to move the mower forwards
        delay(100);
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Forward Try...  ");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Finding Wire  ");
        delay(100);
        int cycle = 0;                   // resets the cycles
        while (robot.inside != false) {  // Move the mower forward until mower is outisde/ON the wire fence or 500 cycles have passed
            cycle = cycle + 1;
            motorsSetFullSpeed();     // Go full speed (in this case forwards)
            readWireSensor();         // Read the wire sensor and see of the mower is now  or outside the wire
            logBoundaryWireStatus();  // Prints of the status of the wire sensor readings.
        }
        if (cycle > MAX_CYCLE_WIRE_FIND) {  // Track forwards for 500 cycles
            robot.wireFound = 0;            // if mower is still tracking after 500 cycles then cancel the find.
            Serial.println("Max Forward Cycles reached");
        }
    }
    motorsStopWheelMotors();
    delay(5);
}
