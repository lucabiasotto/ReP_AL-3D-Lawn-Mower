#include "motorsController.h"
#include "robot.h"

void Motor_Action_Go_Full_Speed()     {
      analogWrite(ENAPin, PWM_MAX_SPEED_RH);                       // Speed = 0-255  (255 is max speed). Speed is set in the settings                        
      analogWrite(ENBPin, PWM_MAX_SPEED_LH);                       // Anaolgwirte sends PWM signals Speed = 0-255  (255 is max speed)
      Console.print(F("Wheel:FULL|"));
      }

void Motor_Action_GoFullSpeed_Out_Garage()     {
  //Speeds can be changed to give the mower a slight curve when exiting the Garage.
  
  //TODO WTF? 
  /*
  PWM_MaxSpeed_LH = PWM_MaxSpeed_LH + 20;                          
  if (PWM_MaxSpeed_LH > 255)  PWM_MaxSpeed_LH = 255;
  if (PWM_MaxSpeed_RH > 255)  PWM_MaxSpeed_RH = 255;
  */
 
  analogWrite(ENAPin, PWM_MAX_SPEED_RH);                                       // Speed = 0-255  (255 is max speed). Speed is set in the settings
  analogWrite(ENBPin, PWM_MAX_SPEED_LH);   
  Console.print(F("Wheel:FULL|"));
  }


void SetPins_ToGoForwards()     {                                 // Motor Bridge pins are set for both motors to move forwards.
  digitalWrite(IN1Pin, LOW);                                      // Motor Birdge pins are set to high or low to set the direction of movement
  digitalWrite(IN2Pin, HIGH);
  digitalWrite(IN3Pin, LOW);
  digitalWrite(IN4Pin, HIGH);
  Console.print(F("Wheel:For|"));
  }


void SetPins_ToGoBackwards()      {                               // Motor Bridge pins are set for both motors to move Backwards
  digitalWrite(IN1Pin, HIGH);                                     // Motor 1
  digitalWrite(IN2Pin, LOW);
  digitalWrite(IN3Pin, HIGH);                                     // Motor 2
  digitalWrite(IN4Pin, LOW);
  Console.print(F("Wheel:Rev|"));
  delay(20);
  }


void Motor_Action_Stop_Motors()  {                                     // Motor Bridge pins are set for both motors to stop
  digitalWrite(ENAPin, 0);
  digitalWrite(IN1Pin, LOW);                                    //Motor 1
  digitalWrite(IN2Pin, LOW);

  digitalWrite(ENBPin, 0);                                      //Motor 2
  digitalWrite(IN3Pin, LOW);
  digitalWrite(IN4Pin, LOW);

  Console.print(F("Wheel:0FF|"));
}


void SetPins_ToTurnLeft()       {                              // Pins are set so that Motors drive in opposite directions
    digitalWrite(IN1Pin, LOW);                                   // Motor 1
    digitalWrite(IN2Pin, HIGH);
    digitalWrite(IN3Pin, HIGH);                                  // Motor 2
    digitalWrite(IN4Pin, LOW);
    Console.print(F("Wheel:TL_|"));  
    }


void SetPins_ToTurnRight() {                                    // Pins are set so that Motors drive in opposite directions
      digitalWrite(IN1Pin, HIGH);                                   // Motor 1
      digitalWrite(IN2Pin, LOW);
      digitalWrite(IN3Pin, LOW);                                    //Motor 2
      digitalWrite(IN4Pin, HIGH);
      Console.print(F("Wheel:R|"));
      }


// USed to turn the mower at a set speed.
void Motor_Action_Turn_Speed() {
      analogWrite(ENAPin, (PWM_MAX_SPEED_RH - robot.turnAdjust) );                                  // Change the 0 value to 10 or 20 to recuce the speed
      analogWrite(ENBPin, (PWM_MAX_SPEED_LH - robot.turnAdjust) );                                  // Change the 0 value to 10 or 20 to recuce the speed
      }


// Turns the mowing blades on
void Motor_Action_Spin_Blades()  {
  if (CUTTING_BLADES_ACTIVATE == 1) {                                       // Blades are turn ON in settings and will spin!
    delay(20);
    digitalWrite(R_EN, HIGH);
    digitalWrite(L_EN, HIGH);
    delay(20);
    analogWrite(RPWM, 255); //TODO SERVE ANCORA? 
    delay(20);
    Console.print("Blades:ON_|");
   }                 

  if (CUTTING_BLADES_ACTIVATE == 0) {                                     // Blades are turn off in settings and will not spin!
    void StopSpinBlades();

  }
}

void Motor_Action_Stop_Spin_Blades()  {
  delay(20);
  digitalWrite(R_EN, LOW);
  digitalWrite(L_EN, LOW);
  delay(20);
  Console.print("Blades:0FF|");
}

//Steers the Mower depending on the PID input from the Algorythm
void Motor_Action_Dynamic_PWM_Steering() {
      analogWrite(ENAPin, robot.pwmRight);                             // ENA low = Right Swerve   ENB low = Left Swerve
      analogWrite(ENBPin, robot.pwmLeft);
      Console.print("PWM_R:");
      Console.print(robot.pwmRight);
      Console.print("|");
      Console.print("PWM_L:");
      Console.print(robot.pwmLeft);
      Console.print("|");
}
