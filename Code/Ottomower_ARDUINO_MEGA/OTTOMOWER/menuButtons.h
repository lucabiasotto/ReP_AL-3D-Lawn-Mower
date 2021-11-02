//Membrane Key Variables
static bool keyOKPressed;
static bool keyUpPressed;
static bool keyDownPressed;
static bool keyBackPressed;
static bool menuInteractionsCompleted; //if true Interacion with menu is done
static char menuModeSelection;
static char menuCursorPosition;
void readMembraneKeys();
void checkMembraneSwitchInputDocked();
void processMenuDockedChoice();
void checkMembraneSwitchInputParked();
void checkMembranSwitchRunning();
void Print_LCD_Menu_Tests(char laeveIndex);
void Print_Membrane_Switch_Input_Tests();
void Run_Menu_Order_Testing();
void Activate_Menu_Option_Testing();
