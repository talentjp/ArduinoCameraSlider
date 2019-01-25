#include <LiquidCrystal.h>
#include "LcdKeypad.h"
#include "MenuData.h"
#include <TimerOne.h>

enum AppModeValues
{
  APP_NORMAL_MODE,
  APP_MENU_MODE,
  APP_PROCESS_MENU_CMD
};

enum class TrackDirection{
  RIGHT,
  LEFT 
};

enum class CircleDirection{
  CLOCKWISE,
  CNT_CLOCKWISE
};

const float rotEpsilon = 0.5f;
int speedT = 0;
int speedC = 0;
TrackDirection directionT = TrackDirection::RIGHT;
CircleDirection directionC = CircleDirection::CLOCKWISE;
int trackLength = 0;
float degreeRot = 0;

byte appMode = APP_NORMAL_MODE;

MenuManager Menu1(sampleMenu_Root, menuCount(sampleMenu_Root));

char strbuf[LCD_COLS + 1]; // one line of lcd display
byte btn;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void refreshMenuDisplay (byte refreshMode);
byte getNavAction();

void PrintInt(int val){
  itoa(val, strbuf, 10);
  size_t len = strlen(strbuf);
  while(len < LCD_COLS - 2){  //2 reserved for prompt
    strbuf[len] = ' ';          
    len++;
  }
  strbuf[len] = '\0';
  lcd.setCursor(2, 1);      
  lcd.print(strbuf);
}

void PrintFloat(float val){
  dtostrf(val, 8, 2, strbuf);
  //Remove padded space in the beginning
  size_t idx = 0;
  while(idx < LCD_COLS - 1 && strbuf[idx] == ' '){
    idx++;
  }
  size_t offset = idx;
  while(idx < LCD_COLS - 1){
    strbuf[idx - offset] = strbuf[idx];
    idx++;    
  }  
  size_t len = strlen(strbuf);
  while(len < LCD_COLS - 2){  //2 reserved for prompt
    strbuf[len] = ' ';          
    len++;
  }
  strbuf[len] = '\0';
  lcd.setCursor(2, 1);      
  lcd.print(strbuf);      
}

void IncrementInt(int* var){
  if(btn == BUTTON_UP_PRESSED){
    (*var)++;
  }
  else if(btn == BUTTON_DOWN_PRESSED){
    (*var) = max(0, (*var)-1);
  }
}

void IncrementAngle(float* angle){
  if(btn == BUTTON_UP_PRESSED){  
    *angle += rotEpsilon;
  }
  else if(btn == BUTTON_DOWN_PRESSED){
    *angle -= rotEpsilon;
  }
  if(*angle >= 360){
    *angle -= 360;
  }
  else if(*angle < 0){
    *angle += 360;
  }
}

void ToggleDirection(TrackDirection* dir){
  if(btn == BUTTON_UP_PRESSED || btn == BUTTON_DOWN_PRESSED){
    if(*dir == TrackDirection::RIGHT){
      *dir = TrackDirection::LEFT;
    }
    else{
      *dir = TrackDirection::RIGHT;
    }
  }
}

void ToggleDirection(CircleDirection* dir){
  if(btn == BUTTON_UP_PRESSED || btn == BUTTON_DOWN_PRESSED){
    if(*dir == CircleDirection::CLOCKWISE){
      *dir = CircleDirection::CNT_CLOCKWISE;
    }
    else{
      *dir = CircleDirection::CLOCKWISE;
    }
  }
}

void ResetAllParams(){
  speedT = 0;
  speedC = 0;
  directionT = TrackDirection::RIGHT;
  directionC = CircleDirection::CLOCKWISE;
  trackLength = 0;
  degreeRot = 0;
}

void PrintDirection(TrackDirection dir){
  if(dir == TrackDirection::RIGHT){
    strcpy(strbuf, "RIGHT");
  }
  else{
    strcpy(strbuf, "LEFT ");
  }
  lcd.setCursor(2, 1);      
  lcd.print(strbuf);
}

void PrintDirection(CircleDirection dir){
  if(dir == CircleDirection::CLOCKWISE){
    strcpy(strbuf,"CLOCKWISE   ");
  }
  else{
    strcpy(strbuf,"CNTCLOCKWISE");
  }
  lcd.setCursor(2, 1);      
  lcd.print(strbuf);  
}

void PrintConfirm(){
  lcd.setCursor(2, 1);
  lcd.print("Sure?");
}

void setup()
{
  //Serial.begin(9600);

  backLightOn();
  // set up the LCD's number of columns and rows:
  lcd.begin(LCD_COLS, LCD_ROWS);

  // fall in to menu mode by default.
  appMode = APP_MENU_MODE;
  refreshMenuDisplay(REFRESH_DESCEND);

  // Use soft PWM for backlight, as hardware PWM must be avoided for some LCD shields.
  Timer1.initialize();
  Timer1.attachInterrupt(lcdBacklightISR, 500);
  setBacklightBrightness(1);
}


void loop()
{ 
  btn = getButton();

  switch (appMode)
  {
    case APP_NORMAL_MODE :
      if (btn == BUTTON_UP_LONG_PRESSED)
      {
        appMode = APP_MENU_MODE;
        refreshMenuDisplay(REFRESH_DESCEND);
      }
      break;
    case APP_MENU_MODE :
    {
      byte menuMode = Menu1.handleNavigation(getNavAction, refreshMenuDisplay);
      if (menuMode == MENU_EXIT)
      {
        lcd.clear();
        lcd.print("Hold UP for menu");
        appMode = APP_NORMAL_MODE;
      }
      else if (menuMode == MENU_INVOKE_ITEM)
      {
        appMode = APP_PROCESS_MENU_CMD;

        // Indicate selected item.
        if (Menu1.getCurrentItemCmdId())
        {
          lcd.setCursor(0, 1);
          strbuf[0] = 0b01111110; // forward arrow representing input prompt.
          strbuf[1] = 0;
          lcd.print(strbuf);
        }
      }
      break;
    }
    case APP_PROCESS_MENU_CMD : //only jump out of this command if it finishes processing
    {
      byte processingComplete = processMenuCommand(Menu1.getCurrentItemCmdId());       
      if (processingComplete)
      {
        appMode = APP_MENU_MODE;
        // clear forward arrow
        lcd.setCursor(0, 1);
        strbuf[0] = ' '; // clear forward arrow
        strbuf[1] = 0;
        lcd.print(strbuf);
        if(Menu1.getCurrentItemCmdId() == mnuCmdReset){
          lcd.setCursor(2, 1);
          lcd.print("Params reset!");
        }
      }
      break;
    }
  }
}

//----------------------------------------------------------------------
// Addition or removal of menu items in MenuData.h will require this method
// to be modified accordingly. 
byte processMenuCommand(byte cmdId)
{
  byte complete = false;  // set to true when menu command processing complete.

  if (btn == BUTTON_SELECT_PRESSED)
  {
    complete = true;
  }

  switch (cmdId)
  {
    // TODO Process menu commands here:       
    case mnuCmdSpeedT:      
      PrintInt(speedT);
      IncrementInt(&speedT);     
      break; 
    case mnuCmdSpeedC:
      PrintInt(speedC);
      IncrementInt(&speedC);  
      break;
    case mnuCmdTrackLength:
      PrintInt(trackLength);
      IncrementInt(&trackLength);
      break;
    case mnuCmdDegreeRotation:
      PrintFloat(degreeRot);
      IncrementAngle(&degreeRot);    
      break;  
    case mnuCmdDirectionT:
      PrintDirection(directionT);
      ToggleDirection(&directionT);
      break;
    case mnuCmdDirectionC:
      PrintDirection(directionC);
      ToggleDirection(&directionC);
      break;
    case mnuCmdReset:
      PrintConfirm();
      ResetAllParams();
      break;
      default:
      break;
  }

  return complete;
}


//----------------------------------------------------------------------
// Callback to convert button press to navigation action.
byte getNavAction()
{
  byte navAction = 0;
  byte currentItemHasChildren = Menu1.currentItemHasChildren();
  
  if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED) navAction = MENU_ITEM_PREV;
  else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED) navAction = MENU_ITEM_NEXT;
  else if (btn == BUTTON_SELECT_PRESSED || (btn == BUTTON_RIGHT_PRESSED && currentItemHasChildren)) navAction = MENU_ITEM_SELECT;
  //else if (btn == BUTTON_LEFT_PRESSED) navAction = MENU_BACK;
  return navAction;
}


//----------------------------------------------------------------------
const char EmptyStr[] = "";

// Callback to refresh display during menu navigation, using parameter of type enum DisplayRefreshMode.
void refreshMenuDisplay (byte refreshMode)
{
  char nameBuf[LCD_COLS+1];
  lcd.setCursor(0, 0);
  if (Menu1.currentItemHasChildren())
  {
    rpad(strbuf, Menu1.getCurrentItemName(nameBuf));
    strbuf[LCD_COLS-1] = 0b01111110;            // Display forward arrow if this menu item has children.
    lcd.print(strbuf);
    lcd.setCursor(0, 1);
    lcd.print(rpad(strbuf, EmptyStr));          // Clear config value in display
  }
  else
  {
    byte cmdId;
    rpad(strbuf, Menu1.getCurrentItemName(nameBuf));
    
    if ((cmdId = Menu1.getCurrentItemCmdId()) == 0)
    {
      strbuf[LCD_COLS-1] = 0b01111111;          // Display back arrow if this menu item ascends to parent.
      lcd.print(strbuf);
      lcd.setCursor(0, 1);
      lcd.print(rpad(strbuf, EmptyStr));        // Clear config value in display.
    }
    else
    {
      lcd.print(strbuf);
      lcd.setCursor(0, 1);
      lcd.print("                ");   //Clear the entire row
      
      // TODO Display config value.
    }
  }
}
