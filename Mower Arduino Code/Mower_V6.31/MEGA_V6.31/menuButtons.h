//Membrane Key Variables
char Start_Key_X;
char Plus_Key_X;
char Minus_Key_X;
char Stop_Key_X;
bool Menu_Complete;
char Menu_Mode_Selection;
int Menu_View;
void Read_Membrane_Keys();
void Print_LCD_Menu_Docked(char LCD_Menu_Docked);
void checkMembraneSwitchInputDocked();
void Run_Menu_Order_Docked();
void Activate_Menu_Option_Docked();
void Print_LCD_Menu_Parked(char LCD_Menu_Parked);
void checkMembraneSwitchInputParked();
void Run_Menu_Order_Parked();
void Activate_Menu_Option_Parked();
void Check_Membrane_Keys_Running();
void Print_Membrane_Switch_Input_Timing();
void Run_Menu_Order_Timing();
void Activate_Menu_Option_Timing();
void Print_LCD_Menu_Settings(char LCD_Menu_Settings);
void Print_Membrane_Switch_Input_Settings();
void Run_Menu_Order_Settings();
void Activate_Menu_Option_Settings();
void Print_LCD_Menu_Tests(char LCD_Menu_Tests);
void Print_Membrane_Switch_Input_Tests();
void Run_Menu_Order_Testing();
void Activate_Menu_Option_Testing();