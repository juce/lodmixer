// win32gui.h

#ifndef _JUCE_WIN32GUI
#define _JUCE_WIN32GUI

#include <windows.h>

#define WIN_WIDTH 615 
#define WIN_HEIGHT 520

extern HWND g_exeNameControl;             // displays name of the executable
extern HWND g_findMoreButtonControl;      // finds more games
extern HWND g_gameInfoControl;            // displays key binding for home kit switch
extern HWND g_lodListControl[5];          // lod lists
extern HWND g_crowdCheckBox;              // crowd

extern HWND g_weatherListControl;         // weather (default,fine,rainy,random)
extern HWND g_timeListControl;            // time of the day (default,day,night)
extern HWND g_seasonListControl;          // season (default,summer,winter)
extern HWND g_stadiumListControl;         // stadium (default,0-35)
extern HWND g_stadiumEffectsListControl;  // stadium effects (default,0/1)
extern HWND g_homeCrowdListControl;       // home crowd attendance (default,0-3)
extern HWND g_awayCrowdListControl;       // away crowd attendance (default,0-3)
extern HWND g_crowdStanceListControl;      // crowd stance (default,1-3)

extern HWND g_statusTextControl;          // displays status messages
extern HWND g_saveButtonControl;          // save settings button
extern HWND g_defButtonControl;          // default settings button

// functions
bool BuildControls(HWND parent);

#endif
