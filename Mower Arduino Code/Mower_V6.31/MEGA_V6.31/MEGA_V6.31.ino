// ReP_AL Lawn Mower Arduino Code
// Please make sure you have installed all the library files to the Arduino libraries folder
// You will need to unzip all the libraries from the GitHuB site.
// Instructions onnhow to do this are available on my webpage
// www.repalmkaershop.com

//Libraries for Perimeter Wire Receiver
#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>

#include "config.h"
#include "adcman.h"
#include "drivers.h"
#include "perimeter.h"

//Libraries for Real Time Clock
#include <DS1302.h>
#include <stdio.h>

//Libraries for ic2 Liquid Crystal
#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//Libraries for the Mowing Calendar Function
#include <TimeAlarms.h>
#include <TimeLib.h>
AlarmId id;

//Compass Setup
#include <DFRobot_QMC5883.h>
DFRobot_QMC5883 compass;

/****************************
 * Global Variables
 *****************************/

//Perimeter Variables
Perimeter perimeter;
unsigned long nextTime = 0;
int counter = 0;
boolean inside = true;

byte Loop_Cycle_Mowing = 0;

//Sonar Variables
long duration1 = 0;  // Time required for the sonar ping to be recieved by the echo pin.
long duration2 = 0;
long duration3 = 0;

int distance1 = 999;  // Distance caculated  by the Sonar
int distance2 = 999;
int distance3 = 999;

int distance_blockage;

int Sonar_Hit_1_Total;
int Sonar_Hit_2_Total;
int Sonar_Hit_3_Total;
bool Sonar_Hit_Any_Total;
bool Sonar_Hit_1 = 0;
bool Sonar_Hit_2 = 0;
bool Sonar_Hit_3 = 0;
bool Sonar_Hit = 0;

//Mower Status Variables
bool Mower_Docked;
bool Mower_Parked;
bool Mower_Running;
bool Mower_Parked_Low_Batt;
bool Mower_Error;
bool Manuel_Mode;

//Membrane Key Variables
byte Start_Key_X;
byte Plus_Key_X;
byte Minus_Key_X;
byte Stop_Key_X;
bool Menu_Complete;
byte Menu_Mode_Selection;
int Menu_View;
int Mow_Time_Set;

//Serial Communication
float Volts;
float Volts_Last;
int Zero_Volts;
float Amps;
float VoltageAmp;
int RawValueAmp;
int RawValueVolt;
int Rain_Detected;
int Rain_Hit_Detected = 0;
//float Battery_Voltage_Last;
float Amps_Last;
int Volts_Outside_Reading;
byte OK_Nano_Data_Volt_Received;
byte OK_Nano_Data_Charge_Received;
byte Charge_Hits = 0;
byte Docked_Hits = 0;
bool Charge_Detected_MEGA = 0;

//Mow Calendar Variables
byte Alarm_Hour_Now;
byte Time_Hour;
byte Time_Minute;
byte Time_Second;
byte Time_Day;
byte Time_Month;
byte Time_Year;
byte Time_Date;
bool Alarm_Timed_Mow_ON = 0;
byte Alarm_Timed_Mow_Hour;    // Mowing Hour Number 3
byte Alarm_Timed_Mow_Minute;  // Alarm minute 3

int Alarm_1_Saved_EEPROM;
int Alarm_2_Saved_EEPROM;
int Alarm_3_Saved_EEPROM;

//Perimeter Wire Tracking
int I;
int Track_Wire_Itterations;
bool Outside_Wire;
byte Exit_Zone;
int MAG_Now;
int MAG_OUT_Stop;
int MAG_IN_Stop;
int MAG_TURN;
int MAG_Average_Start;
int MAG_Last;
byte Outside_Wire_Count = 0;
int Tracking_Wire = 0;
bool Wire_ON_Printed;
int wireOffCounter;  //count how many time wire is OFF

int Tracking_Turn_Left;
int Tracking_Turn_Right;
bool Mower_Track_To_Charge;
bool Mower_Track_To_Exit;

bool Abort_Wire_Find;
bool No_Wire_Found;

int PWM_Right;
int PWM_Left;
int MAG_Goal;
int MAG_Error;
int MAG_Start;
byte PWM_Blade_Speed_Min;
byte PWM_Blade_Speed_Max;
bool Blade_Override = 0;

//Compass Variables
float Compass_Heading_Degrees;
float Heading;
bool Compass_Heading_Locked = 0;
float Heading_Lock;
int Heading_Upper_Limit_Compass;
int Heading_Lower_Limit_Compass;
int Compass_Target;
int Compass_Leg = 0;
int Turn_Adjust = 0;

//Wire Track Printer
int PrintInMax;
int PrintInMid;
int PrintInMin;
int PrintOutMin;
int PrintOutMid;
int PrintOutMax;
int PrintMAG_Now;

//WIFI Variables
float val_WIFI;

//EEPROM Saved Settings
int PWM_LEFT_EEPROM;
int PWM_RIGHT_EEPROM;
int PWM_BLADE_EEPROM;
int COMPASS_EEPROM;
float Traking_PID_P_EEPROM;

void setup() {
    Serial.begin(115200);

    //Open Serial port 1 for the nano communication
    //2021-10-23 disattivato nano Serial1.begin(1200);

    Serial.println(" ");
    Serial.println(" ");
    Serial.print("Ottomower Robot :");
    Serial.println(Version);
    Serial.println("===================");
    Serial.println("");
    Serial.println("Starting Mower Setup");
    Serial.println("===================");

    Load_EEPROM_Saved_Data();
    if (Set_Time == 1) {
        Serial.print("Setting Time");
        Set_Time_On_RTC();
    }
    DisplayTime();
    Serial.println("");

    //Logia per gestione cavo manuale
    pinMode(USE_CABLE_PIN, INPUT);
    isPerimeterWireEnabled = digitalRead(USE_CABLE_PIN);
    Serial.print("Use cable: ");
    Serial.println(isPerimeterWireEnabled);

    Prepare_Mower_from_Settings();
    Setup_Run_LCD_Intro();
    Setup_Relays();
    Setup_Membrane_Buttons();
    Setup_Motor_Pins();
    Setup_Compass();
    Setup_ADCMan();
}

void loop() {
    readVoltAmp();     //update volt and amp info
    checkIfRaining();  // Checks if the water sensor detects Rain

    logMowerStatus();  // Update the Serial monitor with the current mower status.

    lcdUpdateScreen();  //Update LCD Info


    if (Mower_Docked == 1) {  // Mower is docked waiting for a command to leave and mow.
        checkMembraneSwitchInputDocked();  // Check the membrane buttons for any input
        manouverDockTheMower();            //set robot in dock status
        checkSchduledStartig();            //if alarm is set, start the robots
    }

    if (Mower_Parked == 1              // Mower is Parked ready to be started / re-started.
        || Mower_Parked_Low_Batt == 1  // Mower is Parked with Low Battery needing manuel charging
        || Mower_Error == 1) {         // Lost mower is put into standby mode

        checkMembraneSwitchInputParked();  // Check the membrane buttons for any input
    }

    if (Mower_Parked == 1) {
        manouverParkTheMower();
    }

    // Mower is running cutting the grass.
    bool wireOn = isWireOn();
    if (Mower_Running == 1) Process_Volt_Information();                                                            // Take action based on the voltage readings
    if (Mower_Running == 1) Check_Membrane_Keys_Running();                                                         // Check to see if the mower needs to be stopped via keypad
    if (Mower_Running == 1) Check_Timed_Mow();                                                                     // Check to see if the time to go home has come.
    if ((Mower_Running == 1) && wireOn) Check_Wire_In_Out();                                                       // Test if the mower is in or out of the wire fence.
    if ((Mower_Running == 1) && wireOn && (Outside_Wire == 0)) Check_Sonar_Sensors();                              // If the mower is  the boundary wire check the sonars for obsticles and prints to the LCD
    if ((Mower_Running == 1) && wireOn && (Outside_Wire == 0) && (Sonar_Hit == 0)) Manouver_Mow_The_Grass();       // Inputs to the wheel motors / blade motors according to surroundings
    if ((Mower_Running == 1) && wireOn && (Outside_Wire == 1) && (Loop_Cycle_Mowing > 0)) Manouver_Turn_Around();  // If outside the wire turn around
    if ((Mower_Running == 1) && wireOn && (Outside_Wire == 0) && (Sonar_Hit == 1)) Manouver_Turn_Around_Sonar();   // If sonar hit is detected and mower is  the wire, manouver around obsticle

    // WIFI Commands from and to APP
    if (Manuel_Mode == 1) Receive_WIFI_Manuel_Commands();
    if (Manuel_Mode == 1) Print_LCD_Info_Manuel();
    if ((WIFI_Enabled == 1) && (Manuel_Mode == 0)) Get_WIFI_Commands();

    Serial.println();

}  // End Loop

void logMowerStatus() {
    if (Mower_Docked == 1) Serial.print("Docked:1|");
    if (Mower_Parked == 1) Serial.print("Parked:1|");
    if (Mower_Running == 1) Serial.print("Running:1|");
    if (Manuel_Mode == 1) Serial.print("Manuel Mode:1|");
    if (Mower_Parked_Low_Batt == 1) Serial.print("Park_Low_Batt:1|");
    if (Mower_Error == 1) Serial.print("Mower Error:1|");

    if (Mower_Docked == 1) {
        Serial.print("Time:");
        Time t = rtc.time();
        Serial.print(t.hr);
        Serial.print(":");
        if (t.min < 10) Serial.print("0");
        Serial.print(t.min);
        Serial.print(".");
        if (t.sec < 10) Serial.print("0");
        Serial.print(t.sec);

        //log alarm data
        if (Alarm_1_ON == 1) {
            Serial.print("|Alarm 1:");
            Serial.print(Alarm_1_Hour);
            Serial.print(F(":"));
            if (Alarm_1_Minute < 10) Serial.print("0");
            Serial.print(Alarm_1_Minute);
            Serial.print("|");
        } else {
            Serial.print("|Alarm 1 OFF");
        }

        if (Alarm_2_ON == 1) {
            Serial.print("|Alarm 2:");
            Serial.print(Alarm_2_Hour);
            Serial.print(F(":"));
            if (Alarm_2_Minute < 10) Serial.print("0");
            Serial.print(Alarm_2_Minute);
            Serial.print("|");
        } else {
            Serial.print("|Alarm 2 OFF");
        }

        if (Alarm_3_ON == 1) {
            Serial.print("|Alarm 3:");
            Serial.print(Alarm_3_Hour);
            Serial.print(F(":"));
            if (Alarm_3_Minute < 10) Serial.print("0");
            Serial.print(Alarm_3_Minute);
            Serial.print("|");
        } else {
            Serial.print("|Alarm 3 OFF");
        }
    }
}
