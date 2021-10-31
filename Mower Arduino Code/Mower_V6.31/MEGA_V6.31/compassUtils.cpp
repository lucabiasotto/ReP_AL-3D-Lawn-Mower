#include "compassUtils.h"
#include "robot.h"
#include "motorsController.h"
#include <DFRobot_QMC5883.h>
DFRobot_QMC5883 compass;
#include "lcdDisplay.h"

void Setup_Compass() {
    if (Compass_Activate == 1) {
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Compass  ");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Setup");
        Serial.println("Setup Compass");
        while (!compass.begin()) {
            Serial.println(F("Could not find a valid QMC5883 sensor, check wiring!"));
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
        robot.lcdDisplay.clear();
        robot.lcdDisplay.print("Compass Setup ");
        robot.lcdDisplay.setCursor(0, 1);
        robot.lcdDisplay.print("Done!             ");
        delay(500);
        robot.lcdDisplay.clear();
    }

    if (Compass_Activate == 0) {
        Serial.println(F("Compass Switched off - Select 1 in setup to switch on."));
    }
}

/* Calculates the compass heading as heading & degrees of the onboard compass */
void Get_Compass_Reading() {

  Vector norm = compass.readNormalize();
  delay(5);
  robot.heading = atan2(norm.YAxis, norm.XAxis);                        // Calculate heading

  // Set declination angle. Find your location declination on: http://magnetic-declination.com/
  // (+) Positive or (-) for negative,
  float Declination_Angle = (2.0 + (19.0 / 60.0)) / (180 / PI);   // Bad Krozingen is 2° 19'
  robot.heading += Declination_Angle;

  if (robot.heading < 0) {                                              // Correct for heading < 0deg and heading > 360deg
    robot.heading += 2 * PI;
  }
  if (robot.heading > 2 * PI) {
    robot.heading -= 2 * PI;
  }

  robot.compassHeadingDegrees = robot.heading * 180 / M_PI;                 // Convert to degrees
  //Serial.print(F("Comp H:"));
  //Serial.print(robot.heading);
  Serial.print(F("Comp°:"));
  Serial.print(robot.compassHeadingDegrees);
  Serial.print("|");
  delay(5);
}


// Turns the Mower to the correct orientation for the optimum home wire track
// Avoiding tracking around the whole wire to get back to the docking station
void Compass_Turn_Mower_To_Home_Direction() {
  Motor_Action_Stop_Motors;
  delay(1000);
  Print_LCD_Compass_Home();
  delay(2000);
  robot.compassTarget = Home_Wire_Compass_Heading;
  Print_LCD_Heading_for_Home();
  delay(2000);
  robot.lcdDisplay.clear();
  Get_Compass_Reading();
  SetPins_ToTurnLeft();
  Serial.print(F("Compass robot.heading Now : "));
  Serial.println(robot.compassHeadingDegrees);
  Serial.println(F("********************************"));
  delay(100);
  robot.lcdDisplay.print(robot.compassHeadingDegrees);
  // This spins the mower a little to ensure a true compass reading is being read (calibration).
  SetPins_ToTurnLeft();                                     // Calls the motor function turn Left
  Motor_Action_Turn_Speed();                                       // Sets the speed of the turning motion
  delay(500);
  Motor_Action_Stop_Motors();
  Get_Compass_Reading();
  robot.lcdDisplay.clear();
  robot.lcdDisplay.print(robot.compassHeadingDegrees);
  SetPins_ToTurnLeft();                                     // Calls the motor function turn Left
  delay(100);
  Motor_Action_Turn_Speed();                                       // Sets the speed of the turning motion
  delay(2000);
  Motor_Action_Stop_Motors();
  Get_Compass_Reading();
  delay(500);
  robot.lcdDisplay.clear();
  robot.lcdDisplay.print("Compass Set");
  Motor_Action_Stop_Motors();
  delay(2000);
  Turn_To_Compass_Heading();
}


void Turn_To_Compass_Heading()  {
  // Step turns the mower to the left while the heading is outside the home tolerance
  // Once the heading is found. the mower stops and can then activate the find wire function

  robot.lcdDisplay.setCursor(0, 0);
  robot.lcdDisplay.print("Target: ");
  robot.lcdDisplay.print(robot.compassTarget);
  robot.lcdDisplay.setCursor(0, 1);
  robot.lcdDisplay.print("Now:");
  robot.lcdDisplay.setCursor(0, 4);
  robot.headingLowerLimitCompass = robot.compassTarget - 5;
  robot.headingUpperLimitCompass = robot.compassTarget + 5;
  delay(500);
  int Cancel = 0;
  while ((robot.compassHeadingDegrees < robot.headingLowerLimitCompass) || (robot.compassHeadingDegrees > robot.headingUpperLimitCompass) && (Cancel < 40))  {

    Serial.print(F("Turning to Target:"));
    Serial.print(robot.compassTarget);
    Serial.print(F("|"));
    Get_Compass_Reading();
    delay(50);
    robot.lcdDisplay.setCursor(0, 0);
    robot.lcdDisplay.print("Degrees: ");
    robot.lcdDisplay.print(robot.compassHeadingDegrees);
    Serial.println("");
    float Compass_Error;
    Compass_Error = robot.compassHeadingDegrees - robot.compassTarget;
    robot.lcdDisplay.setCursor(0, 1);
    robot.lcdDisplay.print("Error:");
    robot.lcdDisplay.print(Compass_Error);
    Serial.print("Er:");
    Serial.print(Compass_Error);
    Serial.print(F("|"));
    if ( Compass_Error < 0) {
      SetPins_ToTurnRight();
      Serial.print("Spin Right");
      Serial.print(F("|"));
      delay(100);
    }
    if ( Compass_Error > 0) {
      SetPins_ToTurnLeft();
      Serial.print("Spin Left");
      Serial.print(F("|"));
      delay(100);
    }
    if (Compass_Error < 10)  robot.turnAdjust = 120;
    if (Compass_Error < 20)  robot.turnAdjust = 100;
    if (Compass_Error < 50)  robot.turnAdjust = 80;
    if (Compass_Error < 180) robot.turnAdjust = 20;

    Motor_Action_Turn_Speed();                                       // Sets the speed of the turning motion
    delay(100);
    Cancel = Cancel + 1;
    robot.lcdDisplay.setCursor(12, 1);
    robot.lcdDisplay.print(Cancel);
  }
  Get_Compass_Reading();
  delay(5);
  Get_Compass_Reading();




  // Once the while loop is satisfied (compass measures a degree between Lower and Upper, stop the mower
  Motor_Action_Stop_Motors();
  robot.turnAdjust = 0;
  SetPins_ToGoForwards();
  robot.lcdDisplay.print("Found");
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

  float Compass_Error = robot.compassHeadingDegrees - robot.headingLock;        // Calculates the error in compass heading from the saved lock heading

  if (Compass_Error > 180) Compass_Error = Compass_Error * -1 ;
  if (Compass_Error < -180) Compass_Error = Compass_Error * -1 ;
  Serial.print(F("C_Err:"));
  Serial.print(Compass_Error);
  Serial.print("|");

  if (Compass_Error < 0) {                                             // Steer left
    Serial.print("Steer_Right|");
    robot.pwmRight = PWM_MaxSpeed_RH + (CPower * Compass_Error);            // Multiply the difference by D to increase the power then subtract from the PWM
    if (robot.pwmRight < 0) robot.pwmRight = PWM_MaxSpeed_RH - 50;
    robot.pwmLeft = PWM_MaxSpeed_LH;                                        // Keep the Right wheel at full power calibrated to go straight
  }
  if (Compass_Error >= 0) {
    Serial.print("Steer_Left|");
    robot.pwmRight = PWM_MaxSpeed_RH;                                       // Keep the Left wheel at full power calibrated to go straight
    robot.pwmLeft = PWM_MaxSpeed_LH -  (CPower * Compass_Error);            // Multiply the difference by D to increase the power then subtract from the PWM
    if (robot.pwmLeft < 0) robot.pwmLeft = PWM_MaxSpeed_LH - 50;
  }

}
