#include "wireDetection.h"
#include "robot.h"
#include "lcdDisplay.h"
#include "adcman.h"
#include "motorsController.h"
#include "movementsUtils.h"



// Check the mower is inside (0) or outside (1) the perimeter wire
void Check_Wire_In_Out() {
    // If the perimeter wire sensor is de-activated
    if (isPerimeterWireEnabled == 0) {
    }

    // If the perimeter wire sensor is activated
    if (isPerimeterWireEnabled == 1) {
        UpdateWireSensor();  // Read the wire sensor and see of the mower is now  or outside the wire
        ADCMan.run();
        PrintBoundaryWireStatus();

        // OUTSIDE the wire
        if ((robot.perimeter.isInside(0)) == 0) {                      // Mower is OUTSIDE the wire
            robot.outsideWire = 1;                                    // Outside wire variable is tuend on.
            if (robot.mowerRunning == 1) Motor_Action_Stop_Motors();  // Stop immediatley the wheel motors
            Print_LCD_Wire();                                    // Update the LCD screem
            robot.outsideWireCount = robot.outsideWireCount + 1;         // Count the number of times the mower is consecutiley outside the wire
                                                                 // If a certain number is reached its assumed thw mower is lost outside the wire.
        } else {
            //ho messo un else....perchè non aveva tanto senso 2 if
            // INSIDE the wire
            //if ( (robot.perimeter.isInside(0)) == 1 )   {

            robot.outsideWire = 0;        // Outside wire variable is tuend off
            robot.outsideWireCount = 0;  // The number of outside wire counts is reset to 0
            robot.wireRefindTries = 0;
        }
    }

    // If the mower consectivley detects its outside the wire a number of times
    // its assumed the mower is lost and a this search function is started to try and
    // re-find the wire.  Otherwise the mower is hibernated.
    if (robot.outsideWireCount >= Outside_Wire_Count_Max) {
        if (Action_On_Over_Wire_Count_Max == 1) Manouver_Hibernate_Mower();               // Put the mower to sleep and wait
        if (Action_On_Over_Wire_Count_Max == 2) Manouver_Outside_Wire_ReFind_Function();  // re-find Garden using Sonar 1 and wire detect

        if (Action_On_Over_Wire_Count_Max == 3) {  // try to locate the wire using wire find function

            lcd.clear();
            lcd.print("Wire Find");
            lcd.setCursor(0, 1);
            lcd.print("Special Function");

            robot.loopCycleMowing = 999;
            delay(2000);
            robot.outsideWireCount = 0;
            Specials_Find_Wire_Track();
            SetPins_ToGoBackwards();  // Set the mower to back up
            Motor_Action_Go_Full_Speed();
            delay(1000);
            Motor_Action_Stop_Motors();
            UpdateWireSensor();  // Read the wire sensor and see of the mower is now  or outside the wire
            ADCMan.run();
            PrintBoundaryWireStatus();
            delay(1000);
            UpdateWireSensor();  // Read the wire sensor and see of the mower is now  or outside the wire
            ADCMan.run();
            PrintBoundaryWireStatus();
            robot.wireRefindTries = robot.wireRefindTries + 1;
            robot.loopCycleMowing = 0;
            Serial.println("");
            Serial.print("|Wire Refind Atempts:");
            Serial.print(robot.wireRefindTries);
            Serial.print("|");
            Serial.println("");
            if (robot.wireRefindTries > 4) {
                Motor_Action_Stop_Motors();
                lcd.clear();
                robot.mowerError = 1;
                Serial.println("");
                Serial.println("Max refind tries exceeded - Parking the Mower");
                delay(2000);
            }
        }
    }
}

/**
 * Check that boundary wire is turned on
 */
bool isWireOn() {
    bool wireDetected = false;

    ADCMan.run();
    UpdateWireSensor();

    if (isPerimeterWireEnabled == 1) {  // Perimeter use is ON - Perimter_USE can be turned on or off in the setup.

        // Checks the MAG field of the boundary wire
        robot.magNow = robot.perimeter.getMagnitude(0);

        // If the MAG field is very low between these values we can assume the wire is off
        if (robot.magNow > -MIN_WIRE_MAG && robot.magNow < MIN_WIRE_MAG) {
            wireDetected = false;  // Wire not detected
            Print_LCD_NO_Wire();
            robot.wireOffCounter = robot.wireOffCounter + 1;

            if (robot.wireOffCounter > MAX_WIRE_FAIL && !robot.mowerDocked && !robot.mowerParked) {
                Serial.println(F("Wire Test Failed - Hibernating Mower"));
                Manouver_Hibernate_Mower();
            }

        } else {
            wireDetected = true;  // Wire is detected
            robot.wireOffCounter = 0;   // Resets the fail counter
            Print_LCD_Wire_ON();
        }

        Serial.print(F("|Wire"));
        Serial.print(F(":"));
        if (wireDetected) {
            Serial.print(F("OFF|"));
        } else {
            Serial.print(F("ON|"));
        }

    } else {
        //Wire not enable

        Serial.print(F("Wire"));
        Serial.print(F(":"));
        Serial.print(F("DISABLED|"));
        wireDetected = true;
    }

    return wireDetected;
}

void UpdateWireSensor() {
    if (millis() >= robot.nextTime) {
        robot.nextTime = millis() + 50;
        if (robot.perimeter.isInside(0) != robot.inside) {
            robot.inside = robot.perimeter.isInside(0);
            robot.counter++;
        }
    }
}

void PrintBoundaryWireStatus() {
    Serial.print(F("|IN/OUT:"));
    Serial.print((robot.perimeter.isInside(0)));
    Serial.print(F("|Mag:"));
    Serial.print((int)robot.perimeter.getMagnitude(0));
    Serial.print(F("|Smag:"));
    Serial.print((int)robot.perimeter.getSmoothMagnitude(0));
    Serial.print(F("|Q:"));
    Serial.print((robot.perimeter.getFilterQuality(0)));
    Serial.print(F("|"));
}
