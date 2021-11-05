#include "compassUtils.h"

#include "MPU6050_light.h"
MPU6050 mpu(Wire);
//#include "DFRobot_QMC5883.h"
//DFRobot_QMC5883 compass;
#include "lcdDisplay.h"
#include "motorsController.h"
#include "robot.h"

void compassInit() {
    if (COMPASS_ACTIVATE == 1) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Compass  ");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Setup..");
        Serial.println("Setup Compass");
        while (mpu.begin() != 0) {
            Serial.println(F("Could not find a valid MPU6050 sensor, check wiring!"));
            robot.lcdDisplay.setCursor(0, 1);
            robot.lcdDisplay.print("Error..");
            delay(500);
        }

        /*
        TODO vecchia logica con QMC5883

        while (!mpu.begin()) {
            Serial.println(F("Could not find a valid MPU6050 sensor, check wiring!"));
            delay(500);
        }

        if (compass.isHMC()) {
            Serial.println(F("Initialize HMC5883"));
            compass.setRange(HMC5883L_RANGE_1_3GA);
            compass.setMeasurementMode(HMC5883L_CONTINOUS);
            compass.setDataRate(HMC5883L_DATARATE_15HZ);
            compass.setSamples(HMC5883L_SAMPLES_8);
        } else if (compass.isQMC()) {
            Serial.println(F("Initialize QMC5883"));
            compass.setRange(QMC5883_RANGE_2GA);
            compass.setMeasurementMode(QMC5883_CONTINOUS);
            compass.setDataRate(QMC5883_DATARATE_50HZ);
            compass.setSamples(QMC5883_SAMPLES_8);
        }

        // Set declination angle on your location and fix heading
        // You can find your declination on: http://magnetic-declination.com/
        // (+) Positive or (-) for negative
        // For Bytom / Poland declination angle is 4'26E (positive)
        // Formula: (deg + (min / 60.0)) / (180 / PI);
        //Italy is 3.55
        float declinationAngle = (3.55 + (26.0 / 60.0)) / (180 / PI);
        compass.setDeclinationAngle(declinationAngle);

        */

        delay(500);
        mpu.calcOffsets();

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

void resetCompassOffset() {
    delay(500);
    mpu.calcOffsets();
}

/* Calculates the compass heading as heading & degrees of the onboard compass */
void readRobotCompassDegrees() {
    mpu.update();

    robot.compassHeadingDegrees = mpu.getAngleX();
    if (robot.compassHeadingDegrees < 0) {
        robot.compassHeadingDegrees = robot.compassHeadingDegrees + 360;
    }
    /*
    Vector norm = compass.readRaw();

    Vector mag = compass.readRaw();
    compass.getHeadingDegrees();
    robot.compassHeadingDegrees = mag.HeadingDegress;
    */
    Serial.print(F("Comp°:"));
    Serial.print(robot.compassHeadingDegrees);
    Serial.print("|");
    //TODO serve veramente ? delay(5);
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
    robot.lcdDisplay.setCursor(0, 0);
    robot.lcdDisplay.setCursor(0, 1);
    robot.lcdDisplay.print("GYRO    ->    ");
    robot.lcdDisplay.setCursor(10, 1);
    robot.lcdDisplay.print((int)compassTarget);

    float minTarget = compassTarget - 5;  //TODO rinomina in min e max
    float maxTarget = compassTarget + 5;

    //todo >0 e <360

    char cycleCount = 0;
    char MAX_TURN_CYCLE = 80; //each cycle in 300ms
    float compassDiff = 0;
    char turnSpeed = 0;

    while (cycleCount < MAX_TURN_CYCLE  //max attempt
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

        readRobotCompassDegrees();
        if (compassDiff < 0) {
            Console.print("Turn_right|");
            motorsSetPinsToTurnRight();
            delay(100);
        } else {
            Console.print("Turn_left|");
            motorsSetPinsToTurnLeft();
            delay(100);
        }

        if (compassDiff < 10) turnSpeed = 120;
        if (compassDiff < 20) turnSpeed = 100;
        if (compassDiff < 50) turnSpeed = 80;
        if (compassDiff < 180) turnSpeed = 20;

        readRobotCompassDegrees();
        motorsetTurnSpeed(turnSpeed);  // Sets the speed of the turning motion
        delay(100);
        cycleCount++;

        Console.println(" ");
    }

    if (cycleCount >= MAX_TURN_CYCLE) {
        //robot can't turn
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
    }else{
        Serial.print("Steer_Left|");
        robot.pwmRight = PWM_MAX_SPEED_RH;                                              // Keep the Left wheel at full power calibrated to go straight
        robot.pwmLeft = PWM_MAX_SPEED_LH - (COMPASS_CORRECTION_POWER * Compass_Error);  // Multiply the difference by D to increase the power then subtract from the PWM
        if (robot.pwmLeft < 0) robot.pwmLeft = PWM_MAX_SPEED_LH - 50;
    }
}
