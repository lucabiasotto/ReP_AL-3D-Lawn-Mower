/**
 * config.h
 *
 * SETUP OF MOWER
 * The following setup parameters will setup the mower for your garden
 * Turn on or off the settings to defien how you like the mower to behave.
 *
 *  1 = Turned ON      0 = Turned OFF       Value = Value set for variable.
 */

// Alarm Setup
// Turn to 1 to set time on RTC (Set time in Time tab
// Set_Time_On_RTC)  After setting time turn to 0 and reload sketch.
#define SET_TIME false
#define CUTTING_BLADES_ACTIVATE 1  // Activates the cutting blades and disc in the code

//Wire params
#define PERIMETER_WIRE_ENABLED 1
#define FOLLOW_WIRE_WHEN_LEAVE_DOCK false
#define MIN_WIRE_MAG 3    // default 20
#define MAX_WIRE_FAIL 15  // how many time wire must fail before stop mover

// Docking Station
#define USE_CHARGING_STATION 1          // 1 if you are using the docking/charging station 0 if not
#define CW_TRACKING_SEARCHING_CHARGE 1  // 1-Clock-Wise 0-Counter-Clock-Wise tracking around the boundary wire to the charging station
#define CW_TRACKING_EXITING_CHARGE 0    // 1-Clock-Wise 0-Counter-Clock-Wisetracking around the boundary wire when tracking to the start position

//TODO tara
#define TRACK_WIRE_ZONE_CYCLES 600   // Zone 1 - Number of Itterations the PID function does before the mower exits the wire track

#define MAX_TRACKING_TURN_RIGHT 270  // The maximum number of turn right commands during wire tracking before a renewed wire find function is called
#define MAX_TRACKING_TURN_LEFT 270   // This helps to re-find the wire should the mower loose the wire for any reason.
#define MAX_CYCLE_WIRE_FIND 500      // Maximum number of forward tracking cycles in finding wire before the mower restarts a compass turn and wire find.

// Compass Settings
#define COMPASS_ACTIVATE 1              // Turns on the Compass (needs to be 1 to activate further compass features)
#define CYCLE_FOR_STABILEZE 10           // number of cycle to ignore when do compass compensation
#define COMPASS_HEADING_HOLD_ENABLED 1  // Activates the compass heading hold function to keep the mower straight
#define HOME_WIRE_COMPASS_HEADING 110   // heading the Mower will search for the wire once the mowing is  completed.
#define COMPASS_CORRECTION_POWER 2      // Magnification of heading to PWM - How strong the mower  corrects itself in Compass Mowing
#define COMPASS_MOW_DIRECTION 110       // Mow Direction of line when pattern mow is activated

// Rain sensor
#define RAIN_SENSOR_INSTALLED 1     // 1 for Rain sensor installed    0 for no sensor installed.
#define RAIN_TOTAL_HITS_GO_HOME 10  // MAX 127 This sensor only makes sense in combination with a mower docking station as the mower is sent there to get out of the rain.

// Battery Settings
#define BATTERY_MAX 12.6              // Max battery volts in robot.volts. 3S = 12.6V
#define BATTERY_MIN 3.5              // 11.0 alzare il limite quando dovrà tornare a casa // Lower Limit of battery charge before re-charge required.
#define LOW_BATTERY_INSTANCES_CHG 14  // Instances of low battery detected before a re-charge is called..

// Sonar Modules
#define SONAR_1_ACTIVATE 1                   // Activate (1) Deactivate (0) Sonar 1
#define SONAR_2_ACTIVATE 1                   // Activate (1) Deactivate (0) Sonar 2
#define SONAR_3_ACTIVATE 1                   // Activate (1) Deactivate (0) Sonar 3
#define SONAR_MAX_HIT 2                      // default 3, Maximum number of Sonar hits before object is discovered
#define SONAR_MAX_DISTANCE 30                // distance in cm from the mower that the sonar will activate at.
#define SONAR_TURN_OBSTACLE_DELAY_TIME 2500  // A random turn time between these  numbers is selected by the software
#define SONAR_REVERSE_DELAY 1800             // Time the mower revreses at the wire

// Wheel Motors Setup
#define MAX_CYCLES 150        //TODO tarare // Number of loops the Sketch will run before the  mower just turns around anyway.
#define PWM_MAX_SPEED_LH 255  // era 240 boh 240  Straight line speed  LH Wheel (Looking from back of mower)  Will be overidden if saved in EEPROM
#define PWM_MAX_SPEED_RH 255  // Straight line speed RH Wheel - adjust to  keep mower tracking straight.  Will be overridden if saved in  EEPROM

/* Description of how the below values are displayed in the Serial Monitor
   Print Out for the wire function (InMax)                   Wire 0 (OutMax)
       |      (InMid)           |           (OutMid)     |
       |--------|--------|------|------|--------|--------|
       |        |        |      |      |        |        |
                      (InMin)       (OutMin)
*/

// Wire detection Values
// These values are based on the signal received
// by the wire sensor for my perimeter loop

/*Negative Values for In*/
#define WIRE_IN_MIN -200
#define WIRE_IN_MID -700
#define WIRE_IN_MAX -1500  // the maximum received signal value  the wire

/*General Setup PID numbers for wire tracking*/
//#define P 1.2; //TODO NON USATO // 0.08              //EEPROM           // Multiplication  factor to the error measured to the wire center.  if   jerky movement when tracking reduce number
//#define D 20   //TODO NON USATO    // Dampening value to avoid the mower snaking on the wire.
#define SCALE 36  // Serial Monitor Line Tracking Print Scale

/*Positive Values for Out*/
#define WIRE_OUT_MIN 150
#define WIRE_OUT_MID 400
#define WIRE_OUT_MAX 1500  // the maximum received signal value outside the wire

#define WIRE_OUTSIDE_COUNT_MAX 15        // If the mower is outside the wire this many times the mower is stopped
#define ACTION_ON_OVER_WIRE_COUNT_MAX 3  // Set 1 to hibernate mower (Power Off and Stop)               \
                                         // Set 2 to refind garden using sonar and wire detect function \
                                         // Set 3 to do a refind wire function
