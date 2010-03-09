@echo off
echo Setting kitserver compile environment
@call "c:\program files\Microsoft Visual Studio 9.0\vc\vcvarsall.bat"
set DXSDK=c:\dxsdk81
set INCLUDE=%INCLUDE%;%DXSDK%\include
set LIB=%LIB%;%DXSDK%\lib\x86
echo Environment set

