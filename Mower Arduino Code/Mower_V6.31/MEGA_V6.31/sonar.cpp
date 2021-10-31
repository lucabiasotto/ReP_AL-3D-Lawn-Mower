#include "settings.h"
#include "sonar.h"
#include "lcdDisplay.h"
#include "robot.h"

void readSonarSensor() {
    // Ping Sonar sensors

    //Clears the Trig Pin
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin3, LOW);

    // Pings each sonar at a 15ms interval

    if (Sonar_2_Activate == 1) robot.distance2 = PingSonarX(trigPin2, echoPin2, 2, 2, 2, 0, 0);  //SONAR2
    if (Sonar_1_Activate == 1) robot.distance1 = PingSonarX(trigPin1, echoPin1, 1, 1, 1, 1, 0);  //SONAR1
    if (Sonar_3_Activate == 1) robot.distance3 = PingSonarX(trigPin3, echoPin3, 3, 3, 3, 2, 0);  //SONAR3
}

/* SONAR Function
************************************************************************************/
// Function to Ping the Sonar calculate the distance from Object to the Sonars.
// Distance calculated is printed to serial printer and displays X or _ on the LCD Screen
// Distance calculated is then used for the object avoidance logic
// Sonars used can be activated in the settings.

int PingSonarX(int trigPinX, int echoPinX, int distanceX, long durationX, int sonarX, int LCDRow, int LCDColumn) {
    pinMode(trigPinX, OUTPUT);
    pinMode(echoPinX, INPUT);
    //Sets the trigPin at High state for 10 micro secs sending a sound wave
    digitalWrite(trigPinX, HIGH);
    digitalWrite(trigPinX, LOW);
    delayMicroseconds(10);

    /*Reads the echoPin for the bounced wave and records the time in microseconds*/
    durationX = pulseIn(echoPinX, HIGH);

    /*Calculates the distance in cm based on the measured time*/
    distanceX = durationX * 0.034 / 2;
    delay(5);

    /* If a 0 distance is measured normally the Sonar ping has not been received.
    distance is then set to 999cm so the missed ping is not seen as an object detected.*/
    if (distanceX == 0) {
        distanceX = 999;
        Serial.print(F("S"));
        Serial.print(sonarX);
        Serial.print(F(":"));
        Serial.print(F("NP!"));
        Serial.print(F("|"));
    }

    /*Prints the Sonar letter and distance measured on the serial Monitor*/
    Serial.print(F("S"));
    Serial.print(sonarX);
    Serial.print(F(":"));
    Serial.print(distanceX);
    Serial.print(F("cm"));
    Serial.print(F("/"));

    /*If sonar distance is less than maximum distance then an object is registered to avoid*/
    if (distanceX <= maxdistancesonar) {
        //Prints that Sonar X has detected an object to the Mower LCD.
        robot.lcdDisplay.setCursor(LCDRow, LCDColumn);  //sets location for text to be written
        robot.lcdDisplay.print("X");
        delay(10);
        if (sonarX == 1) {
            robot.SonarHit1Total = (robot.SonarHit1Total + 1);
            Serial.print(robot.SonarHit1Total);
        }
        if (sonarX == 2) {
            robot.SonarHit2Total = (robot.SonarHit2Total + 1);
            Serial.print(robot.SonarHit2Total);
        }
        if (sonarX == 3) {
            robot.SonarHit3Total = (robot.SonarHit3Total + 1);
            Serial.print(robot.SonarHit3Total);
        }
        if ((robot.SonarHit1Total == Max_Sonar_Hit) || (robot.SonarHit2Total == Max_Sonar_Hit) || (robot.SonarHit3Total == Max_Sonar_Hit)) {
            robot.sonarHit = 1;
            Print_Sonar_Hit();
            Serial.println("");
            Serial.println("Sonar Hit Detected");
        }
    }

    /*If sonar distance is greater than maximum distance then no object is registered to avoid*/
    if (distanceX > maxdistancesonar) {
        //Prints that the path of Sonar X is open.
        robot.lcdDisplay.setCursor(LCDRow, LCDColumn);  //sets location for text to be written
        robot.lcdDisplay.print("_");
        delay(10);
        if (sonarX == 1) {
            robot.SonarHit1Total = 0;
            Serial.print(robot.SonarHit1Total);
        }
        if (sonarX == 2) {
            robot.SonarHit2Total = 0;
            Serial.print(robot.SonarHit2Total);
        }
        if (sonarX == 3) {
            robot.SonarHit3Total = 0;
            Serial.print(robot.SonarHit3Total);
        }
    }

    return distanceX;
    return sonarX;
}
