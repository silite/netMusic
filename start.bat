@echo off
if "%1" == "h" goto begin
mshta vbscript:createobject("wscript.shell").run("%~nx0 h",0)(window.close)&&exit
:begin
for /f "tokens=1-5" %%i in ('netstat -ano^|findstr ":12449"') do taskkill /pid %%m /f
for /f "tokens=1-5" %%i in ('netstat -ano^|findstr ":10145"') do taskkill /pid %%m /f

cd /d %~dp0Backend\NetMusicPlugin\bin\Release\net48
start NetMusicPlugin.exe

echo %~dp0

cd %~dp0nodeMiddleware
node "index.js"

pause
