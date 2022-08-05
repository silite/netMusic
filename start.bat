@echo off
@REM if "%1" == "h" goto begin
@REM mshta vbscript:createobject("wscript.shell").run("%~nx0 h",0)(window.close)&&exit
@REM :begin
for /f "tokens=1-5" %%i in ('netstat -ano^|findstr ":12449"') do taskkill /pid %%m /f
for /f "tokens=1-5" %%i in ('netstat -ano^|findstr ":10145"') do taskkill /pid %%m /f

set binpath=%~dp0Backend\NetMusicPlugin\bin\Release\net48\NetMusicPlugin.exe

for /f "delims=" %%i in ('sc query netMusicPlugin') do set querySc=%%i

sc stop netMusicPlugin

echo %querySc%|find ":" >nul
if errorlevel 1 (
  sc create netMusicPlugin binPath= %binpath% displayname= netMusicPlugin start= demand
) else (
  sc config netMusicPlugin binPath= %binpath%
)

sc start netMusicPlugin

@REM cd "Backend/NetMusicPlugin/bin/Debug/net48"
@REM start NetMusicPlugin.exe

cd "nodeMiddleware"
node "index.js"

pause
