void Print_Test_Wheel() {  

    tft.fillScreen(BLACK);


// Create Bars for the Motor Values

  int Bar_To_Button_Y = 25;                      // Distance between the bar and the button
  Button_W = 90;
  int Button_H = 40;
  int Button_Spacing = 5;
  int Txt_Size_Main_Menu = 2;
  LH_RH_Spacing = 340;
  
  int Txt_Size_Label = 2;                       // Text size of the battery % text

    // Main Menu Title
    tft.setTextSize(Txt_Size_Main_Menu); 
    tft.setTextColor(GREEN, BLACK);      //Text Colour/ Background Colour
    tft.setCursor(20, 10);            // Text Coordinates X, Y
    if (Robot_Type ==  1) tft.print(F("Wheel Test Mower"));
    if (Robot_Type ==  2) tft.print(F("Wheel Test Aerator"));


  // LEFT FRONT
  int Button_X_Pos = 60;
  int Button_Y_Pos = 100;
  tft.setTextSize(2); 
  tft.setTextColor(GREEN, BLACK);  
  tft.setCursor(Button_X_Pos - 20, (Button_Y_Pos - 45) );            // Text Coordinates X, Y
  tft.print(F("LH Frnt Wheel"));  
  UP1_BTN.initButton  (&tft, Button_X_Pos, Button_Y_Pos  , Button_W, Button_H, WHITE, CYAN, BLACK, "FWD", 2);
  UP1_BTN.drawButton(false);
  UP2_BTN.initButton  (&tft, (Button_X_Pos + Button_W + Button_Spacing), Button_Y_Pos  , Button_W, Button_H, WHITE, CYAN, BLACK, "REV", 2);
  UP2_BTN.drawButton(false);


  // RIGHT FRONT
  Button_X_Pos = Button_X_Pos + Button_W + Button_Spacing + 150;
  tft.setTextSize(2); 
  tft.setTextColor(GREEN, BLACK);  
  tft.setCursor(Button_X_Pos - 20, (Button_Y_Pos - 45) );            // Text Coordinates X, Y
  tft.print(F("RH Frnt Wheel"));  
  UP3_BTN.initButton  (&tft, Button_X_Pos, Button_Y_Pos  , Button_W, Button_H, WHITE, CYAN, BLACK, "FWD", 2);
  UP3_BTN.drawButton(false);
  UP4_BTN.initButton  (&tft, (Button_X_Pos + Button_W + Button_Spacing), Button_Y_Pos  , Button_W, Button_H, WHITE, CYAN, BLACK, "REV", 2);
  UP4_BTN.drawButton(false);


  // LEFT REAR
  Button_X_Pos = 60;
  Button_Y_Pos = 200;
  tft.setTextSize(2); 
  tft.setTextColor(GREEN, BLACK);  
  tft.setCursor(Button_X_Pos - 20, (Button_Y_Pos - 45) );            // Text Coordinates X, Y
  tft.print(F("LH Rear Wheel"));   
  DOWN1_BTN.initButton(&tft, Button_X_Pos, Button_Y_Pos, Button_W, Button_H, WHITE, CYAN, BLACK, "FWD", 2);
  DOWN1_BTN.drawButton(false);
  DOWN2_BTN.initButton(&tft, (Button_X_Pos + Button_W + Button_Spacing), Button_Y_Pos, Button_W, Button_H, WHITE, CYAN, BLACK, "REV", 2);
  DOWN2_BTN.drawButton(false);


  // RIGHT REAR
  Button_X_Pos = Button_X_Pos + Button_W + Button_Spacing + 150;
  tft.setTextSize(2); 
  tft.setTextColor(GREEN, BLACK);  
  tft.setCursor(Button_X_Pos - 20, (Button_Y_Pos - 45) );            // Text Coordinates X, Y
  tft.print(F("RH Rear Wheel"));  
  DOWN3_BTN.initButton  (&tft, Button_X_Pos, Button_Y_Pos  , Button_W, Button_H, WHITE, CYAN, BLACK, "FWD", 2);
  DOWN3_BTN.drawButton(false);
  DOWN4_BTN.initButton  (&tft, (Button_X_Pos + Button_W + Button_Spacing), Button_Y_Pos  , Button_W, Button_H, WHITE, CYAN, BLACK, "REV", 2);
  DOWN4_BTN.drawButton(false);

  
 
  
// Exit Button

    Done6_btn.initButton(&tft, 425, 278, 80, 40, WHITE, CYAN, BLACK, "Exit", 2);
    Done6_btn.drawButton(false);
  
  } 




void React_to_Button_Wheel_Test() {

    
    // Actions if LH Frnt FWD is pressed
    if (UP1_BTN.justPressed()) {
         tft.setTextSize(2); 
         tft.setTextColor(YELLOW, BLACK);  
         tft.setCursor(50, 250 );            // Text Coordinates X, Y
         tft.print(F("LH Frnt Wheel FWD"));  
         Menu_Active = 66;
         Send_Menu_Selected_To_Mower_MEGA_No_RX();
         delay(2000);
         tft.setTextSize(2); 
         tft.setTextColor(BLACK, BLACK);  
         tft.setCursor(50, 250 );            // Text Coordinates X, Y
         tft.print(F("LH Frnt Wheel FWD"));  
         }

    // Actions if LH Frnt REV is pressed
    if (UP2_BTN.justPressed()) {
         tft.setTextSize(2); 
         tft.setTextColor(YELLOW, BLACK);  
         tft.setCursor(50, 250 );            // Text Coordinates X, Y
         tft.print(F("LH Frnt Wheel REV"));  
         Menu_Active = 67;
         Send_Menu_Selected_To_Mower_MEGA_No_RX();
         delay(2000);
         tft.setTextSize(2); 
         tft.setTextColor(BLACK, BLACK);  
         tft.setCursor(50, 250 );            // Text Coordinates X, Y
         tft.print(F("LH Frnt Wheel REV"));  
         }
            
    // Actions if RH Frnt FWD is pressed
    if (UP3_BTN.justPressed()) {
         tft.setTextSize(2); 
         tft.setTextColor(YELLOW, BLACK);  
         tft.setCursor(50, 250 );            // Text Coordinates X, Y
         tft.print(F("RH Frnt Wheel FWD"));  
         Menu_Active = 68;
         Send_Menu_Selected_To_Mower_MEGA_No_RX();
         delay(2000);
         tft.setTextSize(2); 
         tft.setTextColor(BLACK, BLACK);  
         tft.setCursor(50, 250 );            // Text Coordinates X, Y
         tft.print(F("RH Frnt Wheel FWD"));  
         }

    // Actions if RH Frnt REV is pressed
    if (UP4_BTN.justPressed()) {
         tft.setTextSize(2); 
         tft.setTextColor(YELLOW, BLACK);  
         tft.setCursor(50, 250 );            // Text Coordinates X, Y
         tft.print(F("RH Frnt Wheel REV"));  
         Menu_Active = 69;
         Send_Menu_Selected_To_Mower_MEGA_No_RX();
         delay(2000);
         tft.setTextSize(2); 
         tft.setTextColor(BLACK, BLACK);  
         tft.setCursor(50, 250 );            // Text Coordinates X, Y
         tft.print(F("RH Frnt Wheel REV"));  
         }

    // Actions if LH Rear FWD is pressed
    if (DOWN1_BTN.justPressed()) {
         tft.setTextSize(2); 
         tft.setTextColor(YELLOW, BLACK);  
         tft.setCursor(50, 250 );            // Text Coordinates X, Y
         tft.print(F("LH Rear Wheel FWD"));  
         Menu_Active = 70;
         Send_Menu_Selected_To_Mower_MEGA_No_RX();
         delay(2000);
         tft.setTextSize(2); 
         tft.setTextColor(BLACK, BLACK);  
         tft.setCursor(50, 250 );            // Text Coordinates X, Y
         tft.print(F("LH Rear Wheel FWD"));  
         }
    
    // Actions if LH Rear Rev is pressed
    if (DOWN2_BTN.justPressed()) {
         tft.setTextSize(2); 
         tft.setTextColor(YELLOW, BLACK);  
         tft.setCursor(50, 250 );            // Text Coordinates X, Y
         tft.print(F("LH Rear Wheel REV"));  
         Menu_Active = 71;
         Send_Menu_Selected_To_Mower_MEGA_No_RX();
         delay(2000);
         tft.setTextSize(2); 
         tft.setTextColor(BLACK, BLACK);  
         tft.setCursor(50, 250 );            // Text Coordinates X, Y
         tft.print(F("LH Rear Wheel REV"));  
         }

    // Actions if RH Rear FWD is pressed
    if (DOWN3_BTN.justPressed()) {
         tft.setTextSize(2); 
         tft.setTextColor(YELLOW, BLACK);  
         tft.setCursor(50, 250 );            // Text Coordinates X, Y
         tft.print(F("RH Rear Wheel FWD"));  
         Menu_Active = 72;
         Send_Menu_Selected_To_Mower_MEGA_No_RX();
         delay(2000);
         tft.setTextSize(2); 
         tft.setTextColor(BLACK, BLACK);  
         tft.setCursor(50, 250 );            // Text Coordinates X, Y
         tft.print(F("RH Rear Wheel FWD"));  
         }

    
    // Actions if RH Rear Rev is pressed
    if (DOWN4_BTN.justPressed()) {
         tft.setTextSize(2); 
         tft.setTextColor(YELLOW, BLACK);  
         tft.setCursor(50, 250 );            // Text Coordinates X, Y
         tft.print(F("RH Rear Wheel REV"));  
         Menu_Active = 73;
         Send_Menu_Selected_To_Mower_MEGA_No_RX();
         delay(2000);
         tft.setTextSize(2); 
         tft.setTextColor(BLACK, BLACK);  
         tft.setCursor(50, 250 );            // Text Coordinates X, Y
         tft.print(F("RH Rear Wheel REV"));  
         }
           


if (Done6_btn.justPressed() ) {
         Menu_Complete_Wheel_Test = true;
         tft.fillScreen(BLACK);
         Serial.println(F("Wheel Test Exit"));
         Print_Tests_Menu();       
          
 }
 

 
}





void Sense_Button_Wheel_Test() {
    down = Touch_getXY();
    
    UP1_BTN.press                 (down && UP1_BTN.contains(pixel_x, pixel_y));
    UP2_BTN.press                 (down && UP2_BTN.contains(pixel_x, pixel_y));
    UP3_BTN.press                 (down && UP3_BTN.contains(pixel_x, pixel_y));
    UP4_BTN.press                 (down && UP4_BTN.contains(pixel_x, pixel_y));
    DOWN1_BTN.press               (down && DOWN1_BTN.contains(pixel_x, pixel_y));
    DOWN2_BTN.press               (down && DOWN2_BTN.contains(pixel_x, pixel_y));
    DOWN3_BTN.press               (down && DOWN3_BTN.contains(pixel_x, pixel_y));
    DOWN4_BTN.press               (down && DOWN4_BTN.contains(pixel_x, pixel_y));
    Done6_btn.press               (down && Done6_btn.contains(pixel_x, pixel_y));

    if (UP1_BTN.justReleased())               UP1_BTN.drawButton();
    if (UP2_BTN.justReleased())               UP2_BTN.drawButton();
    if (UP3_BTN.justReleased())               UP3_BTN.drawButton();
    if (UP4_BTN.justReleased())               UP4_BTN.drawButton();
    if (DOWN1_BTN.justReleased())             DOWN1_BTN.drawButton();
    if (DOWN2_BTN.justReleased())             DOWN2_BTN.drawButton();
    if (DOWN3_BTN.justReleased())             DOWN3_BTN.drawButton();
    if (DOWN4_BTN.justReleased())             DOWN4_BTN.drawButton();
    if (Done6_btn.justReleased())             Done6_btn.drawButton();
}
