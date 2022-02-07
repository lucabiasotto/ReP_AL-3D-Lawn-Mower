void Print_Test_Menu_Lift() {  

    tft.fillScreen(BLACK);


// Create Bars for the Motor Values

  // Global Variable = Battery                  // Battery % remaining
  Bar_Width = 50;                               // Width of the battery bar
  Bar_Height = 120;                             // Height of the battery bar
  int Bar_To_Button_Y = 25;                      // Distance between the bar and the button
  Button_W = 50;
  int Button_H = 40;
  int Button_Spacing = 5;
  int Txt_Size_Main_Menu = 2;
  LH_RH_Spacing = 340;
  
  Bar_Pos_X = 50;                               // X Coordinate of the Battery Symbol
  Bar_Pos_Y = 110;                               // Y Coordinate of the Battery Symbol
  int Txt_Size_Label = 2;                       // Text size of the battery % text

    // Main Menu Title
    tft.setTextSize(Txt_Size_Main_Menu); 
    tft.setTextColor(GREEN, BLACK);      //Text Colour/ Background Colour
    tft.setCursor(20, 10);            // Text Coordinates X, Y
    if (Robot_Type ==  2) tft.print(F("Lift Mechanism Test"));

  // Mower Motor Bar Graphic  
  tft.fillRect((Bar_Pos_X - 1), (Bar_Pos_Y - 1) , (Bar_Width + 2 ), (Bar_Height + 2) , WHITE);    // Create a white Box (Border)
  tft.fillRect(Bar_Pos_X, (Bar_Pos_Y), (Bar_Width), (Bar_Height) , GREY);      // Create a grey background
 
  Calculate_Gauge_Level_Blade();  // Calulate the position of the red bar line
  if (PWM_Blade_Speed >= 250) tft.fillRect( (Bar_Pos_X + 2), (Bar_Pos_Y + Bar_Height), (Bar_Width - 4), (-1 * Gauge_Level_Blade) , RED);  // draws a new red bar
  if (PWM_Blade_Speed < 250) tft.fillRect( (Bar_Pos_X + 2), (Bar_Pos_Y + Bar_Height), (Bar_Width - 4), (-1 * Gauge_Level_Blade) , GREEN);  // draws a new red bar

  int Button_X_Pos = Bar_Pos_X + (0.5 * Button_W);

  UP1_BTN.initButton  (&tft, Button_X_Pos, (Bar_Pos_Y - Bar_To_Button_Y)  , Button_W, Button_H, WHITE, CYAN, BLACK, "+", 2);
  DOWN1_BTN.initButton(&tft, Button_X_Pos, (Bar_Pos_Y + Bar_Height + Bar_To_Button_Y), Button_W, Button_H, WHITE, CYAN, BLACK, "-", 2);
  UP1_BTN.drawButton(false);
  DOWN1_BTN.drawButton(false);

  tft.setTextSize(2); 
  tft.setTextColor(YELLOW, BLACK);      //Text Colour/ Background Colour
  tft.setCursor(Button_X_Pos - Button_W  + 10, (Bar_Pos_Y + Bar_Height + Bar_To_Button_Y + Button_H - 10) );            // Text Coordinates X, Y
  tft.print(F("Down"));
  tft.setCursor(Button_X_Pos - 20, (Bar_Pos_Y - Button_H - 30) );            // Text Coordinates X, Y
  tft.print(F("Up"));

  
  //Test Drill Cycle  

   OPTION1_BTN.initButton(&tft, 235, 278, 200, 40, WHITE, GREEN, BLACK, "D- Cycle All", 2);
   OPTION1_BTN.drawButton(false);  
 
  
// Exit Button

    Done6_btn.initButton(&tft, 425, 278, 80, 40, WHITE, CYAN, BLACK, "Exit", 2);
    Done6_btn.drawButton(false);
  
  } 




void React_to_Button_Press_Lift_Test() {

    
    // Actions if UP is pressed
    if (UP1_BTN.justPressed()) {
         Menu_Active = 60;
         Send_Menu_Selected_To_Mower_MEGA_No_RX();
         }


    // Action if down is pressed
    if (DOWN1_BTN.justPressed()) {
         Menu_Active = 61;
         Send_Menu_Selected_To_Mower_MEGA_No_RX();
         }
    

if (OPTION1_BTN.justPressed() )  {
         Menu_Active = 62;
         Send_Menu_Selected_To_Mower_MEGA_No_RX();
         }

           


if (Done6_btn.justPressed() ) {
         Menu_Complete_Lift_Menu = true;
         tft.fillScreen(BLACK);
         Serial.println(F("Battery Data Saved and TX"));
         //Menu_Active = 911;
         //Send_Menu_Selected_To_Mower_MEGA();
         Print_Testing_3_Menu();       
          
 }
 

 
}





void Sense_Button_Press_Lift_Test() {
    down = Touch_getXY();
    
    UP1_BTN.press                 (down && UP1_BTN.contains(pixel_x, pixel_y));
    DOWN1_BTN.press               (down && DOWN1_BTN.contains(pixel_x, pixel_y));
    OPTION1_BTN.press             (down && OPTION1_BTN.contains(pixel_x, pixel_y));   // Blade ON/OFF
    Done6_btn.press               (down && Done6_btn.contains(pixel_x, pixel_y));

    if (UP1_BTN.justReleased())               UP1_BTN.drawButton();
    if (DOWN1_BTN.justReleased())             DOWN1_BTN.drawButton();
    if (OPTION1_BTN.justReleased())           OPTION1_BTN.drawButton();
    if (Done6_btn.justReleased())             Done6_btn.drawButton();
}
