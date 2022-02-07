


// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "..."
#define BLYNK_DEVICE_NAME "..."
#define BLYNK_FIRMWARE_VERSION        "0.1.0"
#define BLYNK_PRINT Serial


//#define BLYNK_DEBUG
#define APP_DEBUG


// Enter the Auth code for the WEMOSD1 controlling the Wire ON/OFF
// If not used please leave blank "";
// char auth_WEMOS[] = "..";




// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"

SoftwareSerial NodeMCU(RX, TX);  //RXD2 TXD3

int   buttonState = 0;
int   Docked_Counts = 0;


// RX Variables
float val_VoltNow;                  // Battery Voltage from MEGA
float BatteryVoltage;               // Voltage read last RX
int Loop_Cycle_Mowing;              // Current Loop Cycle from MEGA

int Mower_Parked;
int Mower_Parked_Low_Batt;
int Mower_Docked;
int Mower_Docked_Filter;
int Mower_Running;
int Mower_Running_Filter;
int Mower_Lost;
int Charge_Detected;
int Tracking_Wire;
int Compass_Heading_Lock;


// Setting Sync 1
int Compass_Activate;
int Compass_Heading_Hold_Enabled;
int GYRO_Enabled;
int Sonar_1_Activate;
int Sonar_2_Activate;
int Sonar_3_Activate;
int Bumper_Activate_Frnt;


// Settings Sync 2
int Alarm_1_ON;
int Alarm_2_ON;
int Alarm_3_ON;
int GPS_Enabled;
int Angle_Sensor_Enabled;
int Tip_Over_Sensor_Enabled;
int Wheel_Amp_Sensor_ON;

int i;                              // Transmit blank code

bool Mower_Already_Started = 0;
bool Going_Home_Already = 0;

int j;
int k;
int transmit_blynk_code = 0;

bool Manuel_Mode;
int Automatic_Mode;
bool Wire_Status_ON_OFF;


int loopstatus = 0;
int All;
int RX_Data_Recieved;
int lastConnectionAttempt = millis();
int connectionDelay = 5000; // try to reconnect every 5 seconds

// MEGA Check
int Loop_Cycle_Mowing_Check_1;
int Loop_Cycle_Mowing_Check_2;
float val_VoltNow_Check_1;
float val_VoltNow_Check_2;
int Check_MEGA = 0;
int Error_Loop;
int Error_Volt;
int Total_Error;
int Max_Error = 40;
int Sync_Settings_Now = 0;


// Settings NODEMCU
//----------------------------------------------------

// Node MCU Version

char  Version[16] = "V9.2";

bool Auto_Sync_Settings_Enabled = 0;        // Peridoically Syncs the settings tab on the APP with actual Mower Settings
bool MEGA_Watch_Enabled = 1;                // Detects if the Mower MEGA has forzen and resets it
bool Auto_Wire = 0;                         // Turns the wire on and off automatically (Requires PCB version of Wire transmitter)


//----------------------------------------------------

WidgetLED ParkedLED(V6);
WidgetLED MowingLED(V7);
WidgetLED DockedLED(V8);
WidgetLED TrackingLED(V9);
WidgetLED ChargingLED(V12);
WidgetLED ErrorLED(V11);



BlynkTimer timer;
WidgetBridge PCBWire_WEMOSD1(V1);

//BLYNK_CONNECTED() {
  // Place the AuthToken of the WEMOS D1 for the wire control
  // PCBWire_WEMOSD1.setAuthToken(auth_WEMOS);
//}


void myTimerEvent()  {
  if (  (BatteryVoltage > 10) && (BatteryVoltage < 15) ) {
    Blynk.virtualWrite(V3, BatteryVoltage);
  }

  Blynk.virtualWrite(V5, Loop_Cycle_Mowing);


}

void setup()
{
  Serial.begin(9600);
  NodeMCU.begin(9600);
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.print("ReP_AL Mower Node MCU Version:");
  Serial.println(Version);
  Serial.println("Setting up NODEMCU........");
  Serial.println("");
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);
  pinMode(D5, OUTPUT);
  digitalWrite(D5, HIGH);                             // Reset Switch
  pinMode(D6, OUTPUT);                                // ESP32 CAM ON/OFF Relay
  digitalWrite(BOARD_LED_PIN, HIGH);                          // Turn off LED Light 
  pinMode(BOARD_LED_PIN, OUTPUT);
  timer.setInterval(1000L, myTimerEvent);
  BlynkEdgent.begin();

  Clear_APP();
  }

void loop() {


  loopstatus = loopstatus + 1 ;
  RX_Data_Recieved = 0;                         // resets the data received bool

  BlynkEdgent.run();
  timer.run();
  //timer.run();
  //digitalWrite(LED, LOW);         //LED is inverted on a MODEMCU...
  Receive_All_From_MEGA();
  Update_Blynk_App_With_Status();
  if (Auto_Wire == 1) Set_PCB_Wire_Status();
  if (MEGA_Watch_Enabled == 1) Check_MEGA_Status();
  if (Auto_Sync_Settings_Enabled == 1) Sync_Settings();
  Serial.println("");     // new line serial monitor
  if (!Blynk.connected()) digitalWrite(BOARD_LED_PIN, LOW);
  if (!Blynk.connected()) digitalWrite(BOARD_LED_PIN, HIGH);
}
