/**************************************
 *            TRANSLATIONS
 ***************************************/
 
#define Version "V1.0"

//custom i2c lcd char

#define ENGLISH 0
#define ITALIAN 1 
#define LANGUAGE ITALIAN

#if LANGUAGE == ENGLISH

#define TRS_PARKED "Parked"
#define TRS_ROBOT_NAME "Ottomower"
#define TRS_MANUAL "Manual"
#define TRS_DOCKED "Docked"
#define TRS_RAIN "Rain"
#define TRS_RECHARGE_BATT "Recharge Batt"
#define TRS_WIRE_DETECT "WIRE Detect "
#define TRS_MOVING__ "Mowing..    "
#define TRS_RAINING "RAINING"
#define TRS_SONAR_OBJECT "Sonar Object "
#define TRS_COMPASS_HOME "Compass Home"
#define TRS_TARGET "Target:"
#define TRS_DOCKED_IN "Docked in"
#define TRS_CHARGING_STATION "Charging Station"
#define TRS_MENU_CANCELLED "Menu Cancelled"
#define TRS_WIRE_OFF ":WIRE OFF"
#define TRS_WIRE_OFF_ ":WIRE OFF        "
#define TRS_ALARM_1 "Alarm 1"
#define TRS_ALARM_2 "Alarm 2"
#define TRS_ALARM_3 "Alarm 3"
#define TRS_WHEEL_SPEED_LH "Wheel Speed LH"
#define TRS_WHEEL_SPEED_RH "Wheel Speed RH"
#define TRS_BLADE_SPEED "Blade Speed"
#define TRS_COMPASS_ON_OFF "Compass ON/OFF"
#define TRS_TRACKING_PID "Tracking PID"
#define TRS_SET_CLOCK "Set Clock"
#define TRS_SAVED "Saved"
#define TRS_QUICK_START "Quick Start"
#define TRS_TRAMPOLINE_CUT "Trampoline Cut"
#define TRS_MOW_THE_LINE "Mow the Line"
#define TRS_TEST_MENU "Test Menu"
#define TRS_SETUP_MOWER "Setup Mower"
#define TRS_MANUAL_START "Manual Start"
#define TRS_BLADE_WILL_SPIN "Blade will spin"
#define TRS_MOW_TIME_MAX "Max Mow Time"
#define TRS_MOW_TIME_1HR "1hr Mow Time"
#define TRS_MOW_TIME_2HR "2hr Mow Time"
#define TRS_H_LOCK_LEFT "<H-Lock Mow "
#define TRS_H_LOCK_RIGHT " H-Lock Mow>"
#define TRS_H_LOCK_OK "|H-Lock Mow|"
#define TRS_MAX_MOW "Max Mow"
#define TRS_SELECTE "Selected"
#define TRS_1HR_MOW_SELECTED "1hr Mow Selected"
#define TRS_ENDS_ "Ends: "
#define TRS_2HR_MOW_SELECTED "2hr Mow Selected"
#define TRS_MOW_GRASS__ "Mow Grass     "
#define TRS_GO_TO_DOCK "Go To Dock"
#define TRS_DOCK_REMOVED "Dock Removed"
#define TRS_MOW_RE_STARTING "Mow Re-Starting"
#define TRS_RETURNING_HOME "Returning Home"
#define TRS_NO_DOCK_ACTIVE "No Dock Active"
#define TRS_CHARGING "Charging"
#define TRS_RECHARG_BAT "Recharge Batt"
#define TRS_FIND_WIRE "Finding Wire"
#define TRS_START_CUT "Start"
#define TRS_START_CUT_NO_WIRE "Start (NO WIRE)"
#define TRS_NO_WIRE "WIRE OFF"
#define TRS_WIRE "WIRE"
#define TRS_ROBOT_LOCK "ROBOT LOCK"
#define TRS_GO_HOME "Go home"

#endif

#if LANGUAGE == ITALIAN

#define TRS_PARKED "Parcheggiato"
#define TRS_ROBOT_NAME "Fam.Bias8 Robot"
#define TRS_MANUAL "Manuale"
#define TRS_DOCKED "Nel Box"
#define TRS_RAIN "Piove"
#define TRS_RECHARGE_BATT "Ricaricare Batteria"
#define TRS_WIRE_DETECT "Cavo Rilevato "
#define TRS_MOVING__ "In movimento..    "
#define TRS_RAINING "STA PIOVENDO"
#define TRS_SONAR_OBJECT "Ostacolo Sonar "
#define TRS_COMPASS_HOME "Cerca Box(B)"//tramite bussol
#define TRS_TARGET "Obiettivo:"
#define TRS_DOCKED_IN "Nel Box"
#define TRS_CHARGING_STATION "Stazione Ricarica"
#define TRS_MENU_CANCELLED "Menu Annullato"
#define TRS_WIRE_OFF ":CAVO OFF"
#define TRS_WIRE_OFF_ ":CAVO OFF        "
#define TRS_ALARM_1 "Alarm 1"
#define TRS_ALARM_2 "Alarm 2"
#define TRS_ALARM_3 "Alarm 3"
#define TRS_WHEEL_SPEED_LH "Vel. ruota SX"
#define TRS_WHEEL_SPEED_RH "Vel. ruota DX"
#define TRS_BLADE_SPEED "Vel. Lama"
#define TRS_COMPASS_ON_OFF "Compass ON/OFF"
#define TRS_TRACKING_PID "Tracking PID"
#define TRS_SET_CLOCK "Imposta Ora"
#define TRS_SAVED "Salvato"
#define TRS_QUICK_START "Avvio Rapido"
#define TRS_TRAMPOLINE_CUT "Taglio preciso"
#define TRS_MOW_THE_LINE "Segui il cavo"
#define TRS_TEST_MENU "Test Menu"
#define TRS_SETUP_MOWER "Setup Robot"
#define TRS_MANUAL_START "Manual Start"
#define TRS_BLADE_WILL_SPIN "Blade will spin"
#define TRS_MOW_TIME_MAX "Tempo Massimo"
#define TRS_MOW_TIME_1HR "1 ora"
#define TRS_MOW_TIME_2HR "2 ore"
#define TRS_H_LOCK_LEFT "<  Taglio   "
#define TRS_H_LOCK_RIGHT "   Taglio  >"
#define TRS_H_LOCK_OK "|  Taglio  |"
#define TRS_MAX_MOW "Massimo"
#define TRS_SELECTE "Selez."
#define TRS_1HR_MOW_SELECTED "1 ora di taglio"
#define TRS_ENDS_ "Fine: "
#define TRS_2HR_MOW_SELECTED "2 ore di taglio"
#define TRS_MOW_GRASS__ "Taglia erba"
#define TRS_GO_TO_DOCK "Torna nel box"
#define TRS_DOCK_REMOVED "Box rimosso"
#define TRS_MOW_RE_STARTING "Riavvia taglio"
#define TRS_RETURNING_HOME "Torna nel box"
#define TRS_NO_DOCK_ACTIVE "Nessun box attiv"
#define TRS_CHARGING "In carica"
#define TRS_RECHARG_BAT "Ricarica Batt"
#define TRS_FIND_WIRE "Cerco cavo.."
#define TRS_START_CUT "Avvia"
#define TRS_START_CUT_NO_WIRE "Avvia(NO CAVO)"
#define TRS_NO_WIRE "CAVO OFF"
#define TRS_WIRE "CAVO"
#define TRS_ROBOT_LOCK "ROBOT BLOCCATO"
#define TRS_GO_HOME "Vai in base"

#endif
