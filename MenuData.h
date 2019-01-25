#ifndef _sampleMenu_
#define _sampleMenu_
#include "MenuManager.h"
#include <avr/pgmspace.h>

/*
Generated using LCD Menu Builder at http://lcd-menu-bulder.cohesivecomputing.co.uk/.
*/

enum sampleMenuCommandId
{
  mnuCmdBack = 0,
  mnuCmdTrackMode,
  mnuCmdSpeedT,
  mnuCmdDirectionT,
  mnuCmdTrackLength,
  mnuCmdCircleMode,
  mnuCmdSpeedC,
  mnuCmdDirectionC,
  mnuCmdDegreeRotation,
  mnuCmdReset
};

PROGMEM const char sampleMenu_back[] = "Back";
PROGMEM const char sampleMenu_exit[] = "Exit";

PROGMEM const char sampleMenu_1_1[] = "1 Speed";
PROGMEM const char sampleMenu_1_2[] = "2 Direction";
PROGMEM const char sampleMenu_1_3[] = "3 Track length";
PROGMEM const MenuItem sampleMenu_List_1[] = {{mnuCmdSpeedT, sampleMenu_1_1}, {mnuCmdDirectionT, sampleMenu_1_2}, {mnuCmdTrackLength, sampleMenu_1_3}, {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_2_1[] = "1 Speed";
PROGMEM const char sampleMenu_2_2[] = "2 Direction";
PROGMEM const char sampleMenu_2_3[] = "3 Track length";
PROGMEM const MenuItem sampleMenu_List_2[] = {{mnuCmdSpeedC, sampleMenu_2_1}, {mnuCmdDirectionC, sampleMenu_2_2}, {mnuCmdDegreeRotation, sampleMenu_2_3}, {mnuCmdBack, sampleMenu_back}};

PROGMEM const char sampleMenu_1[] = "1 Track Mode";
PROGMEM const char sampleMenu_2[] = "2 Circle Mode";
PROGMEM const char sampleMenu_3[] = "3 Reset";
PROGMEM const MenuItem sampleMenu_Root[] = {{mnuCmdTrackMode, sampleMenu_1, sampleMenu_List_1, menuCount(sampleMenu_List_1)}, {mnuCmdCircleMode, sampleMenu_2, sampleMenu_List_2, menuCount(sampleMenu_List_2)}, {mnuCmdReset, sampleMenu_3}, {mnuCmdBack, sampleMenu_exit}};

/*
case mnuCmdSpeedT :
  break;
case mnuCmdDirectionT :
  break;
case mnuCmdTrackLength :
  break;
case mnuCmdSpeedC :
  break;
case mnuCmdDirectionC :
  break;
case mnuCmdDegreeRotation :
  break;
case mnuCmdReset :
  break;
*/

/*
<?xml version="1.0"?>
<RootMenu xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema">
  <Config IdPrefix="mnuCmd" VarPrefix="sampleMenu" UseNumbering="true" IncludeNumberHierarchy="false"
          MaxNameLen="15" MenuBackFirstItem="false" BackText="Back" ExitText="Exit" AvrProgMem="true"/>
  <MenuItems>
    <Item Id="TrackMode" Name="Track Mode">
    <MenuItems>
    <Item Id="SpeedT" Name="Speed"/>
    <Item Id="DirectionT" Name="Direction"/>
    <Item Id="TrackLength" Name="Track length"/> 
    </MenuItems>
    </Item>
    <Item Id="CircleMode" Name="Circle Mode">
    <MenuItems>
    <Item Id="SpeedC" Name="Speed"/>
    <Item Id="DirectionC" Name="Direction"/>
    <Item Id="DegreeRotation" Name="Track length"/>
    </MenuItems>
    </Item>
    <Item Id="Reset" Name="Reset"/>
  </MenuItems>
</RootMenu>
*/
#endif
