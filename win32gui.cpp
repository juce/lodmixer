#include <windows.h>
#include <stdio.h>

#define BUFLEN 4096

#include "win32gui.h"

HWND g_exeNameControl;             // displays name of the executable
//HWND g_findMoreButtonControl;      // finds more games
HWND g_gameInfoControl;            // displays key binding for home kit switch
HWND g_lodListControl[5];          // lod lists
HWND g_crowdCheckBox;             // crowd

HWND g_weatherListControl;         // weather (default,fine,rainy,random)
HWND g_timeListControl;            // time of the day (default,day,night)
HWND g_seasonListControl;          // season (default,summer,winter)
HWND g_stadiumListControl;         // stadium (default,0-35)
HWND g_stadiumEffectsListControl;  // stadium effects (default,0/1)
HWND g_homeCrowdListControl;       // home crowd attendance (default,0-3)
HWND g_awayCrowdListControl;       // away crowd attendance (default,0-3)
HWND g_crowdStanceListControl;      // crowd stance (default,1-3)

HWND g_statusTextControl;          // displays status messages
HWND g_saveButtonControl;        // save settings button
HWND g_defButtonControl;          // default settings button

#define LC_LABEL_WIDTH 75 
#define RC_LABEL_WIDTH 90

char* lods[] = {
    "#1: Very high-detail models, big (512x256) textures",
    "#2: High-detail models, medium (256x128) textures",
    "#3: Medium-detail models, medium (256x128) textures",
    "#4: Low-detail models, small (128x64) textures",
    "#5: Very low-detail models, small (128x64) textures",
};

/**
 * build all controls
 */
bool BuildControls(HWND parent)
{
	HGDIOBJ hObj;
	DWORD style, xstyle;
	int x, y, spacer;
	int boxW, boxH, statW, statH, borW, borH, butW, butH;

	// Get a handle to the stock font object
	hObj = GetStockObject(DEFAULT_GUI_FONT);

	spacer = 6; 
	x = spacer, y = spacer;
	butH = 22;
	butW = 60;

	// use default extended style
	xstyle = WS_EX_LEFT;
	style = WS_CHILD | WS_VISIBLE;

	// TOP SECTION

	borW = WIN_WIDTH - spacer*3;
	statW = 40;
	boxW = borW - statW - spacer*3; boxH = 22; statH = 16;
	borH = spacer*3 + boxH*2;

	HWND staticBorderTopControl = CreateWindowEx(
			xstyle, "Static", "", WS_CHILD | WS_VISIBLE | SS_ETCHEDFRAME,
			x, y, borW, borH,
			parent, NULL, NULL, NULL);

	x += spacer; y += spacer;

	HWND exeNameLabel = CreateWindowEx(
			xstyle, "Static", "EXE:", style,
			x, y+4, statW, statH, 
			parent, NULL, NULL, NULL);

	x += statW + spacer;

	char exeName[BUFLEN];
	ZeroMemory(exeName, BUFLEN);

    g_exeNameControl = CreateWindowEx(
            xstyle, "ComboBox", "", 
			CBS_DROPDOWN | WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP,
            x, y, boxW, boxH * 6,
            parent, NULL, NULL, NULL);

	x += boxW + spacer;

    /*
	g_findMoreButtonControl = CreateWindowEx(
			xstyle, "Button", "Another", style,
			x, y, butW, butH,
			parent, NULL, NULL, NULL);
            */

	butW = 60;


	x = spacer * 2;
	y += boxH + spacer;
	statW = borW - spacer*4;

	g_gameInfoControl = CreateWindowEx(
			xstyle, "Static", "EXE Info: ...", style,
			x, y+4, statW, statH, 
			parent, NULL, NULL, NULL);

	x = borW - butW;
	y += statH + spacer*3;

    // MIDDLE section: controls

    x = spacer;
    borH = boxH*6 + spacer*8;
	HWND staticBorderTopControl2 = CreateWindowEx(
			xstyle, "Static", "", WS_CHILD | WS_VISIBLE | SS_ETCHEDFRAME,
			x, y, borW, borH,
			parent, NULL, NULL, NULL);

    x = spacer*2;
    y += spacer;
    HWND label0 = CreateWindowEx(
            xstyle, "Static", "Level-Of-Detail for Player/Goalkeepers:", style,
            x, y+4, statW, statH, 
            parent, NULL, NULL, NULL);
    SendMessage(label0, WM_SETFONT, (WPARAM)hObj, true);

	x += spacer; y += spacer*2+statH;

    int i;
    char* lodLabels[] = {
        "LOD #1", "LOD #2", "LOD #3", "LOD #4", "LOD #5"
    };
    boxW = borW - 60 - spacer*3;
    for (i=0; i<5; i++) {
        x = spacer*2;
        style = WS_CHILD | WS_VISIBLE;
        HWND label = CreateWindowEx(
                xstyle, "Static", lodLabels[i], style,
                x, y+4, 60, statH, 
                parent, NULL, NULL, NULL);
        SendMessage(label, WM_SETFONT, (WPARAM)hObj, true);

        x += 60 + spacer;

        style = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL;
        g_lodListControl[i] = CreateWindowEx(
                xstyle, "ComboBox", "", style | WS_TABSTOP,
                x, y, boxW, boxH * 6,
                parent, NULL, NULL, NULL);

        for (int j=0; j<5; j++) {
            SendMessage(g_lodListControl[i], CB_ADDSTRING, (WPARAM)0, (LPARAM)lods[j]);
            SendMessage(g_lodListControl[i], WM_SETTEXT, (WPARAM)0, (LPARAM)lods[j]);
            SendMessage(g_lodListControl[i], CB_SETCURSEL, (WPARAM)j, (LPARAM)0);
        }

        y += boxH + spacer;
    }

	style = WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX;

    y += spacer*2;
    x = spacer;
	HWND staticCrowdBorderTopControl = CreateWindowEx(
			xstyle, "Static", "", WS_CHILD | WS_VISIBLE | SS_ETCHEDFRAME,
			x, y, borW, butH+spacer*2,
			parent, NULL, NULL, NULL);

    y += spacer;
    x = spacer*2;
	g_crowdCheckBox = CreateWindowEx(
			xstyle, "button", "Show crowd on all cameras", style,
			x, y, 260, butH,
			parent, NULL, NULL, NULL);

    ////////////// League/Cup/ML ////////////////////////////////////

    style = WS_CHILD | WS_VISIBLE;

    y += spacer*2 + butH;
    x = spacer;
	HWND staticWeatherBorderTopControl = CreateWindowEx(
			xstyle, "Static", "", WS_CHILD | WS_VISIBLE | SS_ETCHEDFRAME,
			x, y, borW, boxH*5+spacer*7,
			parent, NULL, NULL, NULL);

    x = spacer*2;
    y += spacer;
    HWND label = CreateWindowEx(
            xstyle, "Static", "Settings for League/Cup/Master League:", style,
            x, y+4, statW, statH, 
            parent, NULL, NULL, NULL);
    SendMessage(label, WM_SETFONT, (WPARAM)hObj, true);
    int topy = y;

    x = spacer*2;
    y += spacer*2 + statH;
    label = CreateWindowEx(
            xstyle, "Static", "Weather:", style,
            x, y+4, LC_LABEL_WIDTH, statH, 
            parent, NULL, NULL, NULL);
    SendMessage(label, WM_SETFONT, (WPARAM)hObj, true);

    x += LC_LABEL_WIDTH + spacer;
    boxW = borW/2 - LC_LABEL_WIDTH - spacer*3;
    style = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL;

	g_weatherListControl = CreateWindowEx(
			xstyle, "ComboBox", "", style | WS_TABSTOP,
			x, y, boxW , boxH * 8,
			parent, NULL, NULL, NULL);
    SendMessage(g_weatherListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Default game logic");
    SendMessage(g_weatherListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Fine");
    SendMessage(g_weatherListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Rain");
    SendMessage(g_weatherListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Snow");
    SendMessage(g_weatherListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Konami Random/Seasonal");
    SendMessage(g_weatherListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Mild Random/Seasonal");
    SendMessage(g_weatherListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Cold Random/Seasonal");

    style = WS_CHILD | WS_VISIBLE;
    x = spacer*2;
    y += spacer*2 + statH;
    label = CreateWindowEx(
            xstyle, "Static", "Time of day:", style,
            x, y+4, LC_LABEL_WIDTH, statH, 
            parent, NULL, NULL, NULL);
    SendMessage(label, WM_SETFONT, (WPARAM)hObj, true);

    x += LC_LABEL_WIDTH + spacer;
    boxW = borW/2 - LC_LABEL_WIDTH - spacer*3;
    style = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL;

	g_timeListControl = CreateWindowEx(
			xstyle, "ComboBox", "", style | WS_TABSTOP,
			x, y, boxW , boxH * 8,
			parent, NULL, NULL, NULL);
    SendMessage(g_timeListControl, WM_SETFONT, (WPARAM)hObj, true);
    SendMessage(g_timeListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Default game logic");
    SendMessage(g_timeListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Day");
    SendMessage(g_timeListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Night");
    SendMessage(g_timeListControl, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

    style = WS_CHILD | WS_VISIBLE;
    x = spacer*2;
    y += spacer*2 + statH;
    label = CreateWindowEx(
            xstyle, "Static", "Season:", style,
            x, y+4, LC_LABEL_WIDTH, statH, 
            parent, NULL, NULL, NULL);
    SendMessage(label, WM_SETFONT, (WPARAM)hObj, true);

    x += LC_LABEL_WIDTH + spacer;
    boxW = borW/2 - LC_LABEL_WIDTH - spacer*3;
    style = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL;

	g_seasonListControl = CreateWindowEx(
			xstyle, "ComboBox", "", style | WS_TABSTOP,
			x, y, boxW , boxH * 8,
			parent, NULL, NULL, NULL);
    SendMessage(g_seasonListControl, WM_SETFONT, (WPARAM)hObj, true);
    SendMessage(g_seasonListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Default game logic");
    SendMessage(g_seasonListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Summer");
    SendMessage(g_seasonListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Winter");
    SendMessage(g_seasonListControl, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

    style = WS_CHILD | WS_VISIBLE;
    x = spacer*2;
    y += spacer*2 + statH;
    label = CreateWindowEx(
            xstyle, "Static", "Effects:", style,
            x, y+4, LC_LABEL_WIDTH, statH, 
            parent, NULL, NULL, NULL);
    SendMessage(label, WM_SETFONT, (WPARAM)hObj, true);

    x += LC_LABEL_WIDTH + spacer;
    boxW = borW/2 - LC_LABEL_WIDTH - spacer*3;
    style = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL;

	g_stadiumEffectsListControl = CreateWindowEx(
			xstyle, "ComboBox", "", style | WS_TABSTOP,
			x, y, boxW , boxH * 8,
			parent, NULL, NULL, NULL);
    SendMessage(g_stadiumEffectsListControl, WM_SETFONT, (WPARAM)hObj, true);
    SendMessage(g_stadiumEffectsListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Default game logic");
    SendMessage(g_stadiumEffectsListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Off");
    SendMessage(g_stadiumEffectsListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"On");
    SendMessage(g_stadiumEffectsListControl, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

    y = topy;

    style = WS_CHILD | WS_VISIBLE;
    x = spacer*2 + borW/2;
    y += spacer*2 + statH;
    label = CreateWindowEx(
            xstyle, "Static", "Stadium:", style,
            x, y+4, RC_LABEL_WIDTH, statH, 
            parent, NULL, NULL, NULL);
    SendMessage(label, WM_SETFONT, (WPARAM)hObj, true);

    x += RC_LABEL_WIDTH + spacer;
    boxW = borW/2 - RC_LABEL_WIDTH - spacer*3;
    style = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL;

	g_stadiumListControl = CreateWindowEx(
			xstyle, "ComboBox", "", style | WS_TABSTOP,
			x, y, boxW , boxH * 8,
			parent, NULL, NULL, NULL);
    SendMessage(g_stadiumListControl, WM_SETFONT, (WPARAM)hObj, true);
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Default game logic");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(00) Club House");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(01) San Siro");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(02) Nou Camp");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(03) Highbury");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(04) Amsterdam Arena");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(05) Munich Olympiastadion");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(06) Stad Louis II");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(07) Old Trafford");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(08) Seoul WC Stadium");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(09) La Bombonera");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(10) Vodacom Park");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(11) Nacional Santiago");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(12) Delle Alpi");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(13) Olimpico");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(14) Ennio Tardini");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(15) Stamford Bridge");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(16) Anfield");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(17) Parc Des Princes");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(18) Stade Velodrome");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(19) Westfalenstadion");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(20) Rasunda");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(21) St.James's Park");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(22) De Kuip");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(23) Schuco Arena");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(24) Newlands");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(25) Berlin Olympiastadion");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(26) Riazor");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(27) Mestalla");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(28) Sapporo Dome");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(29) Kashima");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(30) Yokohama");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(31) Nagai");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(32) Olympic Tokyo");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(33) Oita Big Eye");
    SendMessage(g_stadiumListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"(34) Saitama");
    SendMessage(g_stadiumListControl, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

    style = WS_CHILD | WS_VISIBLE;
    x = spacer*2 + borW/2;
    y += spacer*2 + statH;
    label = CreateWindowEx(
            xstyle, "Static", "Home support:", style,
            x, y+4, RC_LABEL_WIDTH, statH, 
            parent, NULL, NULL, NULL);
    SendMessage(label, WM_SETFONT, (WPARAM)hObj, true);

    x += RC_LABEL_WIDTH + spacer;
    boxW = borW/2 - RC_LABEL_WIDTH - spacer*3;
    style = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL;

	g_homeCrowdListControl = CreateWindowEx(
			xstyle, "ComboBox", "", style | WS_TABSTOP,
			x, y, boxW , boxH * 8,
			parent, NULL, NULL, NULL);
    SendMessage(g_homeCrowdListControl, WM_SETFONT, (WPARAM)hObj, true);
    SendMessage(g_homeCrowdListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Default game logic");
    SendMessage(g_homeCrowdListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"0 - No crowd");
    SendMessage(g_homeCrowdListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"1 - Small crowd");
    SendMessage(g_homeCrowdListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"2 - Medium crowd");
    SendMessage(g_homeCrowdListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"3 - Large crowd");
    SendMessage(g_homeCrowdListControl, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

    style = WS_CHILD | WS_VISIBLE;
    x = spacer*2 + borW/2;
    y += spacer*2 + statH;
    label = CreateWindowEx(
            xstyle, "Static", "Away support:", style,
            x, y+4, RC_LABEL_WIDTH, statH, 
            parent, NULL, NULL, NULL);
    SendMessage(label, WM_SETFONT, (WPARAM)hObj, true);

    x += RC_LABEL_WIDTH + spacer;
    boxW = borW/2 - RC_LABEL_WIDTH - spacer*3;
    style = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL;

	g_awayCrowdListControl = CreateWindowEx(
			xstyle, "ComboBox", "", style | WS_TABSTOP,
			x, y, boxW , boxH * 8,
			parent, NULL, NULL, NULL);
    SendMessage(g_awayCrowdListControl, WM_SETFONT, (WPARAM)hObj, true);
    SendMessage(g_awayCrowdListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Default game logic");
    SendMessage(g_awayCrowdListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"0 - No crowd");
    SendMessage(g_awayCrowdListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"1 - Small crowd");
    SendMessage(g_awayCrowdListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"2 - Medium crowd");
    SendMessage(g_awayCrowdListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"3 - Large crowd");
    SendMessage(g_awayCrowdListControl, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

    style = WS_CHILD | WS_VISIBLE;
    x = spacer*2 + borW/2;
    y += spacer*2 + statH;
    label = CreateWindowEx(
            xstyle, "Static", "Crowd stance:", style,
            x, y+4, RC_LABEL_WIDTH, statH, 
            parent, NULL, NULL, NULL);
    SendMessage(label, WM_SETFONT, (WPARAM)hObj, true);

    x += RC_LABEL_WIDTH + spacer;
    boxW = borW/2 - RC_LABEL_WIDTH - spacer*3;
    style = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL;

	g_crowdStanceListControl = CreateWindowEx(
			xstyle, "ComboBox", "", style | WS_TABSTOP,
			x, y, boxW , boxH * 8,
			parent, NULL, NULL, NULL);
    SendMessage(g_crowdStanceListControl, WM_SETFONT, (WPARAM)hObj, true);
    SendMessage(g_crowdStanceListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Default game logic");
    SendMessage(g_crowdStanceListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"1 - Home/Away");
    SendMessage(g_crowdStanceListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"2 - Neutral");
    SendMessage(g_crowdStanceListControl, CB_ADDSTRING, (WPARAM)0, (LPARAM)"3 - Player");
    SendMessage(g_crowdStanceListControl, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

	style = WS_CHILD | WS_VISIBLE;

    y += spacer*3 + butH;
	x = borW - butW;

	// BOTTOM sections: buttons
	
	g_defButtonControl = CreateWindowEx(
			xstyle, "Button", "Defaults", style,
			x, y, butW + spacer, butH,
			parent, NULL, NULL, NULL);

	butW = 60;
	x -= butW + spacer;

	g_saveButtonControl = CreateWindowEx(
			xstyle, "Button", "Save", style,
			x - spacer, y, butW + spacer, butH,
			parent, NULL, NULL, NULL);

	x = spacer;
	statW = WIN_WIDTH - spacer*4 - 160;

	g_statusTextControl = CreateWindowEx(
			xstyle, "Static", "", style,
			x, y+6, statW, statH,
			parent, NULL, NULL, NULL);

	// If any control wasn't created, return false
	if (g_gameInfoControl == NULL || g_exeNameControl == NULL || 
		//g_findMoreButtonControl == NULL || 
		g_statusTextControl == NULL ||
		g_saveButtonControl == NULL || g_defButtonControl == NULL)
		return false;

	SendMessage(exeNameLabel, WM_SETFONT, (WPARAM)hObj, true);
	SendMessage(g_gameInfoControl, WM_SETFONT, (WPARAM)hObj, true);
	SendMessage(g_exeNameControl, WM_SETFONT, (WPARAM)hObj, true);
    for (i=0; i<5; i++) {
        SendMessage(g_lodListControl[i], WM_SETFONT, (WPARAM)hObj, true);
		EnableWindow(g_lodListControl[i], FALSE);
    }
    EnableWindow(g_crowdCheckBox, FALSE);
    SendMessage(g_weatherListControl, WM_SETFONT, (WPARAM)hObj, true);
    EnableWindow(g_weatherListControl, FALSE);
	//SendMessage(g_findMoreButtonControl, WM_SETFONT, (WPARAM)hObj, true);

	SendMessage(g_statusTextControl, WM_SETFONT, (WPARAM)hObj, true);
	SendMessage(g_saveButtonControl, WM_SETFONT, (WPARAM)hObj, true);
	SendMessage(g_defButtonControl, WM_SETFONT, (WPARAM)hObj, true);
	SendMessage(g_crowdCheckBox, WM_SETFONT, (WPARAM)hObj, true);

	EnableWindow(g_saveButtonControl, FALSE);
	EnableWindow(g_defButtonControl, FALSE);

	return true;
}

