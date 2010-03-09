/* main */
/* Version 1.0 */

#include <windows.h>
#include <windef.h>
#include <string.h>
#include <stdio.h>
#include <tchar.h>

#define BUFLEN 4096

#include "win32gui.h"
#include "main.h"
#include "detect.h"
#include "imageutil.h"

#define UNDEFINED -1
#define WM_APP_EXECHANGE WM_APP+1

HWND hWnd = NULL;
int g_count = 0;
int g_gameId = UNDEFINED;
bool g_buttonClick = true;
BOOL g_isBeingEdited = FALSE;

char* GameKeys[] = {
	"SOFTWARE\\KONAMI\\PES4", 
	"SOFTWARE\\KONAMIWE8IU\\WE8IU", 
	"SOFTWARE\\KONAMIPES5DEMO_S\\PES5", 
	"SOFTWARE\\KONAMIPES5\\PES5", 
};
char* ShortNames[] = {
	"PES4.exe", 
	"WE8.exe", 
	"PES5.exe", 
    "PES5.exe",
};

extern char* GAME[];

DWORD LOD_TABLE_OFFSET[] = {
    0x6c87fc, 0x6ca7e4, 0x6ca824, 0x461a88, 0x82e5b4, 0x53fa8c, 0x53fa8c, 0x53f61c
};
DWORD LODS[][5] = {
    {0x8cac70, 0x8cad00, 0x8cad90, 0x8cae20, 0x8caeb0},
    {0x8cbc80, 0x8cbd10, 0x8cbda0, 0x8cbe30, 0x8cbec0},
    {0x8ccba0, 0x8ccc30, 0x8cccc0, 0x8ccd50, 0x8ccde0},
    {0x704670, 0x704730, 0x7047f0, 0x7048b0, 0x704950},
    {0x8515f0, 0x8516b0, 0x851770, 0x851830, 0x8518d0},
    {0x316640, 0x316700, 0x3167c0, 0x316880, 0x316920},
    {0x316630, 0x3166f0, 0x3167b0, 0x316870, 0x316910},
    {0x316630, 0x3166f0, 0x3167b0, 0x316870, 0x316910},
};

DWORD CROWDBLOCK_PROC[] = {
    0, 0, 0, 0x79bb0, 0x19bab0, 0x6b870, 0x6b870, 0x6b780,
};

DWORD WEATHER_SWITCH[][2] = {
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0x2e52d8, 0x2e52dd},
    {0x14ca83, 0x14ca88},
    {0x14caf3, 0x14caf8},
    {0x14ca53, 0x14ca58},
};

typedef struct _LCM {
    WORD homeTeamId;
    WORD awayTeamId;
    BYTE stadium;
    BYTE unknown1;
    BYTE timeOfDay;
    BYTE weather;
    BYTE season;
    BYTE effects;
    BYTE unknown2[6];
    BYTE unknown3[2];
    BYTE crowdStance;
    BYTE numSubs;
    BYTE homeCrowd;
    BYTE awayCrowd;
    BYTE unknown4[10];
} LCM;

typedef struct _LCM_DATA {
    LCM lcm;
    DWORD gameFuncAddr;
    DWORD gameLCMAddr;
    BYTE frameCounter;
    BYTE myWeather;
    BYTE padding[6];
} LCM_DATA;

#define LCM_CODECOMMON_LEN 0x4b
#define LCM_CODEWEATHER_LEN 0x75
#define LCM_CODEWEATHERHOOK_LEN 6

typedef struct _LCM_PATCH {
    LCM_DATA data;
    BYTE codeCommon[LCM_CODECOMMON_LEN];
    BYTE codeWeather[LCM_CODEWEATHER_LEN];
} LCM_PATCH;

BYTE PES5PC_LCM_CODE_COMMON[] = {
    0xFF,0x15,0xB0,0xD3,0xAC,0x00,0x52,0x50,0x57,0x56,0xBA,0x00,0x00,0x00,0x00,0x8B,
    0x3D,0xB4,0xD3,0xAC,0x00,0xBE,0x90,0xD3,0xAC,0x00,0x8A,0x04,0x17,0x8A,0x24,0x16,
    0x80,0xFC,0xFF,0x74,0x03,0x88,0x24,0x17,0x42,0x83,0xFA,0x20,0x75,0xEC,0x5E,0x5F,
    0x58,0x5A,0xC3,0x90,0xE8,0x67,0x51,0x94,0xFF,0x50,0xA0,0xB8,0xD3,0xAC,0x00,0xFE,
    0xC0,0x24,0x0F,0xA2,0xB8,0xD3,0xAC,0x00,0x58,0xC3,0x90,
};

BYTE PES5PC_LCM_CODE_COLD_WEATHER[] = { 
    0x50,0x57,0x56,0x8B,0x3D,
    0xB4,0xD3,0xAC,0x00,0xBE,0x90,0xD3,0xAC,0x00,0x8A,0x66,0x07,0x80,0xFC,0x03,0x77,
    0x08,0x88,0x61,0x01,0xE9,0x50,0x00,0x00,0x00,0x8A,0x46,0x08,0x3C,0xFF,0x75,0x03,
    0x8A,0x47,0x08,0x3C,0x01,0x74,0x1C,0x8A,0x66,0x28,0x80,0xFC,0x07,0x77,0x0A,0xB0,
    0x00,0x88,0x41,0x01,0xE9,0x30,0x00,0x00,0x00,0xB0,0x01,0x88,0x41,0x01,0xE9,0x26,
    0x00,0x00,0x00,0x8A,0x66,0x28,0x80,0xFC,0x03,0x77,0x0A,0xB0,0x00,0x88,0x41,0x01,
    0xE9,0x14,0x00,0x00,0x00,0x80,0xFC,0x08,0x77,0x0A,0xB0,0x01,0x88,0x41,0x01,0xE9,
    0x05,0x00,0x00,0x00,0xB0,0x02,0x88,0x41,0x01,0x5E,0x5F,0x58,0xC3,0x00,0x00,0x00,
};

BYTE PES5PC_LCM_CODE_MILD_WEATHER[] = { 
    0x50,0x57,0x56,0x8B,0x3D,
    0xB4,0xD3,0xAC,0x00,0xBE,0x90,0xD3,0xAC,0x00,0x8A,0x66,0x07,0x80,0xFC,0x03,0x77,
    0x08,0x88,0x61,0x01,0xE9,0x50,0x00,0x00,0x00,0x8A,0x46,0x08,0x3C,0xFF,0x75,0x03,
    0x8A,0x47,0x08,0x3C,0x01,0x74,0x1C,0x8A,0x66,0x28,0x80,0xFC,0x09,0x77,0x0A,0xB0,
    0x00,0x88,0x41,0x01,0xE9,0x30,0x00,0x00,0x00,0xB0,0x01,0x88,0x41,0x01,0xE9,0x26,
    0x00,0x00,0x00,0x8A,0x66,0x28,0x80,0xFC,0x07,0x77,0x0A,0xB0,0x00,0x88,0x41,0x01,
    0xE9,0x14,0x00,0x00,0x00,0x80,0xFC,0x0B,0x77,0x0A,0xB0,0x01,0x88,0x41,0x01,0xE9,
    0x05,0x00,0x00,0x00,0xB0,0x02,0x88,0x41,0x01,0x5E,0x5F,0x58,0xC3,0x00,0x00,0x00,
};

BYTE PES5PC_LCM_CODE_WEATHERHOOK[] = {
    0xe8,0x2e,0x81,0x3e,0x00,0x90
};

BYTE PES5PC_LCM_CODE_WEATHERHOOK_KONAMI[] = {
    0x90,0x90,0xc6,0x41,0x01,0x03
};

BYTE PES5PC_LCM_CODE_WEATHERHOOK_UNDO[] = {
    0x7a,0x04,0xc6,0x41,0x01,0x03
};

BYTE* LCM_CODE[][6] = {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {PES5PC_LCM_CODE_COMMON, PES5PC_LCM_CODE_MILD_WEATHER, PES5PC_LCM_CODE_COLD_WEATHER,
     PES5PC_LCM_CODE_WEATHERHOOK, PES5PC_LCM_CODE_WEATHERHOOK_KONAMI,
     PES5PC_LCM_CODE_WEATHERHOOK_UNDO},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
};

DWORD LCM_DATA_ADDRESSES[][2] = {
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0xa43b20, 0x3be0f40},
    {0, 0},
    {0, 0},
    {0, 0},
};

// location of the entire LCM-patch
DWORD LCM_PATCH_LOCATION[][2] = {
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0x6cd390, 0xacd390},  // file offset, runtime address
    {0, 0},
    {0, 0},
    {0, 0},
};

// location of frame-counter function
DWORD FC_LOCATION[][2] = {
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0x6cd3f4, 0xacd3f4},  // file offset, runtime address
    {0, 0},
    {0, 0},
    {0, 0},
};

//// LCM-function

BYTE LCM_PATCH_HOOK[][5] = {
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0xe8, 0xc4, 0xc0, 0x09, 0x00}, // call to LCM-patch procedure
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
};

BYTE LCM_PATCH_HOOK_UNDO[][5] = {
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0xe8, 0x24, 0x28, 0x01, 0x00}, // original call
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
};

DWORD LCM_PATCH_HOOK_CS[][2] = {
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0x6312f7, 0xa312f7},  // file offset, runtime address
    {0, 0},
    {0, 0},
    {0, 0},
};

//// frame counter

BYTE LCM_PATCH_FC_HOOK[][5] = {
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0xe8, 0xff, 0xe8, 0x6b, 0x00}, // call to frame-counter procedure
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
};

BYTE LCM_PATCH_FC_HOOK_UNDO[][5] = {
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0xe8, 0x6b, 0x3a, 0x00, 0x00}, // original call 
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
};

DWORD LCM_PATCH_FC_HOOK_CS[][2] = {
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0xeaf0, 0x40eaf0},  // file offset, runtime address
    {0, 0},
    {0, 0},
    {0, 0},
};

// xbox weather
#define XBOX_WEATHER_LEN 6
BYTE XBOX_WEATHERCODE[][XBOX_WEATHER_LEN] = {
    {0x76,0x04,0xc6,0x46,0x01,0x03},
    {0x90,0x90,0xc6,0x46,0x01,0x00},
    {0x90,0x90,0xc6,0x46,0x01,0x01},
    {0x90,0x90,0xc6,0x46,0x01,0x02},
    {0x90,0x90,0xc6,0x46,0x01,0x03},
};

// function prototypes
void EnableControls(BOOL flag);
void ShowInfoForSelected(HWND control);
void ShowInfoForSelected(char* exeName);
BOOL GetExeNameFromReg(HWND control);
void ShowGameInfo(char* exeName, int* gameId);
void InstallPatch(int gameId);
void UninstallPatch(int gameId);
void WritePatch(int gameId, BYTE* data, char* msg);
void PopulateLodLists(int* indices);
void SetCrowdCheckBox(char* exeName);
void SetLCM(char* exeName);
void SaveSetup(char* exeName);

void ShowInfoForSelected(HWND control)
{
	int idx = (int)SendMessage(control, CB_GETCURSEL, 0, 0);
	idx = (idx == -1) ? 0: idx;
	char buf[BUFLEN];
	ZeroMemory(buf, BUFLEN);
	SendMessage(control, CB_GETLBTEXT, (WPARAM)idx, (LPARAM)buf);
	ShowGameInfo(buf, &g_gameId);
}

void ShowInfoForSelected(char* exeName)
{
	ShowGameInfo(exeName, &g_gameId);
}

void SetLCMDefaults()
{
    SendMessage(g_weatherListControl, CB_SETCURSEL, 0, 0);
    SendMessage(g_timeListControl, CB_SETCURSEL, 0, 0);
    SendMessage(g_seasonListControl, CB_SETCURSEL, 0, 0);
    SendMessage(g_stadiumEffectsListControl, CB_SETCURSEL, 0, 0);
    SendMessage(g_stadiumListControl, CB_SETCURSEL, 0, 0);
    SendMessage(g_homeCrowdListControl, CB_SETCURSEL, 0, 0);
    SendMessage(g_awayCrowdListControl, CB_SETCURSEL, 0, 0);
    SendMessage(g_crowdStanceListControl, CB_SETCURSEL, 0, 0);
}

void EnableLCM(BOOL value)
{
    EnableWindow(g_weatherListControl, value);
    EnableWindow(g_timeListControl, value);
    EnableWindow(g_seasonListControl, value);
    EnableWindow(g_stadiumEffectsListControl, value);
    EnableWindow(g_stadiumListControl, value);
    EnableWindow(g_homeCrowdListControl, value);
    EnableWindow(g_awayCrowdListControl, value);
    EnableWindow(g_crowdStanceListControl, value);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_DESTROY:
			// Exit the application when the window closes
			PostQuitMessage(1);
			return true;

		case WM_COMMAND:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				if ((HWND)lParam == g_saveButtonControl)
				{
					// save LOD
					g_buttonClick = true;

                    char exeName[BUFLEN];
                    ZeroMemory(exeName, BUFLEN);
                    SendMessage(g_exeNameControl, WM_GETTEXT, (WPARAM)BUFLEN, (LPARAM)exeName);
					SaveSetup(exeName);
				}
				else if ((HWND)lParam == g_defButtonControl)
				{
					// reset defaults 
					g_buttonClick = true;

                    int idx[] = {0,1,2,3,4};
                    PopulateLodLists(idx);
                    SendMessage(g_crowdCheckBox, BM_SETCHECK, BST_UNCHECKED, 0);
                    SetLCMDefaults();
				}
			}
			else if (HIWORD(wParam) == CBN_EDITUPDATE)
			{
				/*
				if (g_buttonClick) g_buttonClick = false;
				else
				{
					HWND control = (HWND)lParam;
					// clear out game info control
					SendMessage(g_gameInfoControl, WM_SETTEXT, 0, (LPARAM)"");
					g_gameId = UNDEFINED;
				}
				*/
				g_isBeingEdited = TRUE;
			}
			else if (HIWORD(wParam) == CBN_KILLFOCUS && g_isBeingEdited)
			{
				/*
				if (g_buttonClick) g_buttonClick = false;
				else
				{
					HWND control = (HWND)lParam;
					// clear out game info control
					SendMessage(g_gameInfoControl, WM_SETTEXT, 0, (LPARAM)"");
					g_gameId = UNDEFINED;
				}
				*/
				g_isBeingEdited = FALSE;
				HWND control = (HWND)lParam;
				if (control == g_exeNameControl) {
					char buf[BUFLEN];
					ZeroMemory(buf, BUFLEN);
					SendMessage(g_exeNameControl, WM_GETTEXT, BUFLEN, (LPARAM)buf);
					ShowInfoForSelected(buf);
				}
			}
			else if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				HWND control = (HWND)lParam;
				if (control == g_exeNameControl) {
					ShowInfoForSelected(g_exeNameControl);
				}
			}
			break;

		case WM_APP_EXECHANGE:
			if (wParam == VK_RETURN) {
				g_isBeingEdited = FALSE;
				MessageBox(hWnd, "WM_APP_EXECHANGE", "Installer Message", 0);
			}
			break;
	}
	return DefWindowProc(hwnd,uMsg,wParam,lParam);
}

bool InitApp(HINSTANCE hInstance, LPSTR lpCmdLine)
{
	WNDCLASSEX wcx;

	// cbSize - the size of the structure.
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = (WNDPROC)WindowProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wcx.hCursor = LoadCursor(NULL,IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = "LODMIXERCLS";
	wcx.hIconSm = NULL;

	// Register the class with Windows
	if(!RegisterClassEx(&wcx))
		return false;

	return true;
}

HWND BuildWindow(int nCmdShow)
{
	DWORD style, xstyle;
	HWND retval;

	style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	xstyle = WS_EX_LEFT;

	retval = CreateWindowEx(xstyle,
        "LODMIXERCLS",      // class name
        LODMIXER_WINDOW_TITLE, // title for our window (appears in the titlebar)
        style,
        CW_USEDEFAULT,  // initial x coordinate
        CW_USEDEFAULT,  // initial y coordinate
        WIN_WIDTH, WIN_HEIGHT,   // width and height of the window
        NULL,           // no parent window.
        NULL,           // no menu
        NULL,           // no creator
        NULL);          // no extra data

	if (retval == NULL) return NULL;  // BAD.

	ShowWindow(retval,nCmdShow);  // Show the window
	return retval; // return its handle for future use.
}

// This functions looks up the supported games in the Registry, and
// populates the control. Returns TRUE if at least one was found
BOOL GetExeNameFromReg(HWND listControl)
{
	HKEY hKey = NULL;
	DWORD bc = BUFLEN;
	BOOL res = false;
	
    for (int i=0; i<sizeof(GameKeys)/sizeof(char*); i++) {
        char exeName[BUFLEN];
        ZeroMemory(exeName, BUFLEN);
        char buf[BUFLEN];
        ZeroMemory(buf, BUFLEN);
        lstrcpy(buf, GameKeys[i]);
        bc = BUFLEN;

        if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                buf, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
        {

            if (RegQueryValueEx(hKey, "installdir", NULL, NULL, 
                        (LPBYTE)exeName, &bc) == ERROR_SUCCESS)
            {
                strcat(exeName, ShortNames[i]);
                SendMessage(listControl, CB_INSERTSTRING, (WPARAM)0, (LPARAM)exeName);
                SendMessage(listControl, WM_SETTEXT, (WPARAM)0, (LPARAM)exeName);
                res = true;
            }
            RegCloseKey(hKey);
        }
    }
	return res;
}

void PopulateLodLists(int* indices)
{
    if (indices == NULL) return;
	for (int i=0; i<5; i++) {
        for (int j=0; j<5; j++) {
            SendMessage(g_lodListControl[i], CB_SETCURSEL, (WPARAM)indices[i], (LPARAM)0);
            if (indices[i] == -1) {
                EnableWindow(g_lodListControl[i], FALSE);
            }
        }
	}
}

void SetCrowdCheckBox(char* exeName)
{
    if (g_gameId < 0 || CROWDBLOCK_PROC[g_gameId]==0) {
        // crowd fix unsupported/unneeded
        EnableWindow(g_crowdCheckBox, TRUE);
        SendMessage(g_crowdCheckBox, BM_SETCHECK, BST_UNCHECKED, 0);
        EnableWindow(g_crowdCheckBox, FALSE);
        return;
    }
    FILE* f = fopen(exeName,"rb");
    if (f != NULL) {
        BYTE crowd = 0;
        fseek(f, CROWDBLOCK_PROC[g_gameId], SEEK_SET);
        fread(&crowd, sizeof(BYTE), 1, f);
        fclose(f);

        if (crowd == 0x56) {
            EnableWindow(g_crowdCheckBox, TRUE);
            SendMessage(g_crowdCheckBox, BM_SETCHECK, BST_UNCHECKED, 0);
        } else if (crowd == 0xc3) {
            EnableWindow(g_crowdCheckBox, TRUE);
            SendMessage(g_crowdCheckBox, BM_SETCHECK, BST_CHECKED, 0);
        }
    } else {
		// show message box with error msg
		char buf[BUFLEN];
		ZeroMemory(buf, BUFLEN);
		sprintf(buf, "\
========== ERROR! ===========\n\
Problem reading crowd-switch from this file:\n\
%s.\n\
\n", exeName);

		MessageBox(hWnd, buf, "LOD Mixer Message", 0);
		return;
	}
}

void SetLCM(char* exeName)
{
    SetLCMDefaults();
    if (g_gameId < 0 || LCM_PATCH_HOOK_CS[g_gameId][0]==0) {
        // LCM controls unsupported
        EnableLCM(FALSE);

        //for xbox, enable weather controls
        if (g_gameId >= sizeof(ShortNames)/sizeof(char*)) {
            //remove last 2 items (unsupported on xbox)
            SendMessage(g_weatherListControl, CB_DELETESTRING, (WPARAM)5, (LPARAM)0);
            SendMessage(g_weatherListControl, CB_DELETESTRING, (WPARAM)5, (LPARAM)0);

            EnableWindow(g_weatherListControl, TRUE);

            //set selection
            FILE* f = fopen(exeName,"rb");
            if (f != NULL) {
                WORD weather = 0;
                BYTE type = 0;
                fseek(f, WEATHER_SWITCH[g_gameId][0], SEEK_SET);
                fread(&weather, sizeof(WORD), 1, f);
                fseek(f, WEATHER_SWITCH[g_gameId][1], SEEK_SET);
                fread(&type, sizeof(BYTE), 1, f);

                if (weather == 0x0476) {
                    EnableWindow(g_weatherListControl, TRUE);
                    SendMessage(g_weatherListControl, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
                } else if (weather == 0x9090) {
                    switch (type) {
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                            EnableWindow(g_weatherListControl, TRUE);
                            SendMessage(g_weatherListControl, CB_SETCURSEL, (WPARAM)type+1, (LPARAM)0);
                            break;
                    }
                }
            }
        }
        return;
    }
    FILE* f = fopen(exeName,"rb");
    if (f != NULL) {
        // First, handle "old" weather patch, for backward compatibility support ////////
        WORD weather = 0;
        BYTE type = 0;
        fseek(f, WEATHER_SWITCH[g_gameId][0], SEEK_SET);
        fread(&weather, sizeof(WORD), 1, f);
        fseek(f, WEATHER_SWITCH[g_gameId][1], SEEK_SET);
        fread(&type, sizeof(BYTE), 1, f);

        if (weather == 0x047a) {
            EnableWindow(g_weatherListControl, TRUE);
            SendMessage(g_weatherListControl, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
        } else if (weather == 0x9090) {
            switch (type) {
                case 0:
                case 1:
                case 2:
                case 3:
                    EnableWindow(g_weatherListControl, TRUE);
                    SendMessage(g_weatherListControl, CB_SETCURSEL, (WPARAM)type+1, (LPARAM)0);
                    break;
            }
        }

        BYTE hook[5];
        fseek(f, LCM_PATCH_HOOK_CS[g_gameId][0], SEEK_SET);
        fread(hook, 5, 1, f);
        if (memcmp(hook, LCM_PATCH_HOOK[g_gameId], 5)==0) {
            // LCM-patch hooked
            LCM_PATCH patch;
            fseek(f, LCM_PATCH_LOCATION[g_gameId][0], SEEK_SET);
            fread(&patch, sizeof(LCM_PATCH), 1, f);

            if (patch.data.lcm.weather != 0xff) {
                SendMessage(g_weatherListControl, CB_SETCURSEL, (WPARAM)patch.data.lcm.weather+1, (LPARAM)0);
            } else {
                if (patch.data.myWeather > 0) {
                    // check weather hook
                    BYTE wHook[6];
                    fseek(f, WEATHER_SWITCH[g_gameId][0], SEEK_SET);
                    fread(wHook, 6, 1, f);
                    if (memcmp(wHook, LCM_CODE[g_gameId][3], 6)==0) {
                        patch.data.lcm.weather = patch.data.myWeather;
                        SendMessage(g_weatherListControl, CB_SETCURSEL, (WPARAM)patch.data.lcm.weather+1, (LPARAM)0);
                    }
                }
            }
            if (patch.data.lcm.timeOfDay != 0xff) {
                SendMessage(g_timeListControl, CB_SETCURSEL, (WPARAM)patch.data.lcm.timeOfDay+1, (LPARAM)0);
            } 
            if (patch.data.lcm.season != 0xff) {
                SendMessage(g_seasonListControl, CB_SETCURSEL, (WPARAM)patch.data.lcm.season+1, (LPARAM)0);
            }
            if (patch.data.lcm.effects != 0xff) {
                SendMessage(g_stadiumEffectsListControl, CB_SETCURSEL, (WPARAM)patch.data.lcm.effects+1, (LPARAM)0);
            }
            if (patch.data.lcm.stadium != 0xff) {
                SendMessage(g_stadiumListControl, CB_SETCURSEL, (WPARAM)patch.data.lcm.stadium+1, (LPARAM)0);
            } 
            if (patch.data.lcm.homeCrowd != 0xff) {
                SendMessage(g_homeCrowdListControl, CB_SETCURSEL, (WPARAM)patch.data.lcm.homeCrowd+1, (LPARAM)0);
            } 
            if (patch.data.lcm.awayCrowd != 0xff) {
                SendMessage(g_awayCrowdListControl, CB_SETCURSEL, (WPARAM)patch.data.lcm.awayCrowd+1, (LPARAM)0);
            }
            if (patch.data.lcm.crowdStance != 0xff) {
                SendMessage(g_crowdStanceListControl, CB_SETCURSEL, (WPARAM)patch.data.lcm.crowdStance, (LPARAM)0);
            }
        }
        fclose(f);
        EnableLCM(TRUE);
    } else {
		// show message box with error msg
		char buf[BUFLEN];
		ZeroMemory(buf, BUFLEN);
		sprintf(buf, "\
========== ERROR! ===========\n\
Problem reading LCM controls from this file:\n\
%s.\n\
\n", exeName);

		MessageBox(hWnd, buf, "LOD Mixer Message", 0);
		return;
	}
}

int GetLodIndex(DWORD func)
{
    for (int i=0; i<5; i++) {
        if (func == LODS[g_gameId][i]) return i;
    }
    return -1;
}

void GetLods(char* exeName, int* indices)
{
    DWORD lod[5] = {0,0,0,0,0};

    FILE* f = fopen(exeName,"rb");
    if (f != NULL) {
        fseek(f, LOD_TABLE_OFFSET[g_gameId], SEEK_SET);
        fread(lod, sizeof(DWORD), 5, f);
        fclose(f);

        // determine indices
        for (int i=0; i<5; i++) {
            indices[i] = GetLodIndex(lod[i]);
        }

    } else {
		// show message box with error msg
		char buf[BUFLEN];
		ZeroMemory(buf, BUFLEN);
		sprintf(buf, "\
========== ERROR! ===========\n\
Problem reading LOD from this file:\n\
%s.\n\
\n", exeName);

		MessageBox(hWnd, buf, "LOD Mixer Message", 0);
		return;
	}
}

void WriteHook(FILE* f, DWORD callSite, BYTE* callCode) {
    fseek(f, callSite, SEEK_SET);
    fwrite(callCode, 5, 1, f);
}

void SaveSetup(char* exeName)
{
    DWORD lod[5];
    // determine lods from indices
    for (int i=0; i<5; i++) {
        int idx = (int)SendMessage(g_lodListControl[i], CB_GETCURSEL, 0, 0);
        lod[i] = LODS[g_gameId][idx];
    }

    FILE* f = fopen(exeName,"r+b");
    if (f != NULL) {
        // lods
        fseek(f, LOD_TABLE_OFFSET[g_gameId], SEEK_SET);
        fwrite(lod, sizeof(DWORD), 5, f);
        // crowd-switch
        if (g_gameId >= 0 && CROWDBLOCK_PROC[g_gameId]>0) {
            fseek(f, CROWDBLOCK_PROC[g_gameId], SEEK_SET);

            LRESULT lResult = SendMessage(g_crowdCheckBox, BM_GETCHECK, 0, 0);
            if (lResult == BST_CHECKED) {
                BYTE crowd = 0xc3;
                fwrite(&crowd, sizeof(BYTE), 1, f);
            } else if (lResult == BST_UNCHECKED) {
                BYTE crowd = 0x56;
                fwrite(&crowd, sizeof(BYTE), 1, f);
            }
        }
        // LCM
        if (g_gameId >= 0 && LCM_PATCH_HOOK_CS[g_gameId][0]>0) {
            // build the patch
            BOOL allDefaults = TRUE;
            LCM_PATCH patch;
            memset(&patch.data.lcm, 0xff, sizeof(LCM));
            int idx = (int)SendMessage(g_weatherListControl, CB_GETCURSEL, 0, 0);
            if (idx != 0) {
                patch.data.lcm.weather = idx - 1;
                allDefaults = FALSE;
            } 
            idx = (int)SendMessage(g_timeListControl, CB_GETCURSEL, 0, 0);
            if (idx != 0) {
                patch.data.lcm.timeOfDay = idx - 1;
                allDefaults = FALSE;
            } 
            idx = (int)SendMessage(g_seasonListControl, CB_GETCURSEL, 0, 0);
            if (idx != 0) {
                patch.data.lcm.season = idx - 1;
                allDefaults = FALSE;
            } 
            idx = (int)SendMessage(g_stadiumEffectsListControl, CB_GETCURSEL, 0, 0);
            if (idx != 0) {
                patch.data.lcm.effects = idx - 1;
                allDefaults = FALSE;
            } 
            idx = (int)SendMessage(g_stadiumListControl, CB_GETCURSEL, 0, 0);
            if (idx != 0) {
                patch.data.lcm.stadium = idx - 1;
                allDefaults = FALSE;
            } 
            idx = (int)SendMessage(g_homeCrowdListControl, CB_GETCURSEL, 0, 0);
            if (idx != 0) {
                patch.data.lcm.homeCrowd = idx - 1;
                allDefaults = FALSE;
            } 
            idx = (int)SendMessage(g_awayCrowdListControl, CB_GETCURSEL, 0, 0);
            if (idx != 0) {
                patch.data.lcm.awayCrowd = idx - 1;
                allDefaults = FALSE;
            } 
            idx = (int)SendMessage(g_crowdStanceListControl, CB_GETCURSEL, 0, 0);
            if (idx != 0) {
                patch.data.lcm.crowdStance = idx;
                allDefaults = FALSE;
            } 

            if (allDefaults) {
                // write the zeroes in place of patch
                memset(&patch, 0, sizeof(LCM_PATCH));
                fseek(f, LCM_PATCH_LOCATION[g_gameId][0], SEEK_SET);
                fwrite(&patch, sizeof(LCM_PATCH), 1, f);
                // write original function call instead of hook
                WriteHook(f, LCM_PATCH_HOOK_CS[g_gameId][0], LCM_PATCH_HOOK_UNDO[g_gameId]);
                // write original function call instead of frame-counter hook
                WriteHook(f, LCM_PATCH_FC_HOOK_CS[g_gameId][0], LCM_PATCH_FC_HOOK_UNDO[g_gameId]);
                // write original weather code snippet
                fseek(f, WEATHER_SWITCH[g_gameId][0], SEEK_SET);
                fwrite(LCM_CODE[g_gameId][5], LCM_CODEWEATHERHOOK_LEN, 1, f);

            } else {
                patch.data.gameFuncAddr = LCM_DATA_ADDRESSES[g_gameId][0];
                patch.data.gameLCMAddr = LCM_DATA_ADDRESSES[g_gameId][1];
                patch.data.frameCounter = 0;
                patch.data.myWeather = 0;
                memset(patch.data.padding, 0, 6);
                memcpy(patch.codeCommon, LCM_CODE[g_gameId][0], LCM_CODECOMMON_LEN);
                int algoChoice = (patch.data.lcm.weather==4)?1:2;
                memcpy(patch.codeWeather, LCM_CODE[g_gameId][algoChoice], LCM_CODEWEATHER_LEN);
                if (patch.data.lcm.weather > 2) {
                    patch.data.myWeather = patch.data.lcm.weather;
                    patch.data.lcm.weather = 0xff;
                }
                // write the patch
                fseek(f, LCM_PATCH_LOCATION[g_gameId][0], SEEK_SET);
                fwrite(&patch, sizeof(LCM_PATCH), 1, f);
                // write the hook
                WriteHook(f, LCM_PATCH_HOOK_CS[g_gameId][0], LCM_PATCH_HOOK[g_gameId]);
                // write the frame-counter hook
                WriteHook(f, LCM_PATCH_FC_HOOK_CS[g_gameId][0], LCM_PATCH_FC_HOOK[g_gameId]);
                // write the weather hook
                if (patch.data.myWeather == 3) {
                    // konami random/seasonal
                    fseek(f, WEATHER_SWITCH[g_gameId][0], SEEK_SET);
                    fwrite(LCM_CODE[g_gameId][4], LCM_CODEWEATHERHOOK_LEN, 1, f);
                } else {
                    // any other weather setting (except default)
                    fseek(f, WEATHER_SWITCH[g_gameId][0], SEEK_SET);
                    fwrite(LCM_CODE[g_gameId][3], LCM_CODEWEATHERHOOK_LEN, 1, f);
                }
            }

            // update .text flags in section table
            if (SeekCodeSectionFlags(f)) {
                DWORD flags;
                fread(&flags, sizeof(DWORD), 1, f);
                flags |= 0x80000000; // make code section writeable
                fseek(f, -sizeof(DWORD), SEEK_CUR);
                fwrite(&flags, sizeof(DWORD), 1, f);
            }

        } else if (g_gameId >= sizeof(ShortNames)/sizeof(char*)) {
            //write xbox weather
            int idx = (int)SendMessage(g_weatherListControl, CB_GETCURSEL, 0, 0);
            fseek(f, WEATHER_SWITCH[g_gameId][0], SEEK_SET);
            fwrite(XBOX_WEATHERCODE[idx], XBOX_WEATHER_LEN, 1, f);
        }
        fclose(f);

		// show message box with success msg
		char buf[BUFLEN];
		ZeroMemory(buf, BUFLEN);
		sprintf(buf, "\
========== SUCCESS! ===========\n\
All settings updated for this file:\n\
%s.\n\
\n", exeName);

		MessageBox(hWnd, buf, "LOD Mixer Message", 0);

    } else {
		// show message box with error msg
		char buf[BUFLEN];
		ZeroMemory(buf, BUFLEN);
		sprintf(buf, "\
========== ERROR! ===========\n\
Problem writing LOD info this file:\n\
%s.\n\
\n", exeName);

		MessageBox(hWnd, buf, "LOD Mixer Message", 0);
		return;
	}
}

/**
 * Looks for a specific game, and displays info about it in g_gameInfoControl
 * Returns true if the game was found.
 */
void ShowGameInfo(char* exeName, int* gameId)
{
	EnableControls(FALSE);
    *gameId = GetGameVersion(exeName);
    if (*gameId >= 0) {
        char buf[BUFLEN];
        ZeroMemory(buf, BUFLEN);
        lstrcpy(buf, "EXE Info: ");
        lstrcat(buf, GAME[*gameId]);
        SendMessage(g_gameInfoControl, WM_SETTEXT, 0, (LPARAM)buf);
        int indices[5];
        GetLods(exeName,indices);
        PopulateLodLists(indices);
        SetCrowdCheckBox(exeName);
        SetLCM(exeName);
		EnableControls(TRUE);

    } else if (*gameId == -1) {
        SendMessage(g_gameInfoControl, WM_SETTEXT, 0, (LPARAM)"EXE Info: Unknown or unsupported EXE");
        int indices[] = {-1,-1,-1,-1,-1};
        PopulateLodLists(indices);
        SetCrowdCheckBox(exeName);
        SetLCM(exeName);
    } else if (*gameId == -2) {
        SendMessage(g_gameInfoControl, WM_SETTEXT, 0, (LPARAM)"EXE Info: Unable to find or read the file");
        int indices[] = {-1,-1,-1,-1,-1};
        PopulateLodLists(indices);
        SetCrowdCheckBox(exeName);
        SetLCM(exeName);
    }
}

void EnableControls(BOOL flag)
{
    for (int i=0; i<5; i++) {
        EnableWindow(g_lodListControl[i], flag);
    }
    EnableWindow(g_saveButtonControl, flag);
    EnableWindow(g_defButtonControl, flag);
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg; int retval;

 	if(InitApp(hInstance, lpCmdLine) == false)
		return 0;

	hWnd = BuildWindow(nCmdShow);
	if(hWnd == NULL)
		return 0;

	// build GUI
	if (!BuildControls(hWnd))
		return 0;

	// update controls
	bool found = GetExeNameFromReg(g_exeNameControl);
	if (found)
	{
		ShowInfoForSelected(g_exeNameControl);
	}
	else
	{
		SendMessage(g_gameInfoControl, WM_SETTEXT, 0, (LPARAM)"No supported games found. Enter EXE filename and press [Tab]");
		SendMessage(g_exeNameControl, WM_SETTEXT, 0, (LPARAM)"");
		EnableControls(FALSE);
	}

	// show credits
	char buf[BUFLEN];
	ZeroMemory(buf, BUFLEN);
	strncpy(buf, CREDITS, BUFLEN-1);
	SendMessage(g_statusTextControl, WM_SETTEXT, 0, (LPARAM)buf);

	while((retval = GetMessage(&msg,NULL,0,0)) != 0)
	{
		/*
		// capture some key events
		if (msg.hwnd == g_exeNameControl && msg.message == WM_KEYDOWN) {
			PostMessage(hWnd, WM_APP_EXECHANGE, msg.wParam, (LPARAM)msg.hwnd);
			continue;
		}
		*/

		if(retval == -1)
			return 0;	// an error occured while getting a message

		if (!IsDialogMessage(hWnd, &msg)) // need to call this to make WS_TABSTOP work
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}

