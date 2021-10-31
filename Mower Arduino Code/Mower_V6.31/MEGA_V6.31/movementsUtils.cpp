#include "movementsUtils.h"
#include "motorsController.h"
#include "lcdDisplay.h"
#include "compassUtils.h"
#include "robot.h"
#include "compassUtils.h"
#include "adcman.h"
#include "wireDetection.h"
#include "perimeter.h"
#include "wireTracking.h"
#include "sonar.h"

/**
 * Manouvers are a set of motion functions or motor actions that are regulary called
 * e.g. Turn the mower around
 */

// Moves the Mower Forwards in the garden and activates compass features if enabled
void Manouver_Mow_The_Grass() {
    Motor_Action_Spin_Blades();
    SetPins_ToGoForwards();

    robot.loopCycleMowing = (robot.loopCycleMowing + 1);
    printLoop(robot.loopCycleMowing);
    delay(1);  //TODO xk?

    if (Compass_Activate == 1) {
        Get_Compass_Reading();  // Gets the latest compass reading
    }

    if (robot.loopCycleMowing < 5) {
        //da al robot 5 cicli per stabilizzarsi
        Console.print(F("C-Lock:OFF"));
        Console.print("|");
        Print_LCD_Mowing();
        Motor_Action_Go_Full_Speed();
        robot.compassHeadingLocked = 0;  // Turn off the compass heading lock for the new cycles
    } else if (robot.loopCycleMowing == 5) {
        //al 5 ciclo si salva i gradi da mantenere
        if (Compass_Heading_Hold_Enabled == 1 && Compass_Activate == 1) {
            robot.headingLock = robot.compassHeadingDegrees;  //  use the current heading as the lock
            robot.compassHeadingLocked = 1;                   // One more cycle of normal movement
        } else {
            Console.print(F("C-Lock:OFF"));
            Console.print("|");
            Print_LCD_Compass_Mowing();
            Motor_Action_Go_Full_Speed();
            robot.compassHeadingLocked = 0;
        }
    } else if (robot.loopCycleMowing > 5) {
        //dal 5 ciclo in poi cerca di mantenere i gradi
        if (Compass_Heading_Hold_Enabled == 1 && Compass_Activate == 1) {  // if the Mower is tracking using the compass steer here
            Calculate_Compass_Wheel_Compensation();
            Motor_Action_Dynamic_PWM_Steering();  // Removes the full speed function if the mower is trying to hold to the compass heading.
            Print_LCD_Compass_Mowing();
            Console.print(F("C-Lock:ON_"));
            Console.print("|");
        } else {
            //non usa la bussola per mantere la posizione
            Console.print(F("C-Lock:OFF"));
            Console.print("|");
            Motor_Action_Go_Full_Speed();
        }
    }

    if (robot.loopCycleMowing > Max_Cycles) {  // 150 the max length for my garden. Adjust accordingly
        Console.println("");
        Console.println("Loop Cycle at Max");
        Console.println("");
        Motor_Action_Stop_Spin_Blades();  // Stop the blades from spinning
        Manouver_Turn_Around();           // Turn around the mower
        robot.loopCycleMowing = 0;        // Restes the loop cycle to start again.
        lcd.clear();
    }
}

/**
 * Search the wire
 * The logic change if mover is cutting o leaving base
 */
void findWire() {
    Console.println(F("Find Wire Track Function Activated"));
    printFindFire();
    Motor_Action_Stop_Spin_Blades();
    delay(5); //TODO perchè^
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

                Motor_Action_Stop_Motors();  // Stop all wheel motion
                delay(1000);
                SetPins_ToGoBackwards();  // va in retromarcia...

                delay(100);
                lcd.clear();
                lcd.print("Backwards Try...  ");
                lcd.setCursor(0, 1);
                lcd.print("Finding Wire  ");
                delay(100);

                // While the mower is still outside the fence run this code
                while ((robot.inside != true) && (robot.abortWireFind == 0)) {
                    Motor_Action_Go_Full_Speed();  // Go full speed (in this case backwards)
                    UpdateWireSensor();            // Read the wire sensor and see of the mower is now  or outside the wire

                    ADCMan.run();
                    PrintBoundaryWireStatus();  // Prints of the status of the wire sensor readings.
                    Console.println("");

                    if (robot.mowerParked == 1) {
                        Console.println("Abort Wire Find");
                        robot.abortWireFind = 1;
                    }

                    //TODO dopo un po' è il caso di fermare tutto
                }

                Motor_Action_Stop_Motors();
                delay(5);
            }

            if (robot.inside == true && robot.abortWireFind == 0 && robot.noWireFound == 0) {
                // If the Mower is situated  the wire then run the following code.
                // Code to go forwards until the mower is outside/ON the wire

                ADCMan.run();
                UpdateWireSensor();
                Console.println(F("CODE POSITION - MOTOR FORWARDS LOOP:  If statements"));
                PrintBoundaryWireStatus();

                Motor_Action_Stop_Motors();
                delay(1000);
                SetPins_ToGoForwards();  //va avanti

                delay(100);
                lcd.clear();
                lcd.print("Forward Try...  ");
                lcd.setCursor(0, 1);
                lcd.print("Finding Wire  ");
                delay(100);

                int cycle = 0;                                                                 // resets the cycles
                while (robot.inside != false && robot.noWireFound == 0 && robot.mowerParked == 0) {  // Move the mower forward until mower is outisde/ON the wire fence or 500 cycles have passed
                    cycle = cycle + 1;

                    lcd.setCursor(0, 1);
                    lcd.print("Track -> Charge");

                    Motor_Action_Go_Full_Speed();  // Go full speed (in this case forwards)
                    UpdateWireSensor();            // Read the wire sensor and see of the mower is now  or outside the wire

                    ADCMan.run();
                    PrintBoundaryWireStatus();  // Prints of the status of the wire sensor readings.
                    Console.println("");

                    if (robot.mowerParked == 1) {
                        Console.println("Abort Wire Find");
                        robot.abortWireFind = 1;
                    }
                    if (cycle > Max_Cycle_Wire_Find) {  // Track forwards for 500 cycles
                        robot.noWireFound = 1;          // if mower is still tracking after 500 cycles then cancel the find.
                        Console.println("Max Forward Cycles reached");
                    }
                }
                Motor_Action_Stop_Motors();
                delay(5);
            }

            Motor_Action_Stop_Motors();
            delay(1000);
        }
    }

    // Position the mower further over the wire so it has space to turn 90° onto the wire.
    if ((robot.abortWireFind == 0) && (robot.noWireFound == 0) && (robot.mowerParked == 0)) {
        SetPins_ToGoForwards();
        delay(100);
        Motor_Action_Go_Full_Speed();
        delay(500);
        Motor_Action_Stop_Motors();
    }

    // Sets the firection of spin depensing on if the mower is eciting or tracking home

    // Set pins to track home to charge.
    if ((robot.mowerTrackToCharge == 1) && (robot.mowerParked == 0)) {
        lcd.setCursor(0, 1);
        lcd.print("Track -> Charge");
        delay(1000);

        if (CW_TRACKING_SEARCHING_CHARGE == 1) {
            SetPins_ToTurnRight();  // Track perimeter wire in a Clockwise Direction to the charging station
            Console.println(F("CW Tracking to Charger"));
        } else {
            SetPins_ToTurnLeft();
            Console.println(F("CCW Tracking toCharger"));
        }
    }

    // Set pins to track to exit.
    if ((robot.mowerTrackToExit == 1) && (robot.mowerParked == 0)) {
        lcd.setCursor(0, 1);
        lcd.print("Track -> Exit");
        delay(1000);
        if (CW_TRACKING_EXITING_CHARGE == 1) {
            SetPins_ToTurnRight();  // Track perimeter wire in a Clockwise Direction to the charging station
            Console.println(F("CW Tracking to Exit"));
        } else {
            SetPins_ToTurnLeft();
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
        while (robot.inside != true) {           // Do this loop until mower is back  the wire fence
            Motor_Action_Go_Full_Speed();  // Go full speed (in the case turning as set by the previous logic)
            UpdateWireSensor();            // Read the wire sensor and see of the mower is now  or outside the wire
            ADCMan.run();
            PrintBoundaryWireStatus();  // Prints of the status of the wire sensor readings.
        }
        Motor_Action_Stop_Motors();  // Stop the mower on the wire facing the correct direction.
    }

    Motor_Action_Stop_Motors();
    if (robot.abortWireFind == 0) {
        Console.println(F("Track Wire Function Complete - ON WIRE??"));
        lcd.clear();
        lcd.print("Wire Found...");
        delay(2000);  // 2 second pause to check result
        SetPins_ToGoForwards();
        delay(100);
    }

    if (robot.abortWireFind == 1) {
        Console.println("Wire Find Aborted");
        robot.abortWireFind = 0;
        SetPins_ToGoForwards();
        manouverParkTheMower();
    }

    if (robot.noWireFound == 1) {
        Console.println("Re-starting wire find");
        SetPins_ToGoForwards();
    }
}

void Manouver_Turn_Around() {
    if (Compass_Activate == 1) {
        Motor_Action_Stop_Motors();
        Motor_Action_Stop_Spin_Blades();
        delay(500);
        SetPins_ToGoBackwards();
        Motor_Action_Go_Full_Speed();
        delay(1000);
        Motor_Action_Stop_Motors();
        delay(500);

        if (robot.compassLeg == 0) {
            SetPins_ToTurnRight();
            Motor_Action_Go_Full_Speed();
            delay(200);
            Get_Compass_Reading();
            delay(200);
            Get_Compass_Reading();
            // Turn the Mower 90° to the corner
            Console.println("");
            Console.print("Compas_Leg = 0 | ");
            Console.print("Turning 90°|");
            robot.compassTarget = Compass_Mow_Direction + 90;
            if (robot.compassTarget > 360)
                (robot.compassTarget = robot.compassTarget - 360);
            Console.print("|Target Degree robot.heading = ");
            Console.print(robot.compassTarget);
            Console.print("|");
            Turn_To_Compass_Heading();

            // Move Forwards at 90° and stop
            SetPins_ToGoForwards();
            Motor_Action_Go_Full_Speed();
            delay(1500);
            Motor_Action_Stop_Motors();
            SetPins_ToTurnRight();
            Motor_Action_Go_Full_Speed();
            delay(200);
            Get_Compass_Reading();
            delay(200);
            Get_Compass_Reading();

            // Turn 180° to the initial mowing direction.
            Console.println("");
            Console.print("|Return Leg 1°");
            robot.compassTarget = Compass_Mow_Direction;
            if (robot.compassTarget > 360)
                (robot.compassTarget = robot.compassTarget - 360);
            Console.print("|Target Degree robot.heading = ");
            Console.print(robot.compassTarget);
            Console.println("|");
            Turn_To_Compass_Heading();
        }

        if (robot.compassLeg == 1) {
            // Turn the Mower 90° to the corner
            Console.println("");
            Console.print("robot.compassLeg = 1|");
            Console.print("Turning 90°");
            SetPins_ToTurnLeft();
            Motor_Action_Go_Full_Speed();
            delay(200);
            Get_Compass_Reading();
            delay(200);
            Get_Compass_Reading();
            robot.compassTarget = Compass_Mow_Direction + 90;
            if (robot.compassTarget < 0)
                (robot.compassTarget = 360 + robot.compassTarget);
            Console.print("|Target Degree robot.heading = ");
            Console.print(robot.compassTarget);
            Turn_To_Compass_Heading();

            // Move Forwards at 90° and stop
            SetPins_ToGoForwards();
            Motor_Action_Go_Full_Speed();
            delay(1500);
            Motor_Action_Stop_Motors();
            SetPins_ToTurnLeft;
            Motor_Action_Go_Full_Speed();
            delay(200);
            Get_Compass_Reading();
            delay(200);
            Get_Compass_Reading();

            // Turn 180° to the initial mowing direction.
            Console.println("");
            Console.print("|Return Leg 0°");
            robot.compassTarget = Compass_Mow_Direction + 180;
            if (robot.compassTarget > 360)
                (robot.compassTarget = robot.compassTarget - 360);
            Console.print("|Target Degree robot.heading = ");
            Console.print(robot.compassTarget);
            Console.println("|");
            Turn_To_Compass_Heading();
        }

        //Set the compass leg to the next stage.
        robot.compassLeg = robot.compassLeg + 1;
        if (robot.compassLeg > 1)
            robot.compassLeg = 0;
    }
    Motor_Action_Stop_Motors();
    Check_Wire_In_Out();
    delay(200);  //TODO sicuro?
    Check_Wire_In_Out();
    robot.loopCycleMowing = 1;
    robot.sonarHit = 0;
    robot.compassHeadingLocked = 0;
    lcd.clear();
}

void Manouver_Turn_Around_Sonar() {
    Motor_Action_Stop_Motors();
    delay(500);
    SetPins_ToGoBackwards();
    Motor_Action_Go_Full_Speed();
    delay(Mower_Reverse_Delay);
    Motor_Action_Stop_Motors();

    if (robot.distance1 < maxdistancesonar || robot.distance2 < maxdistancesonar) {
        lcd.setCursor(0, 8);
        lcd.print("Go Right -->       ");
        SetPins_ToTurnRight();
        Motor_Action_Turn_Speed();
        delay(Mower_Turn_Delay_Max);
    }

    if (robot.distance3 < maxdistancesonar) {
        lcd.setCursor(0, 8);
        lcd.print("<-- Go Left      ");
        SetPins_ToTurnLeft();
        Motor_Action_Turn_Speed();
        delay(Mower_Turn_Delay_Max);
    }

    Motor_Action_Stop_Motors();
    robot.compassHeadingLocked = 0;
    robot.sonarHit = 0;
    robot.loopCycleMowing = 0;
}

void Manouver_manuelMode() {
    robot.mowerDocked = 0;
    robot.mowerParked = 0;
    robot.mowerRunning = 0;
    robot.mowerParkedLowBatt = 0;
    robot.mowerTrackToExit = 0;
    robot.mowerTrackToCharge = 0;
    robot.mowerError = 0;
    robot.manuelMode = 1;
    robot.loopCycleMowing = 0;
    Motor_Action_Stop_Motors();
    Motor_Action_Stop_Spin_Blades();
}

void Manouver_Start_Mower() {
    robot.mowerDocked = 0;
    robot.mowerParked = 0;
    robot.mowerRunning = 1;
    robot.mowerParkedLowBatt = 0;
    robot.mowerTrackToCharge = 0;
    robot.rainHitDetected = 0;
    robot.mowerError = 0;
    robot.loopCycleMowing = 0;
    robot.manuelMode = 0;
    robot.Turn_On_Relay();
}

void Manouver_Mower_Exit_Dock() {
    robot.mowerDocked = 0;
    robot.mowerParked = 0;
    robot.mowerRunning = 0;
    robot.mowerParkedLowBatt = 0;
    robot.rainHitDetected = 0;
    robot.mowerError = 0;
    robot.manuelMode = 0;
    robot.trackingWire = 0;
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
    robot.manuelMode = 0;
    Motor_Action_Stop_Motors();
    Motor_Action_Stop_Spin_Blades();
    robot.Turn_Off_Relay();
    //TODO serve? lcdUpdateScreen();
    robot.chargeDetectedMEGA = 0;

    //Setup Alarms
    robot.alarmTimedMowON = 0;  // Turns off the 1 hr Alarm
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
    robot.manuelMode = 0;
    Motor_Action_Stop_Motors();
    Motor_Action_Stop_Spin_Blades();
}

// Mower is in a parked or paused potion ready to restart
void manouverParkTheMower() {
    if (robot.mowerParked == 0)
        lcd.clear();
    robot.mowerDocked = 0;
    robot.mowerParked = 1;
    robot.mowerRunning = 0;
    robot.mowerParkedLowBatt = 0;
    robot.mowerTrackToCharge = 0;
    robot.trackingWire = 0;
    robot.mowerTrackToExit = 0;
    robot.mowerError = 0;
    robot.loopCycleMowing = 0;
    robot.manuelMode = 0;
    Motor_Action_Stop_Motors();
    Motor_Action_Stop_Spin_Blades();
    robot.Turn_Off_Relay();

    robot.alarmTimedMowON = 0;  // Turns off the 1 hr Alarm
                                //if (Alarm_1_Repeat == 0) Alarm_1_ON = 0;
                                //if (Alarm_2_Repeat == 0) Alarm_2_ON = 0;
                                //if (Alarm_3_Repeat == 0) Alarm_3_ON = 0;
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
    robot.manuelMode = 0;
    Motor_Action_Stop_Motors();
    Motor_Action_Stop_Spin_Blades();
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
    robot.manuelMode = 0;
    robot.noWireFound = 0;


    delay(5);
    Motor_Action_Stop_Spin_Blades();
    Motor_Action_Stop_Motors();
    delay(2000);
    robot.Turn_On_Relay();
    delay(500);

    // 1.gira a cazzo di cane per posizionarsi giusto verso la base sfruttando il campo magnetico
    if (robot.mowerParked == 0 && Compass_Activate == 1) {
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
    }
}

void leaveChargingStation() {
    // Zone 1 or Zone 2 and the Wire itterations are set on the Membrane Buttons.
    // These values are then crried into the following functions.
    robot.Turn_On_Relay();
    delay(1000);

    Manouver_Mower_Exit_Dock();

    Special_Exit_From_Docking_Station();  // Move the Mower into position backing out of the docking station
    if (isPerimeterWireEnabled == 1) {
        robot.mowerTrackToExit = 1;
        delay(50);  //TODO perchè?

        if (isWireOn()) {
            findWire();  //TODO...perchè? Located the boundary wire

            if (robot.mowerParked == 0) {
                Track_Wire_From_Dock_to_Zone_X();
            }

            if (robot.mowerParked == 0) {
                Special_Move_Into_Garden_Zone_X();
            }

            if (robot.mowerParked == 0) {
                Manouver_Start_Mower();
            }

            if (robot.mowerParked == 1) {
                manouverParkTheMower();
            }

        } else {
            //Wire not detected
            Console.println("");
            Console.println("Perimeter Wire not detected");
            manouverParkTheMower();
        }
    } else {
        //wire not enable
        Console.println("");
        Console.println("Perimeter Wire not activated in settings");
        manouverParkTheMower();
    }
}

// Function to re-find the wire if the mower looses the wire while mowing
// 3 outside the wires ativates this function. Sonar and wire function is then used to re-find the wire.
void Manouver_Outside_Wire_ReFind_Function() {
    Motor_Action_Stop_Spin_Blades();
    lcd.clear();
    lcd.print("Trying to find");
    lcd.setCursor(0, 1);
    lcd.print("Wire Again...");
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
        robot.distanceBlockage = PingSonarX(trigPin1, echoPin1, 1, 1, 1, 4, 0);
        delay(500);
        Console.print("Distance measured from sonar :");
        Console.println(robot.distanceBlockage);

        // if the sonar is measuring an opening as the distance is greater than 300cm then move forward in that direction.
        if (robot.distanceBlockage > 400) {
            while ((robot.inside == false) && (robot.distanceBlockage > 400)) {
                SetPins_ToGoForwards();
                Motor_Action_Go_Full_Speed();
                delay(500);
                ADCMan.run();
                UpdateWireSensor();
                PrintBoundaryWireStatus();
                robot.distanceBlockage = PingSonarX(trigPin1, echoPin1, 1, 1, 1, 4, 0);
                delay(10);
                //Check_Wire_In_Out();
            }
            Motor_Action_Stop_Motors;
            ADCMan.run();
            UpdateWireSensor();
            PrintBoundaryWireStatus();
        }

        // if the sonar is measuring something less than 300cm then turn to the left and measure again
        // keep turning 'while function' until a path is open.
        while (robot.distanceBlockage < 300) {
            SetPins_ToTurnLeft();
            delay(200);
            Motor_Action_Turn_Speed();
            delay(100);
            robot.distanceBlockage = PingSonarX(trigPin1, echoPin1, 1, 1, 1, 4, 0);
            delay(10);
        }
        Motor_Action_Stop_Motors;
        ADCMan.run();
        UpdateWireSensor();
        PrintBoundaryWireStatus();
    }

    Console.println("Mower is now back inside the wire......?");
    lcd.clear();
    lcd.print("Mower now");
    lcd.setCursor(0, 1);
    lcd.print("Inside Wire?");
    robot.outsideWireCount = 0;
    //FindWireTrack();
}




/*****************
 * SPECIAL....mmm TODO
 */
void Special_Cut_Under_Trampoline_Function() {
  //Enter Code Here
}



// after wire tracking the code to bering the mower to a sensible position away from the wire
// This poart of the code should eb modified to fit your garden needs
void Special_Move_Into_Garden_Zone_X() {
    Serial.println("start mower code here");
    Motor_Action_Stop_Motors();                                     // Stop the wheel motors
    SetPins_ToTurnLeft();                                           // Turn left
    Motor_Action_Go_Full_Speed();                                       
    delay(1400);                                                     // Turn left for 0.7seconds
    Motor_Action_Stop_Motors();                                     // Stop the wheel motors.
    SetPins_ToGoForwards();                                         // Get ready to move off
    }

void Special_Exit_From_Docking_Station() {
    lcd.clear();
    lcd.print("Exiting Garage");
    delay(2000);
    SetPins_ToGoBackwards();                                        // Prepare motors pins to go Backwards
    Motor_Action_GoFullSpeed_Out_Garage();                          // Turn the wheels
    Serial.print("Left Wheel PWM:");
    Serial.print(PWM_MaxSpeed_LH);
    Serial.print("|");
    Serial.print("Right Wheel PWM:");
    Serial.println(PWM_MaxSpeed_RH);   
    delay (4000);                                                   // Backwards time
    Motor_Action_Stop_Motors;                                       // Stop
    SetPins_ToTurnLeft();                                           // Prepare motors to turn left
    Motor_Action_Turn_Speed();                                      // Turn the wheels
    delay(500);                                                    // Turn time
    Motor_Action_Stop_Motors;                                       // Stop
    SetPins_ToGoBackwards();                                        // Set again to go backwards
    Motor_Action_Go_Full_Speed();                                   // Turn the wheels
    delay (500);                                                    // Backwards Time
    SetPins_ToTurnLeft();                                           // Set to go left             
    Motor_Action_Turn_Speed();                                      // Turn the wheels
    delay(200);                                                     // Turning time
    Motor_Action_Stop_Motors();                                     // Stop
    SetPins_ToGoForwards();                                         // Set to go wheel motor pins to go forwards
    Motor_Action_Stop_Motors();                                     // Stop / Park the mower here
    lcd.clear();                                                    // Clears the LCD display
    lcd.print("Garage Clear");                                      // Prints to the LCD screen       
    delay(500);
    lcd.clear();
    }



void Specials_Find_Wire_Track()  {

  Serial.println(F("Lost Mower - find wire Track"));
  lcd.clear();
  lcd.print("Finding Wire...  ");
  Motor_Action_Stop_Spin_Blades();
  delay(5);
  robot.abortWireFind = 0;
  robot.noWireFound = 0;
  bool wireOn = isWireOn();// TODO, non viene mai usato...

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
    if ( robot.inside == false) {                                    // If the mower is outside the wire then run the following code.
      ADCMan.run();
      UpdateWireSensor();
      PrintBoundaryWireStatus();
      Motor_Action_Stop_Motors();                                                           // Stop all wheel motion
      delay(1000);
      SetPins_ToGoBackwards();                                                              // Set the mower to back up
      delay(100);
      lcd.clear();
      lcd.print("Backwards Try...  ");
      lcd.setCursor(0,1);
      lcd.print("Finding Wire  ");
      delay(100);
      while (( robot.inside != true) && (robot.abortWireFind == 0) ){                                 // While the mower is still outside the fence run this code
        Motor_Action_Go_Full_Speed();                                                       // Go full speed (in this case backwards)
        UpdateWireSensor();                                                                 // Read the wire sensor and see of the mower is now  or outside the wire
        ADCMan.run();
        PrintBoundaryWireStatus();                                                          // Prints of the status of the wire sensor readings.
        Serial.println("");
        }
      }
      
      Motor_Action_Stop_Motors();
      delay(5);
      }

    // Code to go forwards until the mower is outside/ON the wire
    if ( robot.inside == true) {             // If the Mower is situated  the wire then run the following code.
        ADCMan.run();
        UpdateWireSensor();
        Serial.println(F("CODE POSITION - MOTOR FORWARDS LOOP:  If statements"));
        PrintBoundaryWireStatus();
        Motor_Action_Stop_Motors();
        delay(1000);  
        SetPins_ToGoForwards();                                                             // Set the motors to move the mower forwards
        delay(100);
        lcd.clear();
        lcd.print("Forward Try...  ");
        lcd.setCursor(0,1);
        lcd.print("Finding Wire  ");
        delay(100);
        int cycle = 0;                                                                      // resets the cycles
        while ( robot.inside != false) {                               // Move the mower forward until mower is outisde/ON the wire fence or 500 cycles have passed
          cycle = cycle + 1;
          Motor_Action_Go_Full_Speed();                                                     // Go full speed (in this case forwards)
          UpdateWireSensor();                                                               // Read the wire sensor and see of the mower is now  or outside the wire
          ADCMan.run();
          PrintBoundaryWireStatus();                                                        // Prints of the status of the wire sensor readings.
          }
          if (cycle > Max_Cycle_Wire_Find) {                                                                // Track forwards for 500 cycles
            robot.noWireFound = 1;                                                              // if mower is still tracking after 500 cycles then cancel the find.
            Serial.println("Max Forward Cycles reached");
          }
        }
      Motor_Action_Stop_Motors();
      delay(5);
    }
  
