@echo off
cls
echo Pending...
@REM taskkill /f /im cloudmusic.exe>nul

@REM ping localhost -n 2

del "Z:\CloudMusic\msimg32.dll"
copy .\msimg32.dll "\msimg32.dll">nul

@REM Z:\CloudMusic\cloudmusic.exe
