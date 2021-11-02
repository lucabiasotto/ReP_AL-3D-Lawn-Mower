#include "wireTracking.h"
#include "robot.h"
#include "motorsController.h"
#include "batteryUtils.h"
#include "adcman.h"
#include "compassUtils.h"
#include "movementsUtils.h"

//  Prints a visual display of the wire tracking in the Serial Monitor
//  to see how well the wire is being followed.  Adjusting the P value in the settings
//  can improve the wire tracking ability of the mower.

void PrintWirePosition() {
    robot.printMAG_Now = robot.magNow / SCALE;
    robot.printInMax = WIRE_IN_MAX / SCALE;
    robot.printInMid = WIRE_IN_MID / SCALE;
    robot.printInMin = WIRE_IN_MIN / SCALE;
    robot.printOutMin = WIRE_OUT_MIN / SCALE;
    robot.printOutMid = WIRE_OUT_MID / SCALE;
    robot.printOutMax = WIRE_OUT_MAX / SCALE;

    for (int i = robot.printInMax; i <= robot.printOutMax; i++) {
        if (i == robot.printInMax) Serial.print(F("|"));
        if (i == robot.printInMid) Serial.print(F("|"));
        if (i == robot.printInMin) Serial.print(F("|"));
        if (i == robot.printOutMin) Serial.print(F("|"));
        if (i == robot.printOutMid) Serial.print(F("|"));
        if (i == robot.printOutMax) Serial.print(F("|"));
        if (i == robot.printMAG_Now) Serial.print(F("X"));  //maybe change to MAG_Lasr...
        if (i == 0)
            Serial.print(F("0"));

        else
            Serial.print(F("."));
    }
    Serial.print(F("|robot.magNow:"));
    Serial.print(robot.magNow);
    Serial.print(F("|"));
}


// Function to follow the wire for a specific amount of time set by the itterations 'I'
// Robot tracks the wire until the itterations are exhausted.
void Track_Wire_From_Dock_to_Zone_X() {
    delay(100);
    ADCMan.run();
    if (robot.bladeOverride == 1) {
        Motor_Action_Spin_Blades();
    }
    robot.lcdDisplay.setCursor(0, 0);
    robot.lcdDisplay.print("Exit Docking to");  // into the garden at a good position to start Mowing
    robot.lcdDisplay.setCursor(2, 1);
    delay(1000);  // Prints info to LCD display

    robot.trackingTurnRight = 0;  // resets the tracking errors for LH and RH.
    robot.trackingTurnLeft = 0;

    //uses the PID settings in the setup
    Serial.print(F("P = "));
    Serial.print(robot.P);
    /*
    TODO ERA SOLO LOGGATO...
    Serial.print(F(" / D = "));
    Serial.print(D);*/
    Serial.println(F(""));
    robot.trackingWire = 1;
    robot.magNow = 0;  // Reset Values
    robot.magStart = 0;
    robot.magError = 0;
    robot.magGoal = 0;
    delay(500);
    delay(5);
    robot.lcdDisplay.setCursor(10, 1);
    robot.lcdDisplay.print("0");
    for (robot.I = 0; robot.I < robot.trackWireItterations; robot.I++) {  // Iterations 'I' before mower leaves the wire.
        if (robot.mowerParked == 0) {
            delay(5);
            ADCMan.run();
            robot.magStart = robot.perimeter.getMagnitude(0);  // Gets the signal strength of the sensor
            robot.magNow = robot.magStart;
            delay(5);
            robot.magError = (robot.magGoal - robot.magStart);  // Calculates the Error to the center of the wire which is normally zero magnitude
            PrintWirePosition();                 // Prints the overview to the Serial Monitor.
            //Check_Wire_Blockage();                                              // homework idea is to avoid anything in the way

            // Tracks the wire from the docking station in a Counter-Clockwise direction to the start position

            if (CW_TRACKING_EXITING_CHARGE == 0) {
                if (robot.magError > 0) {  // If the robot.magError > 0 then turn right for CCW Tracking. PWM_left is set to max to turn right.
                    // TURN RIGHT
                    robot.pwmLeft = PWM_MAX_SPEED_LH;            // sets the PWM to the max possible for the wheel
                    robot.pwmRight = 255 - (robot.magError * robot.P);     // Mag_Error * robot.P is the value reduced from the max value set PWM and sent to the PWM
                    if (robot.pwmRight > 255) robot.pwmRight = 255;  // robot.pwmRight capped to Max PWM of 255.
                    if (robot.pwmRight >= 0) {
                        SetPins_ToGoForwards();
                        robot.lcdDisplay.setCursor(15, 1);
                        robot.lcdDisplay.print(" ");
                    }

                    if (robot.pwmRight < 0) {  // sets the mower to sharp turn to the right (wheels spin opposite) if the Error to the wire is large enough.
                        robot.pwmRight = (-1 * robot.pwmRight) + 220;
                        if (robot.pwmRight > 255) robot.pwmRight = 255;
                        if (robot.pwmRight >= 0) SetPins_ToTurnRight();
                        delay(5);
                        robot.lcdDisplay.setCursor(15, 1);
                        robot.lcdDisplay.print(">");
                    }

                    Motor_Action_Dynamic_PWM_Steering();  // Carries out the wheel PWM changes for steering on the wire
                    Serial.print(F(" Turn Right "));
                    robot.trackingTurnLeft = 0;                               // Failsafe if the mower looses the wire.  If the mower is commanded to turn left or right
                    robot.trackingTurnRight = robot.trackingTurnRight + 1;        // too many times it is assumed that the mower is spinning and cant get back on the wire.
                    if (robot.trackingTurnRight > MAX_TRACKING_TURN_RIGHT) {  // if this is detected then a function is ran to find the wire again.
                        Motor_Action_Stop_Motors();
                        robot.lcdDisplay.clear();
                        robot.lcdDisplay.print("Right Wheel");
                        robot.lcdDisplay.print("robot.trackingTurnRight");
                        delay(2000);
                        robot.lcdDisplay.clear();
                        Tracking_Restart_Blocked_Path();
                    }
                }
                if (robot.magError <= 0) {  // If the robot.magError < 0 then turn left for CCW Tracking
                    // TURN LEFT
                    robot.pwmRight = 255;                     // robot.pwmRight set to max to rotate the mower to the left.
                    robot.pwmLeft = 255 + (robot.magError * robot.P);    // + as mag_error is negative to adjust PWM
                    if (robot.pwmLeft > 255) robot.pwmLeft = 255;  // robot.pwmLeft capped to mex PWM of 255
                    if (robot.pwmLeft >= 0) {
                        SetPins_ToGoForwards();
                        robot.lcdDisplay.setCursor(0, 1);
                        robot.lcdDisplay.print(" ");
                    }

                    if (robot.pwmLeft < 0) {  // sets the mower to sharp turn to the left (wheels spin opposite) if the Error to the wire is large enough.
                        robot.pwmLeft = (-1 * robot.pwmLeft) + 220;
                        if (robot.pwmLeft > 255) robot.pwmLeft = 255;
                        SetPins_ToTurnLeft();
                        delay(5);
                        robot.lcdDisplay.setCursor(0, 1);
                        robot.lcdDisplay.print("<");
                    }

                    Motor_Action_Dynamic_PWM_Steering();
                    Serial.print(F(" Turn Left "));
                    robot.trackingTurnRight = 0;
                    robot.trackingTurnLeft = robot.trackingTurnLeft + 1;
                    if (robot.trackingTurnLeft > MAX_TRACKING_TURN_LEFT) {
                        Motor_Action_Stop_Motors();
                        robot.lcdDisplay.clear();
                        robot.lcdDisplay.print("Left Wheel");
                        robot.lcdDisplay.print("robot.trackingTurnLeft");
                        delay(2000);
                        robot.lcdDisplay.clear();
                        Tracking_Restart_Blocked_Path();
                    }
                }
                Serial.print(F(" : robot.magError="));
                Serial.println(robot.magError);
            } else {
                // Add Code here for CW tracking to the exit zone.
                // Use the code above for CCW tracking to the docking station
                // as a template./
            }
        }

        robot.lcdDisplay.setCursor(10, 1);
        robot.lcdDisplay.print(robot.I);
    }
    robot.lcdDisplay.clear();
    robot.trackingWire = 0;

    delay(5);
}

//  Track the Perimeter wire using a PID type method
//  This code tracks the boundary wire and by calculating the Perimeter Magnitude and calculating the distance to the center of the wire.
//  This error value of (position - center of wire) is mulitplied by the P value in the setup to send a PWM change to the left or right
//  wheel to bring the sensor back over the wire.*/
void followingWireToDock() {
    robot.lcdDisplay.clear();
    robot.lcdDisplay.print("Tracking to..");
    robot.lcdDisplay.setCursor(0, 1);
    robot.lcdDisplay.print("Charging Station");  // Prints info to LCD display

    Motor_Action_Stop_Spin_Blades();
    //TODO ?? Docked_Hits = 0;
    Check_if_Charging();  // Checks if an amperage is detected on the charge wire
    Check_if_Docked();

    Serial.println(F(" Tracking the wire to the Garage: "));  // Prints the PID values used.
    Serial.print(F("P = "));
    Serial.print(robot.P);
    /*
    TODO ERA SOLO LOGGATO...
    Serial.print(F(" / D = "));
    Serial.print(D);
    */
    Serial.println(F(""));

    robot.trackingWire = 1;
    robot.magNow = 0;  // Reset Values
    robot.magStart = 0;
    robot.magError = 0;
    robot.magGoal = 0;
    int Dock_Cycles = 0;
    delay(5);

    if (CW_TRACKING_SEARCHING_CHARGE == 1) {                  // Mower tracks the wire in a Counter Clockwise Direction
        Serial.println(F("TRACKING ---  CLOCKWISE"));  // With the same functions as above

        while ((robot.mowerDocked == 0) && (robot.mowerParked == 0)) {
            delay(5);
            robot.lcdDisplay.clear();
            robot.lcdDisplay.print("Cerco casa");
            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.print("");

            ADCMan.run();
            robot.magStart = robot.perimeter.getMagnitude(0);
            robot.magNow = robot.magStart;
            delay(5);
            robot.magError = (robot.magGoal - robot.magStart);
            PrintWirePosition();

            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.print(robot.magError > 0 ? "<--  Orario     " : "     Orario  -->");

            // Turn the Mower to the left to get back on the wire. Clock Wise Motion around the wire
            // Power down the left wheel and full power right wheel to turn left
            if (robot.magError > 0) {  // if robot.magError > 0 then Turn left in CW tracking
                // Turn LEFT
                robot.pwmRight = 255;  // Set the right wheel to max PWMto turn left
                robot.pwmLeft = 255 - (robot.magError * robot.P);
                if (robot.pwmLeft > 255) robot.pwmLeft = 255;  //
                if (robot.pwmLeft >= 0) {
                    SetPins_ToGoForwards();  // keep the mower moving forward
                    robot.lcdDisplay.setCursor(15, 0);
                    robot.lcdDisplay.print(" ");
                }

                if (robot.pwmLeft < 0) {
                    robot.pwmLeft = (-1 * robot.pwmLeft) + 220;
                    if (robot.pwmLeft > 255) robot.pwmLeft = 255;
                    robot.lcdDisplay.setCursor(15, 0);
                    robot.lcdDisplay.print("*");
                    SetPins_ToTurnLeft();
                    delay(5);
                }

                Motor_Action_Dynamic_PWM_Steering();  // send the PWM values to the motor driver.
                Serial.print(F(" Turn Left "));
                robot.trackingTurnRight = 0;
                robot.trackingTurnLeft = robot.trackingTurnLeft + 1;
                if (robot.trackingTurnLeft > MAX_TRACKING_TURN_LEFT) {
                    Tracking_Restart_Blocked_Path();
                }
            }

            if (robot.magError <= 0) {  // Turn the Mower to the right to get back on the wire.
                //Turn Right
                robot.pwmLeft = 255;
                robot.pwmRight = 255 + (robot.magError * robot.P);  // + as mag_error is negative to adjust PWM
                if (robot.pwmRight > 255) robot.pwmRight = 255;
                if (robot.pwmRight >= 0) {
                    SetPins_ToGoForwards();
                    robot.lcdDisplay.setCursor(15, 0);
                    robot.lcdDisplay.print(" ");
                }

                if (robot.pwmRight < 0) {
                    robot.pwmRight = (-1 * robot.pwmRight) + 220;
                    if (robot.pwmRight > 255) robot.pwmRight = 255;
                    if (robot.pwmRight >= 0) SetPins_ToTurnRight();
                    robot.lcdDisplay.setCursor(15, 0);
                    robot.lcdDisplay.print("*");
                }

                Motor_Action_Dynamic_PWM_Steering();
                Serial.print(F(" Turn Right "));
                robot.trackingTurnLeft = 0;
                robot.trackingTurnRight = robot.trackingTurnRight + 1;
                if (robot.trackingTurnRight > MAX_TRACKING_TURN_RIGHT) {
                    Tracking_Restart_Blocked_Path();
                }
            }

            Serial.print(F(" : robot.magError="));
            Serial.print(robot.magError);
            readVoltAmp(); //perchè?? TODO
            Check_if_Charging();
            Check_if_Docked();
            Dock_Cycles = Dock_Cycles + 1;
            if ((Dock_Cycles % 2) == 0) {
            }
        }

    } else {
        // Mower tracks the wire in a Counter Clockwise Direction
        Serial.println(F("TRACKING COUNTER-CLOCKWISE"));

        while (robot.mowerDocked == 0 && robot.mowerParked == 0) {
            robot.lcdDisplay.clear();
            robot.lcdDisplay.print("Cerco casa");
            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.print("");

            delay(5);

            ADCMan.run();
            robot.magStart = robot.perimeter.getMagnitude(0);  // Gets the signal strength of the sensor
            robot.magNow = robot.magStart;
            delay(5);

            robot.magError = (robot.magGoal - robot.magStart);  // Calculates the Error to the center of the wire which is normally zero magnitude (remember - - is + )
            PrintWirePosition();                 // Prints the overview to the Serial Monitor.

            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.print(robot.magError > 0 ? "<--Antiorario   " : "   Antiorario-->");

            if (robot.magError > 0) {  // Trun the mower to the right if robot.magError > 0 with a CCW track direction.
                // RIGHT TURN
                robot.pwmLeft = 255;                     // Set robot.pwmLeft to maximum
                robot.pwmRight = 255 - (robot.magError * robot.P);  // Mag_Error * robot.P is the value reduced from the max value set PWM and sent to the PWM

                if (robot.pwmRight > 255) {
                    robot.pwmRight = 255;  // Caps the robot.pwmRight to 255
                }

                if (robot.pwmRight >= 0) {
                    SetPins_ToGoForwards();
                    robot.lcdDisplay.setCursor(15, 0);
                    robot.lcdDisplay.print(" ");
                }

                if (robot.pwmRight < 0) {                     // Set the wheels to rotate around the axis if a sharp turn is required.
                    robot.pwmRight = (-1 * robot.pwmRight) + 220;  // change the negative value to a positive for the PWM input to the motor bridge.
                    if (robot.pwmRight > 255) {
                        robot.pwmRight = 255;  // cap the maximum PWM to 255
                    }
                    SetPins_ToTurnRight();  // set the motor bridge pins to turn left
                    robot.lcdDisplay.setCursor(15, 0);
                    robot.lcdDisplay.print("*");
                }

                Motor_Action_Dynamic_PWM_Steering();  // Carries out the wheel PWM changes for steering on the wire
                Serial.print(F(" Turn Right "));
                robot.trackingTurnLeft = 0;                               // Failsafe if the mower looses the wire.  If the mower is commanded to turn left or right
                robot.trackingTurnRight = robot.trackingTurnRight + 1;        // too many times it is assumed that the mower is spinning and cant get back on the wire.
                if (robot.trackingTurnRight > MAX_TRACKING_TURN_RIGHT) {  // if this is detected then a function is ran to find the wire again.
                    Tracking_Restart_Blocked_Path();
                }
            } else {
                //LEFT TURN
                robot.pwmRight = 255;                   // Set the robot.pwmRight to maximum
                robot.pwmLeft = 255 + (robot.magError * robot.P);  // + as mag_error is negative to adjust PWM

                if (robot.pwmLeft > 255) {
                    robot.pwmLeft = 255;  // cap robot.pwmLeft to the maximum
                }

                if (robot.pwmLeft >= 0) {
                    SetPins_ToGoForwards();
                    robot.lcdDisplay.setCursor(15, 0);
                    robot.lcdDisplay.print(" ");
                }

                if (robot.pwmLeft < 0) {                    // if the PWM calulated is below zero set the mower for a sharp turn
                    robot.pwmLeft = (-1 * robot.pwmLeft) + 220;  // make the negative value a positive one to input to the motor bridge
                    if (robot.pwmLeft > 255) {
                        robot.pwmLeft = 255;  // if again the PWM is above 255 then cap it to 255
                    }
                    SetPins_ToTurnLeft();  // set the pins to sharp turn left
                    delay(5);
                    robot.lcdDisplay.setCursor(15, 0);
                    robot.lcdDisplay.print("*");
                }

                Motor_Action_Dynamic_PWM_Steering();
                Serial.print(F(" Turn Left "));
                robot.trackingTurnRight = 0;
                robot.trackingTurnLeft = robot.trackingTurnLeft + 1;
                if (robot.trackingTurnLeft > MAX_TRACKING_TURN_LEFT) {
                    Tracking_Restart_Blocked_Path();
                    if (robot.mowerParked == 1) robot.mowerDocked = 1;
                }
            }  //fine LEFT TURN

            Serial.print(F(" : robot.magError="));
            Serial.print(robot.magError);
            readVoltAmp(); //TODO perchè?
            Check_if_Charging();
            Check_if_Docked();
            Dock_Cycles = Dock_Cycles + 1;

            //TODO quando esco dal while?



        }  //fine while
    }

    robot.trackingWire = 0;
}

//Starts an algorithym to find the wire again after it is lost in tracking
void Tracking_Restart_Blocked_Path() {
    Motor_Action_Stop_Motors();
    Serial.println(F("Possible Blocked Path - Trying to Avoid"));
    robot.lcdDisplay.clear();
    robot.lcdDisplay.print("Wire Lost.");
    robot.lcdDisplay.setCursor(0, 1);
    robot.lcdDisplay.print("Recovering.....");                // Prints info to LCD display
    if (robot.mowerParked != 1) {                     // If Pause has been pressed dont carry on.
        SetPins_ToGoBackwards();
        delay(500);
        Motor_Action_Go_Full_Speed();
        delay(5000);  //Reversing Time in seconds
        Motor_Action_Stop_Motors();
        delay(2000);
        robot.trackingTurnLeft = 0;   // Resets the tracking error counters
        robot.trackingTurnRight = 0;  // Resets the tracking error counters
        delay(500);
        if (COMPASS_ACTIVATE == 1) Compass_Turn_Mower_To_Home_Direction();
        findWire();
    }
}
