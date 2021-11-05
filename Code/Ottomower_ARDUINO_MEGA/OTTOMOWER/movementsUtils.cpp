#include "movementsUtils.h"

#include "adcman.h"
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
        Console.print(F("Compass-Lock:OFF|"));
        motorsSetFullSpeed();
    } else if (robot.loopCycleMowing == CYCLE_FOR_STABILEZE) {
        //al 5 ciclo si salva i gradi da mantenere
        robot.headingLock = robot.compassHeadingDegrees;  //  use the current heading as the lock
    } else if (robot.loopCycleMowing > CYCLE_FOR_STABILEZE) {
        //dal 5 ciclo in poi cerca di mantenere i gradi
        // if the Mower is tracking using the compass steer here
        Calculate_Compass_Wheel_Compensation();
        motorsSetDynamicSteeringSpeed();  // Removes the full speed function if the mower is trying to hold to the compass heading.
        Console.print(F("Compass-Lock:ON|"));
    }

    if (robot.loopCycleMowing > MAX_CYCLES) {
        // MAX_CYCLES the max length for my garden. Adjust accordingly
        Console.println("");
        Console.println("Loop Cycle at Max");
        Console.println("");
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

        Console.println("");
        Console.print("Compas_Leg:");
        Console.print(robot.endCycleCompassDirection);
        Console.print("|Turning 90°|");

        //TODO valutare se fare il codice di seguito 2 votle per fare 2 turnToCompassTarget

        //read robot degree
        resetCompassOffset();
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
    Console.println(F("Find Wire Track Function Activated"));
    printFindFire();
    motorsStopSpinBlades();
    delay(5);  //TODO perchè^
    robot.abortWireFind = 0;
    robot.noWireFound = 0;
    bool wireOn = isWireOn();  // Check to see that the wire is on.

    for (int i = 0; i <= 1; i++) {  //che cazzo è sta merda??? deve fare 2 volte la stessa cosa

        if (robot.mowerParked == 0) {
            Console.print(F("Position Try = "));
            Console.println(i);
            ADCMan.run();
            UpdateWireSensor();
            delay(20);
            ADCMan.run();
            UpdateWireSensor();
            delay(20);
            Console.println(F("before if loops start"));
            PrintBoundaryWireStatus();

            // First go backwards if the mower is outside the wire
            if ((robot.inside == false) && (robot.abortWireFind == 0)) {
                //Sono fuori dal perimetro, provo a tornare dentro

                ADCMan.run();
                UpdateWireSensor();
                PrintBoundaryWireStatus();

                motorsStopWheelMotors();  // Stop all wheel motion
                delay(1000);
                motorsSetPinsToGoBackwards();  // va in retromarcia...

                delay(100);
                robot.lcdDisplay.clear();
                robot.lcdDisplay.print("Backwards Try...  ");
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("Finding Wire  ");
                delay(100);

                // While the mower is still outside the fence run this code
                while ((robot.inside != true) && (robot.abortWireFind == 0)) {
                    motorsSetFullSpeed();  // Go full speed (in this case backwards)
                    UpdateWireSensor();    // Read the wire sensor and see of the mower is now  or outside the wire

                    ADCMan.run();
                    PrintBoundaryWireStatus();  // Prints of the status of the wire sensor readings.
                    Console.println("");

                    if (robot.mowerParked == 1) {
                        Console.println("Abort Wire Find");
                        robot.abortWireFind = 1;
                    }

                    //TODO dopo un po' è il caso di fermare tutto
                }

                motorsStopWheelMotors();
                delay(5);
            }

            if (robot.inside == true && robot.abortWireFind == 0 && robot.noWireFound == 0) {
                // If the Mower is situated  the wire then run the following code.
                // Code to go forwards until the mower is outside/ON the wire

                ADCMan.run();
                UpdateWireSensor();
                Console.println(F("CODE POSITION - MOTOR FORWARDS LOOP:  If statements"));
                PrintBoundaryWireStatus();

                motorsStopWheelMotors();
                delay(1000);
                motorsSetPinsToGoForwards();  //va avanti

                delay(100);
                robot.lcdDisplay.clear();
                robot.lcdDisplay.print("Forward Try...  ");
                robot.lcdDisplay.setCursor(0, 1);
                robot.lcdDisplay.print("Finding Wire  ");
                delay(100);

                int cycle = 0;                                                                       // resets the cycles
                while (robot.inside != false && robot.noWireFound == 0 && robot.mowerParked == 0) {  // Move the mower forward until mower is outisde/ON the wire fence or 500 cycles have passed
                    cycle = cycle + 1;

                    robot.lcdDisplay.setCursor(0, 1);
                    robot.lcdDisplay.print("Track -> Charge");

                    motorsSetFullSpeed();  // Go full speed (in this case forwards)
                    UpdateWireSensor();    // Read the wire sensor and see of the mower is now  or outside the wire

                    ADCMan.run();
                    PrintBoundaryWireStatus();  // Prints of the status of the wire sensor readings.
                    Console.println("");

                    if (robot.mowerParked == 1) {
                        Console.println("Abort Wire Find");
                        robot.abortWireFind = 1;
                    }
                    if (cycle > MAX_CYCLE_WIRE_FIND) {  // Track forwards for 500 cycles
                        robot.noWireFound = 1;          // if mower is still tracking after 500 cycles then cancel the find.
                        Console.println("Max Forward Cycles reached");
                    }
                }
                motorsStopWheelMotors();
                delay(5);
            }

            motorsStopWheelMotors();
            delay(1000);
        }
    }

    // Position the mower further over the wire so it has space to turn 90° onto the wire.
    if ((robot.abortWireFind == 0) && (robot.noWireFound == 0) && (robot.mowerParked == 0)) {
        motorsSetPinsToGoForwards();
        delay(100);
        motorsSetFullSpeed();
        delay(500);
        motorsStopWheelMotors();
    }

    // Sets the firection of spin depensing on if the mower is eciting or tracking home

    // Set pins to track home to charge.
    if ((robot.mowerTrackToCharge == 1) && (robot.mowerParked == 0)) {
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Track -> Charge");
        delay(1000);

        if (CW_TRACKING_SEARCHING_CHARGE == 1) {
            motorsSetPinsToTurnRight();  // Track perimeter wire in a Clockwise Direction to the charging station
            Console.println(F("CW Tracking to Charger"));
        } else {
            motorsSetPinsToTurnLeft();
            Console.println(F("CCW Tracking toCharger"));
        }
    }

    // Set pins to track to exit.
    if ((robot.mowerTrackToExit == 1) && (robot.mowerParked == 0)) {
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Track -> Exit");
        delay(1000);
        if (CW_TRACKING_EXITING_CHARGE == 1) {
            motorsSetPinsToTurnRight();  // Track perimeter wire in a Clockwise Direction to the charging station
            Console.println(F("CW Tracking to Exit"));
        } else {
            motorsSetPinsToTurnLeft();
            Console.println(F("CCW Tracking to Exit"));
        }
    }

    delay(20);
    // Pins are now set to turn from the above logic which way to turn onto the wire

    // Update the mowers position to the wire.
    ADCMan.run();
    UpdateWireSensor();
    ADCMan.run();
    PrintBoundaryWireStatus();  // Prints of the status of the wire sensor readings
    delay(20);

    // Spins the mower over the wire in the driection of tracking
    while ((robot.inside == false) && (robot.abortWireFind == 0) && (robot.noWireFound == 0) && (robot.mowerParked == 0)) {
        while (robot.inside != true) {  // Do this loop until mower is back  the wire fence
            motorsSetFullSpeed();       // Go full speed (in the case turning as set by the previous logic)
            UpdateWireSensor();         // Read the wire sensor and see of the mower is now  or outside the wire
            ADCMan.run();
            PrintBoundaryWireStatus();  // Prints of the status of the wire sensor readings.
        }
        motorsStopWheelMotors();  // Stop the mower on the wire facing the correct direction.
    }

    motorsStopWheelMotors();
    if (robot.abortWireFind == 0) {
        Console.println(F("Track Wire Function Complete - ON WIRE??"));
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Wire Found...");
        delay(2000);  // 2 second pause to check result
        motorsSetPinsToGoForwards();
        delay(100);
    }

    if (robot.abortWireFind == 1) {
        Console.println("Wire Find Aborted");
        robot.abortWireFind = 0;
        motorsSetPinsToGoForwards();
        manouverParkTheMower();
    }

    if (robot.noWireFound == 1) {
        Console.println("Re-starting wire find");
        motorsSetPinsToGoForwards();
    }
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
        motorsetTurnSpeed(0);
        delay(SONAR_TURN_OBSTACLE_DELAY_TIME);
    } else if (robot.distance3 < SONAR_MAX_DISTANCE) {
        motorsSetPinsToTurnLeft();
        motorsetTurnSpeed(0);
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
    robot.Turn_Off_Relay();
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
    robot.Turn_Off_Relay();
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
    robot.Turn_Off_Relay();
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
    robot.noWireFound = 0;

    delay(5);
    motorsStopSpinBlades();
    motorsStopWheelMotors();
    delay(2000);
    robot.Turn_On_Relay();
    delay(500);

    // 1.gira a cazzo di cane per posizionarsi giusto verso la base sfruttando il campo magnetico
    if (robot.mowerParked == 0 && COMPASS_ACTIVATE == 1) {
        //TODO.....approfondire
        Compass_Turn_Mower_To_Home_Direction();
    }

    //2. corre finchè trova il cavo
    if (robot.mowerParked == 0) {
        findWire();
    }

    //3. segue il filo
    if (robot.mowerParked == 0 && robot.noWireFound == 0) {
        followingWireToDock();
    }

    //4. si è perso...ricomincia
    if (robot.noWireFound == 1) {
        //ricorsivo
        goToChargingStation();
        //TODO quando si arrende?
    }
}

void leaveChargingStation(bool followWireWhenExit) {
    // Zone 1 or Zone 2 and the Wire itterations are set on the Membrane Buttons.
    // These values are then crried into the following functions.
    robot.Turn_On_Relay();
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
    motorsetTurnSpeed(0);        // Turn the wheels
    delay(1000);                 // Turn time

    motorsStopWheelMotors();       // Stop
    motorsSetPinsToGoBackwards();  // Set again to go backwards
    motorsSetFullSpeed();          // Turn the wheels
    delay(1000);                   // Backwards Time

    motorsSetPinsToTurnRight();  // Set to go left
    motorsetTurnSpeed(0);        // Turn the wheels
    delay(500);                  // Turning time

    motorsStopWheelMotors();      // Stop
    motorsSetPinsToGoForwards();  // Set to go wheel motor pins to go forwards
    motorsStopWheelMotors();      // Stop / Park the mower here

    robot.lcdDisplay.clear();  // Clears the LCD display

    if (robot.isPerimeterWireEnable == 1) {
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

    if (COMPASS_ACTIVATE == 1) {
        delay(500);
        resetCompassOffset();
    }
}

// Function to re-find the wire if the mower looses the wire while mowing
// 3 outside the wires ativates this function. Sonar and wire function is then used to re-find the wire.
void Manouver_Outside_Wire_ReFind_Function() {
    motorsStopSpinBlades();
    robot.lcdDisplay.clear();
    robot.lcdDisplay.print("Trying to find");
    robot.lcdDisplay.setCursor(0, 1);
    robot.lcdDisplay.print("Wire Again...");
    ADCMan.run();
    UpdateWireSensor();
    delay(20);
    PrintBoundaryWireStatus();
    ADCMan.run();
    UpdateWireSensor();
    delay(20);
    PrintBoundaryWireStatus();
    while (robot.inside == false) {  // If the mower is outside the wire then run the following code.
        ADCMan.run();
        UpdateWireSensor();
        PrintBoundaryWireStatus();
        //Check_Wire_In_Out();
        delay(500);
        robot.distanceBlockage = getSonarDistance(trigPin1, echoPin1, 1);
        delay(500);
        Console.print("Distance measured from sonar :");
        Console.println(robot.distanceBlockage);

        // if the sonar is measuring an opening as the distance is greater than 300cm then move forward in that direction.
        if (robot.distanceBlockage > 400) {
            while ((robot.inside == false) && (robot.distanceBlockage > 400)) {
                motorsSetPinsToGoForwards();
                motorsSetFullSpeed();
                delay(500);
                ADCMan.run();
                UpdateWireSensor();
                PrintBoundaryWireStatus();
                robot.distanceBlockage = getSonarDistance(trigPin1, echoPin1, 1);
                delay(10);
                //Check_Wire_In_Out();
            }
            motorsStopWheelMotors;
            ADCMan.run();
            UpdateWireSensor();
            PrintBoundaryWireStatus();
        }

        // if the sonar is measuring something less than 300cm then turn to the left and measure again
        // keep turning 'while function' until a path is open.
        while (robot.distanceBlockage < 300) {
            motorsSetPinsToTurnLeft();
            delay(200);
            motorsetTurnSpeed(0);
            delay(100);
            robot.distanceBlockage = getSonarDistance(trigPin1, echoPin1, 1);
            delay(10);
        }
        motorsStopWheelMotors;
        ADCMan.run();
        UpdateWireSensor();
        PrintBoundaryWireStatus();
    }

    Console.println("Mower is now back inside the wire......?");
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
    robot.abortWireFind = 0;
    robot.noWireFound = 0;
    bool wireOn = isWireOn();  // TODO, non viene mai usato...

    for (int i = 0; i <= 1; i++) {
        Serial.print(F("Position Try = "));
        Serial.println(i);
        ADCMan.run();
        UpdateWireSensor();
        delay(20);
        ADCMan.run();
        UpdateWireSensor();
        delay(20);
        Serial.println(F("before if loops start"));
        PrintBoundaryWireStatus();

        // First go backwards if the mower is outside the wire
        if (robot.inside == false) {  // If the mower is outside the wire then run the following code.
            ADCMan.run();
            UpdateWireSensor();
            PrintBoundaryWireStatus();
            motorsStopWheelMotors();  // Stop all wheel motion
            delay(1000);
            motorsSetPinsToGoBackwards();  // Set the mower to back up
            delay(100);
            robot.lcdDisplay.clear();
            robot.lcdDisplay.print("Backwards Try...  ");
            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.print("Finding Wire  ");
            delay(100);
            while ((robot.inside != true) && (robot.abortWireFind == 0)) {  // While the mower is still outside the fence run this code
                motorsSetFullSpeed();                                       // Go full speed (in this case backwards)
                UpdateWireSensor();                                         // Read the wire sensor and see of the mower is now  or outside the wire
                ADCMan.run();
                PrintBoundaryWireStatus();  // Prints of the status of the wire sensor readings.
                Serial.println("");
            }
        }

        motorsStopWheelMotors();
        delay(5);
    }

    // Code to go forwards until the mower is outside/ON the wire
    if (robot.inside == true) {  // If the Mower is situated  the wire then run the following code.
        ADCMan.run();
        UpdateWireSensor();
        Serial.println(F("CODE POSITION - MOTOR FORWARDS LOOP:  If statements"));
        PrintBoundaryWireStatus();
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
            motorsSetFullSpeed();  // Go full speed (in this case forwards)
            UpdateWireSensor();    // Read the wire sensor and see of the mower is now  or outside the wire
            ADCMan.run();
            PrintBoundaryWireStatus();  // Prints of the status of the wire sensor readings.
        }
        if (cycle > MAX_CYCLE_WIRE_FIND) {  // Track forwards for 500 cycles
            robot.noWireFound = 1;          // if mower is still tracking after 500 cycles then cancel the find.
            Serial.println("Max Forward Cycles reached");
        }
    }
    motorsStopWheelMotors();
    delay(5);
}
