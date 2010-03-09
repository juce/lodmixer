// detect.cpp

#include "windows.h"
#include <stdio.h>
#include "detect.h"
#include "imageutil.h"

char* GAME[] = { 
    "PES4 PC v1.0",
    "PES4 PC v1.10",
    "WE8I PC",
	"PES5 PC DEMO 2",
	"PES5 PC",
    "PES5 XBOX",
    "PES5 XBOX Beta",
    "WE9 XBOX",
};
char* GAME_GUID[] = {
    "{C1B99C5F-9B3F-42d3-A0BF-FF1AD185A485}",
    "{C1B99C5F-9B3F-42d3-A0BF-FF1AD185A485}",
    "{541E9553-EC5E-49ef-8834-59CEFAFF3B6A}",
	"{5F982571-1E7B-4eee-BFDD-7FBA80117E59}",
    "{FAD2F3FB-BD36-4e6b-A287-A1052274BC3E}",
};
DWORD GAME_GUID_OFFSETS[] = { 0x5a4490, 0x5a5490, 0x5a6490, 0x41a470, 0x6ce490 };

BYTE XBOX_SIG1[][16] = {
    {0x58,0x42,0x45,0x48,0x58,0x2f,0xc4,0x80,0xdb,0xb6,0xab,0xb4,0xd0,0x00,0x6a,0xf8},
    {0x58,0x42,0x45,0x48,0x38,0xdb,0xac,0xa8,0x98,0x1e,0xf2,0xf6,0xe2,0x0e,0xa3,0x06},
    {0x58,0x42,0x45,0x48,0x73,0xb8,0x49,0xa5,0x5f,0x8e,0x2d,0x21,0x81,0x5f,0x8d,0x1c},
};
BYTE XBOX_SIG2[][57] = {
    {0x50,0,0x72,0,0x6f,0,0x45,0,0x76,0,0x6f,0,0x6c,0,0x75,0,
     0x74,0,0x69,0,0x6f,0,0x6e,0,0x53,0,0x6f,0,0x63,0,0x63,0,
     0x65,0,0x72,0,0x35,0,
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0x50,0,0x72,0,0x6f,0,0x45,0,0x76,0,0x6f,0,0x6c,0,0x75,0,
     0x74,0,0x69,0,0x6f,0,0x6e,0,0x53,0,0x6f,0,0x63,0,0x63,0,
     0x65,0,0x72,0,0x35,0,
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0x57,0,0x6f,0,0x72,0,0x6c,0,0x64,0,0x20,0,0x53,0,0x6f,0,
     0x63,0,0x63,0,0x65,0,0x72,0,0x20,0,0x57,0,0x69,0,0x6e,0,
     0x6e,0,0x69,0,0x6e,0,0x67,0,0x20,0,0x45,0,0x6c,0,0x65,0,
     0x76,0,0x65,0,0x6e,0,0x20,0,0x39},
};


// Returns the game version id
int GetGameVersion(void)
{
	HMODULE hMod = GetModuleHandle(NULL);
	for (int i=0; i<sizeof(GAME_GUID)/sizeof(char*); i++)
	{
		char* guid = (char*)((DWORD)hMod + GAME_GUID_OFFSETS[i]);
		if (strncmp(guid, GAME_GUID[i], lstrlen(GAME_GUID[i]))==0)
			return i;
	}
	return -1;
}

// Returns the game version id
int GetGameVersion(char* filename)
{
	char guid[] = "{00000000-0000-0000-0000-000000000000}";

	FILE* f = fopen(filename, "rb");
	if (f == NULL)
		return -2;

	// check for regular exes
	for (int i=0; i<sizeof(GAME_GUID)/sizeof(char*); i++)
	{
		fseek(f, GAME_GUID_OFFSETS[i], SEEK_SET);
		fread(guid, lstrlen(GAME_GUID[i]), 1, f);
		if (strncmp(guid, GAME_GUID[i], lstrlen(GAME_GUID[i]))==0)
		{
			fclose(f);
			return i;
		}
	}

    // check for XBOX xbe files
    BYTE buf1[16];
    BYTE buf2[57];
    fseek(f,0,SEEK_SET);
    fread(buf1, 16, 1, f);
    fseek(f,0x190,SEEK_SET);
    fread(buf2, 57, 1, f);

    if (memcmp(buf1, XBOX_SIG1[0], 16)==0 && memcmp(buf2, XBOX_SIG2[0], 38)==0) {
        return sizeof(GAME_GUID)/sizeof(char*);
    }
    if (memcmp(buf1, XBOX_SIG1[1], 16)==0 && memcmp(buf2, XBOX_SIG2[1], 38)==0) {
        return sizeof(GAME_GUID)/sizeof(char*) + 1;
    }
    if (memcmp(buf1, XBOX_SIG1[2], 16)==0 && memcmp(buf2, XBOX_SIG2[2], 57)==0) {
        return sizeof(GAME_GUID)/sizeof(char*) + 2;
    }

    return -1;
}

