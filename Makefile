CC=cl
CFLAGS=/nologo
LINK=link
LFLAGS=/NOLOGO /NODEFAULTLIB:libci.lib
LIBS=user32.lib gdi32.lib advapi32.lib comdlg32.lib

!if "$(debug)"=="1"
EXTRA_CFLAGS=
!else
EXTRA_CFLAGS=/DMYDLL_RELEASE_BUILD
!endif

all: lodmixer.exe
lodmixer.exe: main.obj win32gui.obj detect.obj imageutil.obj
	$(LINK) $(LFLAGS) /out:lodmixer.exe main.obj win32gui.obj detect.obj imageutil.obj $(LIBS)
main.obj: main.cpp main.h win32gui.h detect.h imageutil.h
win32gui.obj: win32gui.cpp win32gui.h
detect.obj: detect.cpp detect.h
imageutil.obj: imageutil.cpp imageutil.h

.cpp.obj:
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c $<

clean:
	del /Q /F *.exp *.lib *.obj

clean-all:
	del /Q /F *.exp *.lib *.obj *.log *~
