enum Callbacks { START_CUT,
                 START_CUT_NO_WIRE,
                 MOW_THE_LINE,
                 TEST_MENU,
                 WIRE_TEST,
                 RELAY_TEST,
                 WHEEL_TEST,
                 BLADE_TEST,
                 SONAR_TEST,
                 TURN_TEST,
                 VOLT_AMP_TEST,
                 COMPASS_TEST,
                 GO_HOME };

//Membrane Key Variables
static bool keyOKPressed;
static bool keyUpPressed;
static bool keyDownPressed;
static bool keyBackPressed;
void readMembraneKeys();
void openMenu(const char *menuItems[],const Callbacks dockedMenuCallbacks[],const char leaveCount);
void processCallback(Callbacks callback);
void checkMembraneSwitchInputDocked();
void processMenuDockedChoice();
void checkMembraneSwitchInputParked();
void checkMembranSwitchRunning();
void openTestsMenu();
void Run_Menu_Order_Testing();
void Activate_Menu_Option_Testing();
