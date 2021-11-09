#include "compassUtils.h"

//#include "MPU6050_light.h"
//MPU6050 mpu(Wire);
//#include "DFRobot_QMC5883.h"
//DFRobot_QMC5883 compass;
#include <Wire.h>

#include "MPU6050.h"
#include "lcdDisplay.h"
#include "motorsController.h"
#include "robot.h"
MPU6050 mpu;

int pitch = 0;
int roll = 0;
float yaw = 0;
static unsigned long mpu6050LastReadMS = 0;

//DOCS: https://github.com/ElectronicCats/mpu6050/blob/master/examples/MPU6050_DMP6/MPU6050_DMP6.ino
//DOCS2: https://www.giuseppecaccavale.it/arduino/roll-pitch-e-yaw-con-mpu6050-arduino/

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

        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Setup....");
        delay(500);
        
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
   
    long currenttMS = millis();

    Vector normAccel = mpu.readNormalizeAccel();
    Vector normGyro = mpu.readNormalizeGyro();

    // Calculate Pitch & Roll
    pitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis * normAccel.YAxis + normAccel.ZAxis * normAccel.ZAxis)) * 180.0) / M_PI;
    roll = (atan2(normAccel.YAxis, normAccel.ZAxis) * 180.0) / M_PI;

    //Ignore the gyro if our angular velocity does not meet our threshold
    if (normGyro.ZAxis > 1 || normGyro.ZAxis < -1) {
        normGyro.ZAxis /= (1000 / (currenttMS - mpu6050LastReadMS));  //è 1000/delay_da_ultima_lettura
        yaw += normGyro.ZAxis;
    }

    //Keep our angle between 0-359 degrees
    if (yaw < 0){
        yaw += 360;
    }else if (yaw > 359){
        yaw -= 360;
    }

    mpu6050LastReadMS = currenttMS;

    /* 
    //log output
    Serial.print("Pitch = ");
    Serial.print(pitch);
    Serial.print("\tRoll = ");
    Serial.print(roll);
    Serial.print("\tYaw = ");
    Serial.print(yaw);
    Serial.println();
    */

    robot.compassHeadingDegrees = yaw;
    
    Serial.print(F("Comp°:"));
    Serial.print(robot.compassHeadingDegrees);
    Serial.print("|");
}

// Turns the Mower to the correct orientation for the optimum home wire track
// Avoiding tracking around the whole wire to get back to the docking station
void Compass_Turn_Mower_To_Home_Direction() {
    motorsStopWheelMotors();
    delay(1000);

    lcdPrintSearchBoxDirectionWithCompass();
    delay(2000);
    //TODO serve? Print_LCD_Heading_for_Home();
    delay(2000);

    robot.lcdDisplay.clear();
    readRobotCompassDegrees();
    motorsSetPinsToTurnLeft();
    Serial.print(F("Compass heading Now : "));
    Serial.println(robot.compassHeadingDegrees);
    Serial.println(F("********************************"));
    delay(100);

    robot.lcdDisplay.print(robot.compassHeadingDegrees);
    // This spins the mower a little to ensure a true compass reading is being read (calibration).
    motorsSetPinsToTurnLeft();  // Calls the motor function turn Left
    motorsetTurnSpeed(0);       // Sets the speed of the turning motion
    delay(500);

    motorsStopWheelMotors();
    readRobotCompassDegrees();
    robot.lcdDisplay.clear();
    robot.lcdDisplay.print(robot.compassHeadingDegrees);
    motorsSetPinsToTurnLeft();  // Calls the motor function turn Left

    delay(100);
    motorsetTurnSpeed(0);  // Sets the speed of the turning motion

    delay(2000);
    motorsStopWheelMotors();
    readRobotCompassDegrees();
    delay(500);

    robot.lcdDisplay.clear();
    robot.lcdDisplay.print("Compass Set");
    motorsStopWheelMotors();
    delay(2000);

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

    float minTarget = compassTarget - 5;  //TODO rinomina in min e max
    float maxTarget = compassTarget + 5;

    const long MAX_TURN_TIME_MS = 30000;  //each cycle in 300ms
    float compassDiff = 0;
    char turnSpeed = 0;

    unsigned long endTime = millis() + MAX_TURN_TIME_MS;

    while (millis() < endTime   //max attempt
           && (robot.compassHeadingDegrees < minTarget || robot.compassHeadingDegrees > maxTarget)) {
        readRobotCompassDegrees();

        robot.lcdDisplay.setCursor(5, 1);
        robot.lcdDisplay.print((int)robot.compassHeadingDegrees);
        compassDiff = robot.compassHeadingDegrees - compassTarget;

        Console.print("|Compass_Target:");
        Console.print(compassTarget);
        Console.print("|Compass_Current:");
        Console.print(robot.compassHeadingDegrees);
        Console.print("|");

        if (compassDiff < 0) {
            Console.print("Turn_right|");
            motorsSetPinsToTurnRight();
            //delay(100); //TODO serviva
        } else {
            Console.print("Turn_left|");
            motorsSetPinsToTurnLeft();
            //delay(100); //TODO serviva
        }

        if (compassDiff < 10) turnSpeed = 120;
        if (compassDiff < 20) turnSpeed = 100;
        if (compassDiff < 50) turnSpeed = 80;
        if (compassDiff < 180) turnSpeed = 20;

        motorsetTurnSpeed(turnSpeed);  // Sets the speed of the turning motion
        delay(100); //TODO serve o no?

        Console.println(" ");
    }

    if (millis() > endTime ) {
        //robot can't turn
        robot.turnOffMotorsRelay();
        motorsStopWheelMotors();
        motorsStopSpinBlades();
        robot.lcdDisplay.clear();
        robot.lcdDisplay.setCursor(0, 0);
        robot.lcdDisplay.print("GYRO ERROR");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print(TRS_ROBOT_LOCK);
        while (true) {
            //lock robot
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

void Calculate_Compass_Wheel_Compensation() {
    float Compass_Error = robot.compassHeadingDegrees - robot.headingLock;  // Calculates the error in compass heading from the saved lock heading

    if (Compass_Error > 180 || Compass_Error < -180) {
        Compass_Error = Compass_Error * -1;
    }

    Serial.print(F("C_Err:"));
    Serial.print(Compass_Error);
    Serial.print("|");

    if (Compass_Error < 0) {  // Steer left
        Serial.print("Steer_Right|");
        robot.pwmRight = PWM_MAX_SPEED_RH + (COMPASS_CORRECTION_POWER * Compass_Error);  // Multiply the difference by D to increase the power then subtract from the PWM
        if (robot.pwmRight < 0) robot.pwmRight = PWM_MAX_SPEED_RH - 50;
        robot.pwmLeft = PWM_MAX_SPEED_LH;  // Keep the Right wheel at full power calibrated to go straight
    } else {
        Serial.print("Steer_Left|");
        robot.pwmRight = PWM_MAX_SPEED_RH;                                              // Keep the Left wheel at full power calibrated to go straight
        robot.pwmLeft = PWM_MAX_SPEED_LH - (COMPASS_CORRECTION_POWER * Compass_Error);  // Multiply the difference by D to increase the power then subtract from the PWM
        if (robot.pwmLeft < 0) robot.pwmLeft = PWM_MAX_SPEED_LH - 50;
    }
}
