/**
 * In this class there are pin setup for Arduino MEGA
 */
#include <Arduino.h>
#include <DS1302.h>
#include "perimeter.h"
#include "translations.h"
#include "settings.h"
#include <LiquidCrystal_I2C.h>

#define Console Serial

// Perimeter Wire Pins
#define pinPerimeterLeft A5   // perimeter sensor pin
#define pinPerimeterRight A4  // leave open
#define pinLED LED_BUILTIN

// Sonar Setup for Front Sonars 1-3
// S1
#define echoPin1 34
#define trigPin1 35
// S2
#define echoPin2 36
#define trigPin2 37
// S3
#define echoPin3 38
#define trigPin3 39

// Membrane Switch
#define Start_Key 50  // connect wire 1 to pin 2
#define Plus_Key 51   // connect wire 2 to pin 3
#define Minus_Key 52  // connect wire 3 to pin 4
#define Stop_Key 53   // connect wire 4 to pin 5

// Pin Setup for the wheel Motor Bridge Controller
// Motor A
#define ENAPin 7  // EN Pins need a digital pin with PWM
#define IN1Pin 6  // IN Pins dont need digital PWM
#define IN2Pin 5
// Motor B
#define ENBPin 2  // EN Pins need a digital pin with PWM
#define IN3Pin 4  // IN Pins dont need digital PWM
#define IN4Pin 3

// Motor Blades
#define RPWM 8
#define L_EN 9
#define R_EN 10

// Relay Switch
#define Relay_Motors \
    24  // be careful that you really use PIN24. The order is sometimes labelled \
        // so it looks like 24 is actually 22.

// Pin per controllare se usare o no il cavo
#define USE_CABLE_PIN 11

//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

class Mower {
   public:
    //Perimeter Variables
    unsigned long nextTime = 0;
    int counter = 0;
    boolean inside = true;
    int wireRefindTries = 0;
    int loopCycleMowing = 0;
    Perimeter perimeter;
    LiquidCrystal_I2C lcdDisplay = LiquidCrystal_I2C(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
    
    //Sonar Variables
    int distance1 = 999;  // Distance caculated  by the Sonar
    int distance2 = 999;
    int distance3 = 999;
    int distanceBlockage;
    int SonarHit1Total;
    int SonarHit2Total;
    int SonarHit3Total;

    //Mower Status Variables
    bool mowerDocked;
    bool mowerParked;
    bool mowerRunning;
    bool mowerParkedLowBatt;
    bool mowerError;
    bool manuelMode;

    //Serial Communication
    float volts;
    float voltsLast;
    int zeroVolts;
    float amps;
    float voltageAmp;
    int rawValueAmp;
    int rawValueVolt;
    int rainDetected;
    int rainHitDetected = 0;
    bool chargeDetectedMEGA = 0;

    //Sonar
    byte sonarHit = 0;

    //Battery variable
    byte lowBatteryDetectedCount = 0;
    char charging;

    //Mow Calendar Variables
    bool alarmTimedMowON = 0;
    byte alarmTimedMowHour;    // Mowing Hour Number 3
    byte alarmTimedMowMinute;  // Alarm minute 3
    //char Mow_Time_Set; //TODO forse si può togliere

    //Perimeter Wire Tracking
    int I;           //TODO che minchia è?
    double P = 1.2;  //TODO che minchia è? dobule? float?     // 0.08//EEPROM           // Multiplication  factor to the error measured to the wire center.  if   jerky movement when tracking reduce number
    int trackWireItterations;
    bool outsideWire;
    int magNow;
    byte outsideWireCount = 0;
    int trackingWire = 0;
    bool wireOnPrinted;  //TODO perchè?
    int wireOffCounter;  //count how many time wire is OFF

    int trackingTurnLeft;
    int trackingTurnRight;
    bool mowerTrackToCharge;
    bool mowerTrackToExit;

    bool abortWireFind;
    bool noWireFound;

    int pwmRight;
    int pwmLeft;
    int magGoal;
    int magError;
    int magStart;
    bool bladeOverride = 0;

    //Compass Variables
    float compassHeadingDegrees;
    float heading;
    bool compassHeadingLocked = 0;
    float headingLock;
    int headingUpperLimitCompass;
    int headingLowerLimitCompass;
    int compassTarget;
    int compassLeg = 0;
    int turnAdjust = 0;

    //Wire Track Printer
    int printInMax;
    int printInMid;
    int printInMin;
    int printOutMin;
    int printOutMid;
    int printOutMax;
    int printMAG_Now;

    Mower();
    virtual void setup();
    virtual void loop();
    virtual void Setup_Relays();
    virtual void Turn_Off_Relay();
    virtual void Turn_On_Relay();
    virtual void logMowerStatus();

   private:
    virtual void Prepare_Mower_from_Settings();
    virtual void Setup_Motor_Pins();
    virtual void Setup_Membrane_Buttons();
    virtual void Setup_ADCMan();
};

extern Mower robot;
