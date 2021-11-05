#include "sonar.h"

#include "lcdDisplay.h"
#include "robot.h"
#include "settings.h"

void readSonarSensors() {
    // Ping Sonar sensors

    //Clears the Trig Pin
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin3, LOW);

    // Pings each sonar at a 15ms interval

    if (SONAR_2_ACTIVATE == 1) {
        //LEFT sonar
        robot.distance2 = getSonarDistance(trigPin2, echoPin2, 2);
    }
    if (SONAR_1_ACTIVATE == 1) {
        //CENTER sonar
        robot.distance1 = getSonarDistance(trigPin1, echoPin1, 1);
    }
    if (SONAR_3_ACTIVATE == 1) {
        //RIGHT sonar
        robot.distance3 = getSonarDistance(trigPin3, echoPin3, 3);
    }
}

/**
 * Function to Ping the Sonar calculate the distance from Object to the Sonars.
 * Distance calculated is printed to serial printer and displays X or _ on the LCD Screen
 * Sonars used can be activated in the settings.
 */
int getSonarDistance(int trigPinX, int echoPinX, int sonarNumber) {
    int distance = 0;
    long duration = 0;

    pinMode(trigPinX, OUTPUT);
    pinMode(echoPinX, INPUT);
    //Sets the trigPin at High state for 10 micro secs sending a sound wave
    digitalWrite(trigPinX, HIGH);
    digitalWrite(trigPinX, LOW);
    delayMicroseconds(10);

    /*Reads the echoPin for the bounced wave and records the time in microseconds*/
    duration = pulseIn(echoPinX, HIGH);

    /*Calculates the distance in cm based on the measured time*/
    distance = duration * 0.034 / 2;
    delay(5);

    //If a 0 distance is measured normally the Sonar ping has not been received.
    //distance is then set to 999cm so the missed ping is not seen as an object detected.
    if (distance == 0) {
        distance = 999;
        Serial.print(F("Sonar"));
        Serial.print(sonarNumber);
        Serial.print(F(":FAIL"));
    } else {
        //Prints the Sonar letter and distance measured on the serial Monitor
        Serial.print(F("Sonar"));
        Serial.print(sonarNumber);
        Serial.print(F(":"));
        Serial.print(distance);
        Serial.print(F("cm|TotalHit:"));
    }

    //If sonar distance is less than maximum distance then an object is registered to avoid
    if (distance <= SONAR_MAX_DISTANCE) {
        

        if (sonarNumber == 1) {
            robot.lcdDisplay.setCursor(1, 0);
            robot.SonarHit1Total = (robot.SonarHit1Total + 1);
            Serial.print(robot.SonarHit1Total);
        }
        if (sonarNumber == 2) {
            robot.lcdDisplay.setCursor(0, 0);
            robot.SonarHit2Total = (robot.SonarHit2Total + 1);
            Serial.print(robot.SonarHit2Total);
        }
        if (sonarNumber == 3) {
            robot.lcdDisplay.setCursor(2, 0);
            robot.SonarHit3Total = (robot.SonarHit3Total + 1);
            Serial.print(robot.SonarHit3Total);
        }
        robot.lcdDisplay.print("X");//Prints that Sonar X has detected an object to the Mower LCD.
        delay(10);  //TODO serve?

        if (robot.SonarHit1Total >= SONAR_MAX_HIT || robot.SonarHit2Total >= SONAR_MAX_HIT || robot.SonarHit3Total >= SONAR_MAX_HIT) {
            Serial.println("|");
            Serial.println("Sonar Hit Detected");
        }
    } else {
        // If sonar distance is greater than maximum distance then no object is registered to avoid

        

        if (sonarNumber == 1) {
            robot.lcdDisplay.setCursor(1, 0);
            robot.SonarHit1Total = 0;
            Serial.print(robot.SonarHit1Total);
        }
        if (sonarNumber == 2) {
            robot.lcdDisplay.setCursor(0, 0);
            robot.SonarHit2Total = 0;
            Serial.print(robot.SonarHit2Total);
        }
        if (sonarNumber == 3) {
            robot.SonarHit3Total = 0;
            robot.lcdDisplay.setCursor(2, 0);
            Serial.print(robot.SonarHit3Total);
        }

        robot.lcdDisplay.print("_"); ////Prints that the path of Sonar _ is open.
        delay(10); //TODO serve?
    }

    return distance;
}
