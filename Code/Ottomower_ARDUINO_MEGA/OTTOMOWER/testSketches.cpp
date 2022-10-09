
#include "testSketches.h"

#include "adcman.h"
#include "compassUtils.h"
#include "motorsController.h"
#include "movementsUtils.h"
#include "robot.h"
#include "sonar.h"
#include "wireDetection.h"

/* Perimieter Wire Collision Motion
  ************************************************************************************/
void Test_Mower_Check_Wire() {
    ADCMan.run();
    // ADCMan.setCapture(pinPerimeterLeft, 1, 0);

    readWireSensor();  // Read the wire sensor and see of the mower is now  or outside the wire

    /* Prints Values to the Serial Monitor of mag, smag and signal quality.  */
    Serial.print("Inside (1) or Outside (0):  ");
    Serial.print((robot.perimeter.isInside(0)));
    Serial.print("     MAG: ");
    Serial.print((int)robot.perimeter.getMagnitude(0));
    Serial.print("    smag: ");
    Serial.print((int)robot.perimeter.getSmoothMagnitude(0));
    Serial.print("     qaulity: ");
    Serial.println((robot.perimeter.getFilterQuality(0)));

    robot.lcdDisplay.setCursor(0, 0);
    robot.lcdDisplay.print("IN/Out:");
    robot.lcdDisplay.setCursor(8, 0);
    robot.lcdDisplay.print(robot.perimeter.isInside(0));
    robot.lcdDisplay.setCursor(0, 1);
    robot.lcdDisplay.print("MAG:");
    robot.lcdDisplay.setCursor(8, 1);
    robot.lcdDisplay.print(robot.perimeter.getMagnitude(0));
}

void Test_Relay() {
    digitalWrite(RELAY_MOTORS_PIN, HIGH);
    Serial.println("Relay OFF");
    robot.lcdDisplay.print("Relay OFF");
    delay(1000);
    robot.lcdDisplay.clear();
    digitalWrite(RELAY_MOTORS_PIN, LOW);
    Serial.println("Relay ON");
    robot.lcdDisplay.print("Relay ON");
    delay(1000);
    robot.lcdDisplay.clear();
}

void Test_Wheel_Motors() {
    digitalWrite(RELAY_MOTORS_PIN, LOW);
    delay(200);

    robot.lcdDisplay.clear();
    robot.lcdDisplay.setCursor(0, 0);
    robot.lcdDisplay.print("Drive Wheel");
    robot.lcdDisplay.setCursor(1, 0);
    robot.lcdDisplay.print("Test");
    delay(1000);
    robot.lcdDisplay.clear();

    robot.lcdDisplay.print("Remove ALL");
    robot.lcdDisplay.setCursor(0, 1);
    robot.lcdDisplay.print("Blades!!!");
    delay(1000);
    robot.lcdDisplay.clear();

    robot.lcdDisplay.print("<-- Turn Left");
    delay(500);
    motorsSetPinsToTurnLeft();
    motorsSetFullSpeed();
    delay(1000);
    motorsStopWheelMotors();
    delay(1000);
    robot.lcdDisplay.clear();

    robot.lcdDisplay.print("Turn Right -->");
    delay(500);
    motorsSetPinsToTurnRight();
    motorsSetFullSpeed();
    delay(2000);
    motorsStopWheelMotors();
    delay(1000);
    robot.lcdDisplay.clear();

    robot.lcdDisplay.print("Forwards");
    delay(500);
    motorsSetPinsToGoForwards();
    motorsSetFullSpeed();
    delay(1000);
    motorsStopWheelMotors();
    delay(1000);
    robot.lcdDisplay.clear();

    robot.lcdDisplay.print("Backwards");
    delay(500);
    motorsSetPinsToGoBackwards();
    motorsSetFullSpeed();
    delay(1000);
    motorsStopWheelMotors();
    delay(1000);
    robot.lcdDisplay.clear();

    robot.lcdDisplay.clear();
    robot.lcdDisplay.setCursor(0, 0);
    robot.lcdDisplay.print("Dynamic");
    robot.lcdDisplay.setCursor(0, 1);
    robot.pwmLeft = 120;
    robot.pwmRight = 120;
    robot.lcdDisplay.print("L:");
    robot.lcdDisplay.print(robot.pwmLeft);
    robot.lcdDisplay.print("  R:");
    robot.lcdDisplay.print(robot.pwmRight);
    motorsSetPinsToGoForwards();
    motorsSetDynamicSteeringSpeed();
    delay(2000);

    robot.lcdDisplay.clear();
    robot.lcdDisplay.setCursor(0, 1);
    robot.pwmLeft = 255;
    robot.pwmRight = 0;
    robot.lcdDisplay.print("L:");
    robot.lcdDisplay.print(robot.pwmLeft);
    robot.lcdDisplay.print("  R:");
    robot.lcdDisplay.print(robot.pwmRight);
    motorsSetPinsToGoForwards();
    motorsSetDynamicSteeringSpeed();

    robot.lcdDisplay.clear();
    robot.lcdDisplay.setCursor(0, 1);
    robot.pwmLeft = 255;
    robot.pwmRight = 0;
    robot.lcdDisplay.print("L:");
    robot.lcdDisplay.print(robot.pwmLeft);
    robot.lcdDisplay.print("  R:");
    robot.lcdDisplay.print(robot.pwmRight);
    motorsSetPinsToGoForwards();
    motorsSetDynamicSteeringSpeed();
    delay(2000);

    robot.lcdDisplay.clear();
    robot.lcdDisplay.setCursor(0, 1);
    robot.pwmLeft = 150;
    robot.pwmRight = 0;
    robot.lcdDisplay.print("L:");
    robot.lcdDisplay.print(robot.pwmLeft);
    robot.lcdDisplay.print("  R:");
    robot.lcdDisplay.print(robot.pwmRight);
    motorsSetPinsToGoForwards();
    motorsSetDynamicSteeringSpeed();
    delay(2000);

    robot.lcdDisplay.clear();
    robot.lcdDisplay.setCursor(0, 1);
    robot.pwmLeft = 255;
    robot.pwmRight = 0;
    robot.lcdDisplay.print("L:");
    robot.lcdDisplay.print(robot.pwmLeft);
    robot.lcdDisplay.print("  R:");
    robot.lcdDisplay.print(robot.pwmRight);
    motorsSetPinsToGoForwards();
    motorsSetDynamicSteeringSpeed();
    delay(2000);

    robot.lcdDisplay.clear();
    robot.lcdDisplay.setCursor(0, 1);
    robot.pwmLeft = 255;
    robot.pwmRight = 0;
    robot.lcdDisplay.print("L:");
    robot.lcdDisplay.print(robot.pwmLeft);
    robot.lcdDisplay.print("  R:");
    robot.lcdDisplay.print(robot.pwmRight);
    motorsSetPinsToGoForwards();
    motorsSetDynamicSteeringSpeed();

    robot.lcdDisplay.clear();
    robot.lcdDisplay.setCursor(0, 1);
    robot.pwmLeft = 0;
    robot.pwmRight = 255;
    robot.lcdDisplay.print("L:");
    robot.lcdDisplay.print(robot.pwmLeft);
    robot.lcdDisplay.print("  R:");
    robot.lcdDisplay.print(robot.pwmRight);
    motorsSetPinsToGoForwards();
    motorsSetDynamicSteeringSpeed();
    delay(2000);

    robot.lcdDisplay.clear();
    robot.lcdDisplay.setCursor(0, 1);
    robot.pwmLeft = 0;
    robot.pwmRight = 150;
    robot.lcdDisplay.print("L:");
    robot.lcdDisplay.print(robot.pwmLeft);
    robot.lcdDisplay.print("  R:");
    robot.lcdDisplay.print(robot.pwmRight);
    motorsSetPinsToGoForwards();
    motorsSetDynamicSteeringSpeed();
    delay(2000);

    robot.lcdDisplay.clear();
    robot.lcdDisplay.setCursor(0, 1);
    robot.pwmLeft = 0;
    robot.pwmRight = 255;
    robot.lcdDisplay.print("L:");
    robot.lcdDisplay.print(robot.pwmLeft);
    robot.lcdDisplay.print("  R:");
    robot.lcdDisplay.print(robot.pwmRight);
    motorsSetPinsToGoForwards();
    motorsSetDynamicSteeringSpeed();
    delay(2000);

    motorsStopWheelMotors();
    delay(1000);

    robot.lcdDisplay.clear();
    robot.lcdDisplay.print("Test Finished");
    delay(1000);
    robot.lcdDisplay.clear();

    digitalWrite(RELAY_MOTORS_PIN, HIGH);
    delay(200);
}

void Test_Mower_Blade_Motor() {
    // Spin the blade motor for 7 seconds
    digitalWrite(RELAY_MOTORS_PIN, LOW);
    delay(200);
    robot.lcdDisplay.print("Blade Motor");
    robot.lcdDisplay.setCursor(0, 1);
    robot.lcdDisplay.print("Test..!!");
    delay(1000);
    robot.lcdDisplay.clear();
    robot.lcdDisplay.print("Remove ALL");
    robot.lcdDisplay.setCursor(0, 1);
    robot.lcdDisplay.print("Blades!!!");
    delay(4000);
    robot.lcdDisplay.clear();
    delay(2000);
    robot.lcdDisplay.print("BLADE MOTOR");
    delay(500);
    Serial.println("Blades ON");
    robot.lcdDisplay.setCursor(0, 1);
    robot.lcdDisplay.print("ON ");
    robot.lcdDisplay.setCursor(6, 1);
    robot.lcdDisplay.print("PWM =MAX");
    motorsActivateSpinBlades();
    delay(7000);

    // Stop the blade motor spinning for 2 seconds
    robot.lcdDisplay.clear();
    Serial.println("Blades OFF");
    robot.lcdDisplay.print("BLADE MOTOR");
    robot.lcdDisplay.setCursor(0, 1);
    robot.lcdDisplay.print("OFF..  ");
    motorsStopSpinBlades();
    delay(2000);
    robot.lcdDisplay.clear();
    delay(500);

    digitalWrite(RELAY_MOTORS_PIN, HIGH);
    delay(200);
}

void Test_Sonar_Array() {
    //Clears the Trig Pin
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin3, LOW);

    /*Fires all Sonars to detect objects ahead...
   * Sonars are not fired in order to avoid reflections of sonar in the next sensor.
     distance# reurned (trigpin#, echopin#, distance#, duration#, Sonar#, LCDColumn#, LCD Row#)
   *********************************************************************************************/
    if (SONAR_1_ACTIVATE) robot.distance1 = getSonarDistance(trigPin1, echoPin1, 1);  //SONAR1
    delay(15);
    if (SONAR_2_ACTIVATE) robot.distance2 = getSonarDistance(trigPin2, echoPin2, 2);  //SONAR2
    delay(15);
    if (SONAR_3_ACTIVATE) robot.distance3 = getSonarDistance(trigPin3, echoPin3, 3);  //SONAR3
    delay(15);
}

void testCompassTurnFunction() {
    digitalWrite(RELAY_MOTORS_PIN, LOW);
    delay(200);
    motorsSetPinsToGoForwards();
    motorsSetFullSpeed();
    delay(2000);
    robotReverseDirection();
    turnToCompassTarget(180);
    motorsSetPinsToGoForwards();
    motorsSetFullSpeed();
    delay(2000);
    digitalWrite(RELAY_MOTORS_PIN, HIGH);
    delay(2000);
    motorsStopWheelMotors();
    robot.lcdDisplay.clear();
    robot.lcdDisplay.print("Test Finished");
    delay(1000);
    robot.lcdDisplay.clear();
}
