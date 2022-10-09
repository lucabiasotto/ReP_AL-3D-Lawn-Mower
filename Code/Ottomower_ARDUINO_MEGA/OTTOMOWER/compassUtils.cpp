#include "compassUtils.h"

#include <Arduino.h>
//#include "MPU6050_light.h"
// MPU6050 mpu(Wire);
//#include "DFRobot_QMC5883.h"
// DFRobot_QMC5883 compass;
#include <Wire.h>

#include "MPU6050.h"
#include "lcdDisplay.h"
#include "motorsController.h"
#include "robot.h"
MPU6050 mpu;

int pitch = 0;
int roll = 0;
static float yaw = 0;
static unsigned long mpu6050LastReadMS = 0;

// DOCS: https://github.com/ElectronicCats/mpu6050/blob/master/examples/MPU6050_DMP6/MPU6050_DMP6.ino
// DOCS2: https://www.giuseppecaccavale.it/arduino/roll-pitch-e-yaw-con-mpu6050-arduino/

unsigned int resetCount __attribute__((section(".noinit")));

/**
   Function for reset arduino
*/
void (*reset)(void) = 0;

/**
 * Setup compass
 */
void compassInit() {
    if (COMPASS_ACTIVATE == 1) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Compass  ");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Setup..");
        Serial.println("Setup Compass");

        while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
            Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
            delay(500);
        }

        // Calibrate gyroscope. The calibration must be at rest.
        // If you don't want calibrate, comment this line.
        mpu.calibrateGyro();

        // Set threshold sensivty. Default 3.
        // If you don't want use threshold, comment this line or set 0.
        mpu.setThreshold(1);

        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Setup...");

        // Check settings
        Serial.print(" * Sleep Mode:        ");
        Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");

        Serial.print(" * Clock Source:      ");
        switch (mpu.getClockSource()) {
            case MPU6050_CLOCK_KEEP_RESET:
                Serial.println("Stops the clock and keeps the timing generator in reset");
                break;
            case MPU6050_CLOCK_EXTERNAL_19MHZ:
                Serial.println("PLL with external 19.2MHz reference");
                break;
            case MPU6050_CLOCK_EXTERNAL_32KHZ:
                Serial.println("PLL with external 32.768kHz reference");
                break;
            case MPU6050_CLOCK_PLL_ZGYRO:
                Serial.println("PLL with Z axis gyroscope reference");
                break;
            case MPU6050_CLOCK_PLL_YGYRO:
                Serial.println("PLL with Y axis gyroscope reference");
                break;
            case MPU6050_CLOCK_PLL_XGYRO:
                Serial.println("PLL with X axis gyroscope reference");
                break;
            case MPU6050_CLOCK_INTERNAL_8MHZ:
                Serial.println("Internal 8MHz oscillator");
                break;
        }

        Serial.print(" * Gyroscope:         ");
        switch (mpu.getScale()) {
            case MPU6050_SCALE_2000DPS:
                Serial.println("2000 dps");
                break;
            case MPU6050_SCALE_1000DPS:
                Serial.println("1000 dps");
                break;
            case MPU6050_SCALE_500DPS:
                Serial.println("500 dps");
                break;
            case MPU6050_SCALE_250DPS:
                Serial.println("250 dps");
                break;
        }

        mpu6050LastReadMS = millis();

        Serial.print(" * Gyroscope offsets: ");
        Serial.print(mpu.getGyroOffsetX());
        Serial.print(" / ");
        Serial.print(mpu.getGyroOffsetY());
        Serial.print(" / ");
        Serial.println(mpu.getGyroOffsetZ());
        Serial.println();

        // reset the mower when powered because some time MPU6050 some time does not work
        if (resetCount == 1) {
            // 1 is the magic number used to flag when i'm after a reset
            Serial.println("AFTER RESET");
            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.print("Setup....");
            delay(500);
        } else {
            // i need to do a reset because sometimes the MPU6050 not works at first boot
            Serial.println("FIRST POWER UP");
            resetCount = 1;

            robot.lcdDisplay.clear();
            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.print("Restart compass");
            delay(2000);

            reset();
        }

        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Compass Setup ");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Done!             ");
        delay(500);
        robot.lcdDisplay.clear();
    } else {
        Serial.println(F("Compass Switched off - Select 1 in setup to switch on."));
    }
}

/**
 *  Calculates the compass heading as heading & degrees of the onboard compass
 * */
void readRobotCompassDegrees() {
    long currentMS = millis();

    float tempYaw = yaw;

    if (currentMS - mpu6050LastReadMS <= 10) {
        // to infinity and beyond
        return;
    }

    Vector normAccel = mpu.readNormalizeAccel();
    Vector normGyro = mpu.readNormalizeGyro();

    // Calculate Pitch & Roll
    pitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis * normAccel.YAxis + normAccel.ZAxis * normAccel.ZAxis)) * 180.0) / M_PI;
    roll = (atan2(normAccel.YAxis, normAccel.ZAxis) * 180.0) / M_PI;

    // Ignore the gyro if our angular velocity does not meet our threshold
    if (normGyro.ZAxis > 1 || normGyro.ZAxis < -1) {
        normGyro.ZAxis /= (1000 / (currentMS - mpu6050LastReadMS));  //è 1000/delay_da_ultima_lettura
        tempYaw += normGyro.ZAxis;
    }

    // Keep our angle between 0-359 degrees
    if (tempYaw < 0) {
        tempYaw += 360;
    } else if (tempYaw > 359) {
        tempYaw -= 360;
    }

    if (!isnan(tempYaw) && !isinf(tempYaw)) {
        yaw = tempYaw;
        robot.compassHeadingDegrees = 360 - yaw;  // clockside direction
    }else{
        Serial.println("");
        Serial.println("YAW NAN found!!");
    }

    mpu6050LastReadMS = currentMS;
}

// Turns the Mower to the correct orientation for the optimum home wire track
// Avoiding tracking around the whole wire to get back to the docking station
void compassTurnMowerToHomeDirection() {
    motorsStopWheelMotors();
    delay(1000);

    lcdPrintSearchBoxDirectionWithCompass();
    delay(2000);

    // TODO fare 360-HOME_WIRE_COMPASS_HEADING in base al ciclo pari o dispari?
    turnToCompassTarget(HOME_WIRE_COMPASS_HEADING);
}

void turnToCompassTarget(float compassTarget) {
    // Step turns the mower to the left while the heading is outside the home tolerance
    // Once the heading is found. the mower stops and can then activate the find wire function

    /*
    es lcd
    GYRO
    120°->230°
    */
    robot.lcdDisplay.clear();
    robot.lcdDisplay.setCursor(0, 1);
    robot.lcdDisplay.print("GYRO    ->    ");
    robot.lcdDisplay.setCursor(10, 1);
    robot.lcdDisplay.print((int)compassTarget);

    const char DEGREE_TOLLERANCE = 10;                    // 5
    float minTarget = compassTarget - DEGREE_TOLLERANCE;  // TODO rinomina in min e max
    float maxTarget = compassTarget + DEGREE_TOLLERANCE;

    const long MAX_TURN_TIME_MS = 30000;  // each cycle in 300ms
    float compassDiff = 0;
    char turnCompensation = 0;

    const char MAX_SPEED_COMPENSATION = 100;  // 80
    const char MID_SPEED_COMPENSATION = 125;  // 100
    const char MIN_SPEED_COMPENSATION = 150;  // 120

    unsigned long endTime = millis() + MAX_TURN_TIME_MS;

    char startDirection = 0;

    Serial.println(" ");
    while (millis() < endTime  // max attempt
           && (robot.compassHeadingDegrees < minTarget || robot.compassHeadingDegrees > maxTarget)) {
        readRobotCompassDegrees();
        turnCompensation = 0;

        robot.lcdDisplay.setCursor(5, 1);
        if (robot.compassHeadingDegrees < 10) {
            robot.lcdDisplay.print("  ");
            robot.lcdDisplay.setCursor(7, 1);
        } else { /*if (robot.compassHeadingDegrees < MID_SPEED_COMPENSATION) {*/
            robot.lcdDisplay.print(" ");
            robot.lcdDisplay.setCursor(6, 1);
        }
        robot.lcdDisplay.print((int)robot.compassHeadingDegrees);

        compassDiff = robot.compassHeadingDegrees - compassTarget;

        Serial.print("|Compass_Target:");
        Serial.print(compassTarget);
        Serial.print("|Compass_Current:");
        Serial.print(robot.compassHeadingDegrees);
        Serial.print("|");

        // turn in the most shoart direction
        if (compassDiff > 180 || startDirection == 1) {
            startDirection = 1;
            Serial.print("Turn_right|");
            motorsSetPinsToTurnRight();

            if (compassDiff > 350) {
                turnCompensation = MIN_SPEED_COMPENSATION;
            } else if (compassDiff > 340) {
                turnCompensation = MID_SPEED_COMPENSATION;
            } else {
                turnCompensation = MAX_SPEED_COMPENSATION;
            }

        } else if (compassDiff < -180 || startDirection == 2) {
            startDirection = 2;
            Serial.print("Turn_left|");
            motorsSetPinsToTurnLeft();

            if (compassDiff < -350) {
                turnCompensation = MIN_SPEED_COMPENSATION;
            } else if (compassDiff < -340) {
                turnCompensation = MID_SPEED_COMPENSATION;
            } else {
                turnCompensation = MAX_SPEED_COMPENSATION;
            }

        } else if (compassDiff < 0 || startDirection == 3) {
            startDirection = 3;
            Serial.print("Turn_right|");
            motorsSetPinsToTurnRight();

            if (compassDiff > -10) {
                turnCompensation = MIN_SPEED_COMPENSATION;
            } else if (compassDiff > -20) {
                turnCompensation = MID_SPEED_COMPENSATION;
            } else {
                turnCompensation = MAX_SPEED_COMPENSATION;
            }

        } else if (startDirection == 4) {
            startDirection = 4;
            // compassDiff > 0
            Serial.print("Turn_left|");
            motorsSetPinsToTurnLeft();

            if (compassDiff < 10) {
                turnCompensation = MIN_SPEED_COMPENSATION;
            } else if (compassDiff < 20) {
                turnCompensation = MID_SPEED_COMPENSATION;
            } else {
                turnCompensation = MAX_SPEED_COMPENSATION;
            }
        }

        motorsReduceTurnSpeed(turnCompensation);  // Sets the speed of the turning motion
        delay(100);                               // TODO serve o no?

        Serial.println(" ");
    }

    if (millis() > endTime) {
        // robot can't turn
        robot.turnOffMotorsRelay();
        motorsStopWheelMotors();
        motorsStopSpinBlades();
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print("GYRO ERROR");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print(TRS_ROBOT_LOCK);
        while (true) {
            // lock robot
        }
    }

    // Once the while loop is satisfied (compass measures a degree between Lower and Upper, stop the mower
    motorsStopWheelMotors();
    motorsSetPinsToGoForwards();
    delay(1000);
    robot.lcdDisplay.clear();
}

void Display_Compass_Current_Heading_on_LCD() {
    robot.lcdDisplay.setCursor(5, 1);
    robot.lcdDisplay.print("    ");
    robot.lcdDisplay.print(robot.compassHeadingDegrees);
    delay(10);
}

void keepDirectionWithCompassWheelCompensation() {
    float compassDiff = robot.compassHeadingDegrees - robot.headingLock;  // Calculates the error in compass heading from the saved lock heading

    if (compassDiff > 180 || compassDiff < -180) {
        compassDiff = compassDiff * -1;
    }

    Serial.print(F("|compassHeadingDegrees:"));
    Serial.print(robot.compassHeadingDegrees);
    Serial.print(F("|headingLock:"));
    Serial.print(robot.headingLock);
    Serial.print(F("|compass_diff:"));
    Serial.print(compassDiff);

    if (compassDiff < 0) {  // Steer left
        Serial.print("|Steer_Right|");
        robot.pwmRight = PWM_MAX_SPEED_RH + (COMPASS_CORRECTION_POWER * compassDiff);  // Multiply the difference by D to increase the power then subtract from the PWM
        if (robot.pwmRight < 0) {
            robot.pwmRight = 0;
        }
        robot.pwmLeft = PWM_MAX_SPEED_LH;  // Keep the Right wheel at full power calibrated to go straight
    } else {
        Serial.print("|Steer_Left|");
        robot.pwmRight = PWM_MAX_SPEED_RH;                                            // Keep the Left wheel at full power calibrated to go straight
        robot.pwmLeft = PWM_MAX_SPEED_LH - (COMPASS_CORRECTION_POWER * compassDiff);  // Multiply the difference by D to increase the power then subtract from the PWM
        if (robot.pwmLeft < 0) {
            robot.pwmLeft = 0;
        }
    }
}
