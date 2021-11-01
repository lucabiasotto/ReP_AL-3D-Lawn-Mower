
#include "testSketches.h"
#include "robot.h"
#include "adcman.h"
#include "motorsController.h"
#include "movementsUtils.h"
#include "compassUtils.h"


/* Perimieter Wire Collision Motion
  ************************************************************************************/
  void Test_Mower_Check_Wire()  {
  
  ADCMan.run();
  // ADCMan.setCapture(pinPerimeterLeft, 1, 0);

  if (millis() >= robot.nextTime)  {
    robot.nextTime = millis() + 50;
    if (robot.perimeter.isInside(0) != robot.inside) {
      robot.inside = robot.perimeter.isInside(0);
      robot.counter++;
    }
  }

  /* Prints Values to the Serial Monitor of mag, smag and signal quality.  */
  Serial.print("Inside (1) or Outside (0):  ");
  Serial.print((robot.perimeter.isInside(0)));
  Serial.print("     MAG: ");
  Serial.print((int)robot.perimeter.getMagnitude(0));
  Serial.print("    smag: ");
  Serial.print((int)robot.perimeter.getSmoothMagnitude(0));
  Serial.print("     qaulity: ");
  Serial.println((robot.perimeter.getFilterQuality(0)));


  
  robot.lcdDisplay.setCursor(0,0);
  robot.lcdDisplay.print("IN/Out:");
  robot.lcdDisplay.setCursor(8,0);
  robot.lcdDisplay.print(robot.perimeter.isInside(0));
  robot.lcdDisplay.setCursor(0,1);
  robot.lcdDisplay.print("MAG:");
  robot.lcdDisplay.setCursor(8,1);
  robot.lcdDisplay.print(robot.perimeter.getMagnitude(0)); 


}


void Test_Relay() {
  
  digitalWrite(Relay_Motors, HIGH);
  Serial.println("Relay OFF");
  robot.lcdDisplay.print("Relay OFF");
  delay(1000);
  robot.lcdDisplay.clear();
  digitalWrite(Relay_Motors, LOW);
  Serial.println("Relay ON");
  robot.lcdDisplay.print("Relay ON");
  delay(1000);
  robot.lcdDisplay.clear();
}



void Test_Wheel_Motors() {

  digitalWrite(Relay_Motors, LOW);
  delay(200);
  if (robot.I == 1) {

  robot.lcdDisplay.clear();
  robot.lcdDisplay.setCursor(0,0);
  robot.lcdDisplay.print("Drive Wheel");
  robot.lcdDisplay.setCursor(1,0);
  robot.lcdDisplay.print("Test");
  delay(1000);
  robot.lcdDisplay.clear();

  robot.lcdDisplay.print("Remove ALL");
  robot.lcdDisplay.setCursor(0,1);
  robot.lcdDisplay.print("Blades!!!");
  delay(1000);
  robot.lcdDisplay.clear();


  robot.lcdDisplay.print("<-- Turn Left");
  delay(500);
  SetPins_ToTurnLeft();
  Motor_Action_Go_Full_Speed();
  delay(1000);
  Motor_Action_Stop_Motors();
  delay(1000);
  robot.lcdDisplay.clear();

  robot.lcdDisplay.print("Turn Right -->");
  delay(500);
  SetPins_ToTurnRight();
  Motor_Action_Go_Full_Speed();
  delay(2000);
  Motor_Action_Stop_Motors();
  delay(1000);
  robot.lcdDisplay.clear();

  robot.lcdDisplay.print("Forwards");
  delay(500);
  SetPins_ToGoForwards();
  Motor_Action_Go_Full_Speed();
  delay(1000);
  Motor_Action_Stop_Motors(); 
  delay(1000);
  robot.lcdDisplay.clear();

  robot.lcdDisplay.print("Backwards");
  delay(500);
  SetPins_ToGoBackwards();   
  Motor_Action_Go_Full_Speed();
  delay(1000);
  Motor_Action_Stop_Motors();  
  delay(1000);
  robot.lcdDisplay.clear();

  robot.lcdDisplay.clear();
  robot.lcdDisplay.setCursor(0,0);
  robot.lcdDisplay.print("Dynamic");
  robot.lcdDisplay.setCursor(0,1);
  robot.pwmLeft = 120;
  robot.pwmRight = 120;
  robot.lcdDisplay.print("L:");
  robot.lcdDisplay.print(robot.pwmLeft);
  robot.lcdDisplay.print("  R:");
  robot.lcdDisplay.print(robot.pwmRight);
  SetPins_ToGoForwards();
  Motor_Action_Dynamic_PWM_Steering();
  delay(2000);

  
  robot.lcdDisplay.clear();
  robot.lcdDisplay.setCursor(0,1);  
  robot.pwmLeft = 255;
  robot.pwmRight = 0;
  robot.lcdDisplay.print("L:");
  robot.lcdDisplay.print(robot.pwmLeft);
  robot.lcdDisplay.print("  R:");
  robot.lcdDisplay.print(robot.pwmRight);
  SetPins_ToGoForwards();
  Motor_Action_Dynamic_PWM_Steering();

  robot.lcdDisplay.clear();
  robot.lcdDisplay.setCursor(0,1);
  robot.pwmLeft = 255;
  robot.pwmRight = 0;
  robot.lcdDisplay.print("L:");
  robot.lcdDisplay.print(robot.pwmLeft);
  robot.lcdDisplay.print("  R:");
  robot.lcdDisplay.print(robot.pwmRight);
  SetPins_ToGoForwards();
  Motor_Action_Dynamic_PWM_Steering();
  delay(2000);


  robot.lcdDisplay.clear();
  robot.lcdDisplay.setCursor(0,1);  
  robot.pwmLeft = 150;
  robot.pwmRight = 0;
  robot.lcdDisplay.print("L:");
  robot.lcdDisplay.print(robot.pwmLeft);
  robot.lcdDisplay.print("  R:");
  robot.lcdDisplay.print(robot.pwmRight);
  SetPins_ToGoForwards();
  Motor_Action_Dynamic_PWM_Steering();
  delay(2000);
  
  robot.lcdDisplay.clear();
  robot.lcdDisplay.setCursor(0,1);  
  robot.pwmLeft = 255;
  robot.pwmRight = 0;
  robot.lcdDisplay.print("L:");
  robot.lcdDisplay.print(robot.pwmLeft);
  robot.lcdDisplay.print("  R:");
  robot.lcdDisplay.print(robot.pwmRight);
  SetPins_ToGoForwards();
  Motor_Action_Dynamic_PWM_Steering();
  delay(2000);

    robot.lcdDisplay.clear();
  robot.lcdDisplay.setCursor(0,1);  
  robot.pwmLeft = 255;
  robot.pwmRight = 0;
  robot.lcdDisplay.print("L:");
  robot.lcdDisplay.print(robot.pwmLeft);
  robot.lcdDisplay.print("  R:");
  robot.lcdDisplay.print(robot.pwmRight);
  SetPins_ToGoForwards();
  Motor_Action_Dynamic_PWM_Steering();

  robot.lcdDisplay.clear();
  robot.lcdDisplay.setCursor(0,1);
  robot.pwmLeft = 0;
  robot.pwmRight = 255;
  robot.lcdDisplay.print("L:");
  robot.lcdDisplay.print(robot.pwmLeft);
  robot.lcdDisplay.print("  R:");
  robot.lcdDisplay.print(robot.pwmRight);
  SetPins_ToGoForwards();
  Motor_Action_Dynamic_PWM_Steering();
  delay(2000);


  robot.lcdDisplay.clear();
  robot.lcdDisplay.setCursor(0,1);  
  robot.pwmLeft = 0;
  robot.pwmRight = 150;
  robot.lcdDisplay.print("L:");
  robot.lcdDisplay.print(robot.pwmLeft);
  robot.lcdDisplay.print("  R:");
  robot.lcdDisplay.print(robot.pwmRight);
  SetPins_ToGoForwards();
  Motor_Action_Dynamic_PWM_Steering();
  delay(2000);
  
  robot.lcdDisplay.clear();
  robot.lcdDisplay.setCursor(0,1);  
  robot.pwmLeft = 0;
  robot.pwmRight = 255;
  robot.lcdDisplay.print("L:");
  robot.lcdDisplay.print(robot.pwmLeft);
  robot.lcdDisplay.print("  R:");
  robot.lcdDisplay.print(robot.pwmRight);
  SetPins_ToGoForwards();
  Motor_Action_Dynamic_PWM_Steering();
  delay(2000);

  
  Motor_Action_Stop_Motors();  
  delay(1000);

  robot.lcdDisplay.clear();
  robot.lcdDisplay.print("Test Finished");
  delay(1000);
  robot.lcdDisplay.clear();

  robot.I = 2;
  }
  digitalWrite(Relay_Motors, HIGH);
  delay(200);
}     



void Test_Mower_Blade_Motor() {
  // Spin the blade motor for 7 seconds
  digitalWrite(Relay_Motors, LOW);
  delay(200);
  robot.lcdDisplay.print("Blade Motor");
  robot.lcdDisplay.setCursor(0,1);
  robot.lcdDisplay.print("Test..!!");
  delay(1000);
  robot.lcdDisplay.clear();
  robot.lcdDisplay.print("Remove ALL");
  robot.lcdDisplay.setCursor(0,1);
  robot.lcdDisplay.print("Blades!!!");
  delay(4000);
  robot.lcdDisplay.clear();
  delay(2000);
  robot.lcdDisplay.print("BLADE MOTOR");
  delay(500);
  Serial.println("Blades ON");
  robot.lcdDisplay.setCursor(0,1);
  robot.lcdDisplay.print("ON ");
  robot.lcdDisplay.setCursor(6,1);
  robot.lcdDisplay.print("PWM =MAX");
  Motor_Action_Spin_Blades();
  delay(7000);


  // Stop the blade motor spinning for 2 seconds
  robot.lcdDisplay.clear();
  Serial.println("Blades OFF");
  robot.lcdDisplay.print("BLADE MOTOR");
  robot.lcdDisplay.setCursor(0,1);
  robot.lcdDisplay.print("OFF..  ");
  Motor_Action_Stop_Spin_Blades();
  delay(2000);
  robot.lcdDisplay.clear();
  delay(500);

  digitalWrite(Relay_Motors, HIGH);
  delay(200);

  }


void Test_Sonar_Array()   {

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
    if (SONAR_1_ACTIVATE) robot.distance1 = PingSonarY(trigPin1, echoPin1, 1, 1, 1, 5, 0);          //SONAR1
    delay(15);
    if (SONAR_2_ACTIVATE) robot.distance2 = PingSonarY(trigPin2, echoPin2, 2, 2, 2, 0, 0);         //SONAR2
    delay(15);
    if (SONAR_3_ACTIVATE) robot.distance3 = PingSonarY(trigPin3, echoPin3, 3, 3, 3, 10, 0);          //SONAR3
    delay(15);
  }





/* SONAR Function
************************************************************************************/
// Function to Ping the Sonar calculate the distance from Object to the Sonars.
// Distance calculated is printed to serial printer and displays X or _ on the LCD Screen
// Distance calculated is then used for the object avoidance logic
// Sonars used can be activated in the settings.

int PingSonarY(int trigPinY, int echoPinY, int distanceY, long durationY, int sonarY, int LCDRow, int LCDColumn) {
  pinMode(trigPinY, OUTPUT);
  pinMode(echoPinY, INPUT);
  //Sets the trigPin at High state for 10 micro secs sending a sound wave
  digitalWrite(trigPinY, HIGH);
  digitalWrite(trigPinY, LOW);
  delayMicroseconds(10);

  /*Reads the echoPin for the bounced wave and records the time in microseconds*/
  durationY = pulseIn(echoPinY, HIGH);

  /*Calculates the distance in cm based on the measured time*/
  distanceY = durationY * 0.034 / 2;
  delay(5);

  /* If a 0 distance is measured normally the Sonar ping has not been received.
    distance is then set to 999cm so the missed ping is not seen as an object detected.*/
  if (distanceY == 0) {
    distanceY = 999;
    Serial.print("SONAR ");
    Serial.print(sonarY);
    Serial.print(": ");
    Serial.println("NO PING ERROR REMOVED");
  }

  /*Prints the Sonar letter and distance measured on the serial Monitor*/
  Serial.print("SONAR ");
  Serial.print(sonarY);
  Serial.print(": ");
  Serial.print(distanceY);
  Serial.println(" cm");
  //Serial.println(SONAR_MAX_DISTANCE);

  /*If sonar distance is less than maximum distance then an object is registered to avoid*/
  if (distanceY <= SONAR_MAX_DISTANCE ) {
    //Prints that Sonar X has detected an object to the Mower LCD.
    robot.lcdDisplay.setCursor(LCDRow, LCDColumn);                //sets location for text to be written
    robot.lcdDisplay.print("X");
    LCDColumn = LCDColumn + 1;    
    robot.lcdDisplay.setCursor(LCDRow, LCDColumn);                //sets location for text to be written
    robot.lcdDisplay.print("   ");
    robot.lcdDisplay.setCursor(LCDRow, LCDColumn);
    robot.lcdDisplay.print(distanceY);
    delay(10);
  }

  /*If sonar distance is greater than maximum distance then no object is registered to avoid*/
  if (distanceY > 100) {
    //Prints that the path of Sonar X is open.
    LCDColumn = LCDColumn - 1;   
    robot.lcdDisplay.setCursor(LCDRow, LCDColumn);                 //sets location for text to be written
    robot.lcdDisplay.print("_");
    delay(10);
  }

  return distanceY;
  return sonarY;

}


void Test_Compass_Turn_Function() {
    digitalWrite(Relay_Motors, LOW);
    delay(200);
    SetPins_ToGoForwards();
    Motor_Action_Go_Full_Speed();
    delay(2000);
    Manouver_Turn_Around();
    Turn_To_Compass_Heading();
    SetPins_ToGoForwards();
    Motor_Action_Go_Full_Speed();
    delay(2000); 
    digitalWrite(Relay_Motors, HIGH);
    }
  
