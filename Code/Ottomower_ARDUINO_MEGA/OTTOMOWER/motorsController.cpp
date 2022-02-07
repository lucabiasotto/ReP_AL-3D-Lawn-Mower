#include "motorsController.h"
#include "robot.h"

/**
 * Set wheel motors to full speed
 */
void motorsSetFullSpeed() {
    analogWrite(ENAPin, PWM_MAX_SPEED_RH);  // Speed = 0-255  (255 is max speed). Speed is set in the settings
    analogWrite(ENBPin, PWM_MAX_SPEED_LH);  // Anaolgwirte sends PWM signals Speed = 0-255  (255 is max speed)
    Serial.print(F("Wheel:FULL|"));
}

/**
 * Ued to turn the mower at a set speed.
 * @param turnSpeedCompensation, 0 full spedd >0 reduce speed
 */
void motorsReduceTurnSpeed(int turnSpeedCompensation) {
    analogWrite(ENAPin, PWM_MAX_SPEED_RH - turnSpeedCompensation);  // Change the 0 value to 10 or 20 to recuce the speed
    analogWrite(ENBPin, PWM_MAX_SPEED_LH - turnSpeedCompensation);  // Change the 0 value to 10 or 20 to recuce the speed
}

/**
 * Set Motor Bridge pins are set for both motors to stop
 */
void motorsStopWheelMotors() { 
    digitalWrite(ENAPin, 0);
    digitalWrite(IN1Pin, LOW);  //Motor 1
    digitalWrite(IN2Pin, LOW);

    digitalWrite(ENBPin, 0);  //Motor 2
    digitalWrite(IN3Pin, LOW);
    digitalWrite(IN4Pin, LOW);

    Serial.print(F("Wheel:0FF|"));
}

/**
 * Set wheel motors forward direction
 */
void motorsSetPinsToGoForwards() {   // Motor Bridge pins are set for both motors to move forwards.
    digitalWrite(IN1Pin, LOW);  // Motor Birdge pins are set to high or low to set the direction of movement
    digitalWrite(IN2Pin, HIGH);
    digitalWrite(IN3Pin, LOW);
    digitalWrite(IN4Pin, HIGH);
    Serial.print(F("Wheel:For|"));
}

/**
 * Set wheel motors backwards direction
 */
void motorsSetPinsToGoBackwards() {   // Motor Bridge pins are set for both motors to move Backwards
    digitalWrite(IN1Pin, HIGH);  // Motor 1
    digitalWrite(IN2Pin, LOW);
    digitalWrite(IN3Pin, HIGH);  // Motor 2
    digitalWrite(IN4Pin, LOW);
    Serial.print(F("Wheel:Rev|"));
    delay(20);
}


/**
 * Pins are set so that Motors drive in opposite directions to go left
 */
void motorsSetPinsToTurnLeft() {     
    digitalWrite(IN1Pin, LOW);  // Motor 1
    digitalWrite(IN2Pin, HIGH);
    digitalWrite(IN3Pin, HIGH);  // Motor 2
    digitalWrite(IN4Pin, LOW);
    Serial.print(F("Wheel:TL_|"));
}

/**
 * Pins are set so that Motors drive in opposite directions to go right
 */
void motorsSetPinsToTurnRight() {     // Pins are set so that Motors drive in opposite directions
    digitalWrite(IN1Pin, HIGH);  // Motor 1
    digitalWrite(IN2Pin, LOW);
    digitalWrite(IN3Pin, LOW);  //Motor 2
    digitalWrite(IN4Pin, HIGH);
    Serial.print(F("Wheel:R|"));
}

/**
 *  Turns the mowing blades on
 */
void motorsActivateSpinBlades() {
    if (CUTTING_BLADES_ACTIVATE == 1) {
        // Blades are turn ON in settings and will spin!
        delay(20);
        digitalWrite(R_EN, HIGH);
        digitalWrite(L_EN, HIGH);
        delay(20);
        analogWrite(RPWM, 255);  //TODO SERVE ANCORA?
        delay(20);
        Serial.print("Blades:ON_|");
    } else {
      //Blade not enable
    }
}

/**
 * Disactivate spin blades
 */
void motorsStopSpinBlades() {
    delay(20);
    digitalWrite(R_EN, LOW);
    digitalWrite(L_EN, LOW);
    delay(20);
    Serial.print("Blades:0FF|");
}

/**
 * Steers the Mower depending on the PID input from the Algorythm
 * use robot.pwmRight and robot.pwmLeft
 */
void motorsSetDynamicSteeringSpeed() {
    analogWrite(ENAPin, robot.pwmRight);  // ENA low = Right Swerve   ENB low = Left Swerve
    analogWrite(ENBPin, robot.pwmLeft);
    Serial.print("PWM_R:");
    Serial.print(robot.pwmRight);
    Serial.print("|");
    Serial.print("PWM_L:");
    Serial.print(robot.pwmLeft);
    Serial.print("|");
}
