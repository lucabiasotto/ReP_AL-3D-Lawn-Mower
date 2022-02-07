void Print_Test_Menu_PIXHAWK() {  

  int Txt_Size_Main_Menu = 2;
  int Txt_Size_Label = 2;                       // Text size of the battery % text


    tft.fillScreen(BLACK);
    // Main Menu Title
    tft.setTextSize(Txt_Size_Main_Menu); 
    tft.setTextColor(GREEN, BLACK);      //Text Colour/ Background Colour
    tft.setCursor(20, 10);            // Text Coordinates X, Y
    tft.print(F("PIXHAWK Test"));  




  Button_X1 = 120;
  Button_Y1 = 80;
  int Button_H = 50;
  int Button_Spacing = 10;
  int Button_W = 200;

  UP1_BTN.initButton  (&tft, Button_X1, Button_Y1  , Button_W, Button_H, WHITE, CYAN, BLACK, "PWM Test", 2);
  UP1_BTN.drawButton(false); 


  Button_X2 = Button_X1;
  Button_Y2 = Button_X1 + Button_H + Button_Spacing; 
  
  DOWN1_BTN.initButton(&tft, Button_X2, Button_Y2, Button_W, Button_H, WHITE, CYAN, BLACK, "Spare 2", 2);  
  DOWN1_BTN.drawButton(false);


  Button_X3 = Button_X1;
  
  //Arm PIXHAWK Mission

   OPTION1_BTN.initButton(&tft, Button_X3, 250, 200, 60, WHITE, GREEN, BLACK, "ARM", 2);
   OPTION1_BTN.drawButton(false);  
 
  
// Exit Button

    Done6_btn.initButton(&tft, 425, 278, 80, 40, WHITE, CYAN, BLACK, "Exit", 2);
    Done6_btn.drawButton(false);
  
  } 




void React_to_Button_Press_PIXHAWK_Test() {

    
    // Actions if UP is pressed
    if (UP1_BTN.justPressed()) {

        Serial.println(F("PIXHAWK Motor Test"));
        int Button_H = 50;
        int Button_Spacing = 10;
        int Button_W = 200;

        int Arrow_X = 280;

        UP1_BTN.initButton  (&tft, Button_X1, Button_Y1  , Button_W, Button_H, WHITE, GREEN, BLACK, "..Running..", 2);
        UP1_BTN.drawButton(false); 
         
        int cancel = 0;
        cycles = 0;
         
         Menu_Active = 64;
         Send_Menu_Selected_To_Mower_MEGA_No_RX();
         
        while (cycles < 499) {
          Receive_PIXHAWK_Running_Data();
          delay(100);


          tft.setTextSize(3); 
          tft.setTextColor(GREEN, BLACK);      //Text Colour/ Background Colour
          tft.setCursor((Arrow_X + 75), 50);            // Text Coordinates X, Y
          tft.print(500 - cycles);          
          
          if (PWM_Arduino_LH > 150) {
              tft.setTextSize(10); 
              tft.setTextColor(GREEN, BLACK);      //Text Colour/ Background Colour
              tft.setCursor(Arrow_X, 100);            // Text Coordinates X, Y
              tft.print(F("<"));        
              }
          if (PWM_Arduino_LH < 150) {
              tft.setTextSize(10); 
              tft.setTextColor(BLACK, BLACK);      //Text Colour/ Background Colour
              tft.setCursor(Arrow_X, 100);            // Text Coordinates X, Y
              tft.print(F("<"));        
              }
          if (PWM_Arduino_RH > 150) {
              tft.setTextSize(10); 
              tft.setTextColor(GREEN, BLACK);      //Text Colour/ Background Colour
              tft.setCursor((Arrow_X + 75), 100);            // Text Coordinates X, Y
              tft.print(F(">"));        
              }
          if (PWM_Arduino_RH < 150) {
              tft.setTextSize(10); 
              tft.setTextColor(BLACK, BLACK);      //Text Colour/ Background Colour
              tft.setCursor((Arrow_X + 75), 100);            // Text Coordinates X, Y
              tft.print(F(">"));        
              }
          
          tft.setTextSize(3); 
          tft.setTextColor(BLACK, BLACK);      //Text Colour/ Background Colour
          tft.setCursor((Arrow_X + 75), 50);            // Text Coordinates X, Y
          tft.print(500 - cycles);    
          
          }
        Serial.println(F("Test Finised"));
        UP1_BTN.initButton  (&tft, Button_X1, Button_Y1  , Button_W, Button_H, WHITE, CYAN, BLACK, "PWM Test", 2);
        UP1_BTN.drawButton(false); 
        }


    // Action if down is pressed
    if (DOWN1_BTN.justPressed()) {
         Menu_Active = 64;
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





void Sense_Button_Press_PIXHAWK_Test() {
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
