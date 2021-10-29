/**
 * SETUP OF MOWER
 * The following setup parameters will setup the mower for your garden 
 * Turn on or off the settings to defien how you like the mower to behave.
 * 
 *  1 = Turned ON      0 = Turned OFF       Value = Value set for variable.
 * 
*/

#include "translations.h"
#include "mower.h"

char Version[16] = "V6.31";

int TFT_Screen_Menu = 0; //add per 8.9
int Wire_Refind_Tries = 0;

bool Cutting_Blades_Activate    = 1; // Activates the cutting blades and disc in the code
bool WIFI_Enabled               = 0; // Activates the WIFI Fucntions
bool isPerimeterWireEnabled     = 1; // ATTENZIONE viene pilotato dal pin11, Activates use of the perimeter boundary wire 
int MIN_WIRE_MAG = 3; //default 20
int MAX_WIRE_FAIL = 15; //how many time wire must fail before stop mover

//Docking Station
bool Use_Charging_Station       = 1; // 1 if you are using the docking/charging station 0 if not
bool CW_TRACKING_SEARCHING_CHARGE      = 1; // 1-Clock-Wise 0-Counter-Clock-Wise tracking around the boundary wire to the charging station
bool CW_TRACKING_EXITING_CHARGE       = 0; // 1-Clock-Wise 0-Counter-Clock-Wisetracking around the boundary wire when tracking to the start position
#define Docked_Filter_Hits         = 1; // Number of charge signals to be detected before mower powers off

int Track_Wire_Zone_1_Cycles    = 600;                       // Zone 1 - Number of Itterations the PID function does before the mower exits the wire track
int Track_Wire_Zone_2_Cycles    = 1000;                       // Zone 2 - Therefore how long the mower is tracking the wire can be set = distance tracked.

int  Max_Tracking_Turn_Right    = 270;                        // The maximum number of turn right commands during wire tracking before a renewed wire find function is called
int  Max_Tracking_Turn_Left     = 270;                        // This helps to re-find the wire should the mower loose the wire for any reason.
int  Max_Cycle_Wire_Find        = 320;                        // Maximum number of forward tracking cycles in finding wire before the mower restarts a compass turn and wire find.

//Compass Settings
bool Compass_Activate               = 1;                      // Turns on the Compass (needs to be 1 to activate further compass features)
bool Compass_Heading_Hold_Enabled   = 1;                      // Activates the compass heading hold function to keep the mower straight
int  Home_Wire_Compass_Heading      = 110;                    // Heading the Mower will search for the wire once the mowing is completed.
int  CPower                         = 2;                      // Magnification of heading to PWM - How strong the mower corrects itself in Compass Mowing
bool Pattern_Mow                    = 0;  //*** Not Working yet  // Activate pattern mowing instead of random mowing direction
int  Compass_Mow_Direction          = 110;                    // Mow Direction of line when pattern mow is activated

//Rain sensor
bool Rain_Sensor_Installed          = 0;                      // 1 for Rain sensor installed    0 for no sensor installed.
#define Rain_Total_Hits_Go_Home 10;                      // This sensor only makes sense in combination with a mower docking station
// as the mower is sent there to get out of the rain.
//Battery Settings
float Battery_Max               = 12.6;                       // Max battery volts in Volts. 3S = 12.6V
float Battery_Min               = 10.5; //11.4; alzare il limite quando dovrà tornare a casa                       // Lower Limit of battery charge before re-charge required.
int  Low_Battery_Detected      = 0;       //TODO era byte                    // Always set to 0
#define  Low_Battery_Instances_Chg  14;       //TODO era byte                  // Instances of low battery detected before a re-charge is called..

//Sonar Modules
bool Sonar_1_Activate           = 1;                          // Activate (1) Deactivate (0) Sonar 1
bool Sonar_2_Activate           = 1;                          // Activate (1) Deactivate (0) Sonar 2
bool Sonar_3_Activate           = 1;                          // Activate (1) Deactivate (0) Sonar 3
int  Max_Sonar_Hit              = 2;//default 3                          // Maximum number of Sonar hits before object is discovered
long maxdistancesonar           = 30;                         // distance in cm from the mower that the sonar will activate at.

//Wheel Motors Setup
int Max_Cycles                 = 150;                         // Number of loops the Sketch will run before the mower just turns around anyway.
int PWM_MaxSpeed_LH            = 255;// era 240 boh 240;     //EEPROM            // Straight line speed LH Wheel (Looking from back of mower)  Will be overidden if saved in EEPROM
int PWM_MaxSpeed_RH            = 255;     //EEPROM            // Straight line speed RH Wheel - adjust to keep mower tracking straight.  Will be overridden if saved in EEPROM

int Max_Motor_PWM_LH           = 255;
int Max_Motor_PWM_RH           = 255;
int Mower_Turn_Delay_Min        = 1000;                       // Min Max Turn time of the Mower after it reverses at the wire.
int Mower_Turn_Delay_Max        = 2500;                       // A random turn time between these numbers is selected by the software
int Mower_Reverse_Delay         = 1800;                       // Time the mower revreses at the wire



//Blade Motor Setup
//Blade Speed can be modified in the settings menu and will be written to EEPROM
//The number below will then be overidden
int PWM_Blade_Speed            = 250;      //EEPROM          // PWM signal sent to the blade motor (speed of blade) new motor works well at 245. //TODO era byte

// Alarm Setup
bool Set_Time                   = 0;                          // Turn to 1 to set time on RTC (Set time in Time tab Set_Time_On_RTC)  After setting time turn to 0 and reload sketch.


// If the Alarm is changed in settings it will be written to EEPROM and the settings below will be overriden.
// Action for Alarm 1 is set to exit the dock and mow at this time.
// To change this action go to "void checkSchduledStartig()"
bool Alarm_1_ON                 = 0;       //EEPROM            // Activate Alarm 1  (1 = ON 0 = OFF)
int Alarm_1_Hour                = 12;      //EEPROM            // Mowing Hour Number 1
int Alarm_1_Minute              = 00;      //EEPROM            // Alarm Minute 1
bool Alarm_1_Repeat             = 0;                           // Repeat the Alarm at the same time

// Action for Alarm 2 can be set in "void checkSchduledStartig()"
bool Alarm_2_ON                 = 0;       //EEPROM            // Activate Alarm 2 (1 = ON 0 = OFF)
int Alarm_2_Hour               = 12;       //EEPROM            // Mowing Hour Number 2
int Alarm_2_Minute             = 00;       //EEPROM            // Alarm minute 2
bool Alarm_2_Repeat             = 0;                           // Repeat the Alarm at the same time

// Action for Alarm 3 can be set in "void checkSchduledStartig()"
// Go Home Alarm
bool Alarm_3_ON                 = 0;       //EEPROM            // Activate Alarm 3 (1 = ON 0 = OFF)
int Alarm_3_Hour               = 12;       //EEPROM            // Mowing Hour Number 3
int Alarm_3_Minute             = 00;       //EEPROM            // Alarm minute 3
bool Alarm_3_Repeat             = 0;                           // Repeat the Alarm at the same time

#define Alarm_Second               = 5;                          // Seconds //TODO era byte


/* Description of how the below values are displayed in the Serial Monitor Print Out for the wire
   function
   (InMax)                   Wire = 0                 (OutMax)
       |      (InMid)           |           (OutMid)     |
       |--------|--------|------|------|--------|--------|
       |        |        |      |      |        |        |
                      (InMin)       (OutMin)
*/

// Wire detection Values
/*Negative Values for In*/                                    // These values are based on the signal received by the wire sensor for my perimeter loop
int InMin = -200;
int InMid = -700;
int InMax = -1500;                                            // the maximum received signal value  the wire
/*General Setup PID numbers for wire tracking*/
float P               = 1.2;//0.08;              //EEPROM           // Multiplication factor to the error measured to the wire center.  if jerky movement when tracking reduce number
float D               = 20;                                   // Dampening value to avoid the mower snaking on the wire.
#define Scale 36;     //TODO era byte                              // Serial Monitor Line Tracking Print Scale

/*Positive Values for Out*/
int OutMin = 150;
int OutMid = 400;
int OutMax = 1500;                                            // the maximum received signal value outside the wire

int Outside_Wire_Count_Max          = 15;                     // If the mower is outside the wire this many times the mower is stopped
int Action_On_Over_Wire_Count_Max   = 3;                      // Set 1 to hibernate mower (Power Off and Stop)   Set 2 to refind garden using sonar and wire detect function
// 3 to do a refind wire function

bool Show_TX_Data                   = 0;                      // Show the values recieved from the Nano / ModeMCU in the serial monitor
