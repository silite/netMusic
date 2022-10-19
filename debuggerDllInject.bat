@echo off
cls
echo Pending...
@REM taskkill /f /im cloudmusic.exe>nul

@REM ping localhost -n 2

del "Z:\CloudMusic\msimg32.dll"
copy Z:\miko\netMusic\RustInjector\target\i686-pc-windows-msvc\release\msimg32.dll "Z:\CloudMusic\msimg32.dll">nul

Z:\CloudMusic\cloudmusic.exe
