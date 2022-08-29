@echo off
cls
echo Pending...
@REM taskkill /f /im cloudmusic.exe>nul

@REM ping localhost -n 2

del "Z:\CloudMusic\msimg32.dll"
copy C:\Users\silite\Desktop\netMusic\msimg32\Debug\msimg32.dll "Z:\CloudMusic\msimg32.dll">nul

Z:\CloudMusic\cloudmusic.exe
