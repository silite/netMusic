@echo off
cls
echo Pending...
@REM taskkill /f /im cloudmusic.exe>nul

@REM ping localhost -n 2

del "C:\Program Files (x86)\Netease\CloudMusic\msimg32.dll"
copy .\msimg32.dll "C:\Program Files (x86)\Netease\CloudMusic\msimg32.dll">nul

@REM Z:\CloudMusic\cloudmusic.exe
