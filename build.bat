(
  cd /d %~dp0netMusicFront
  npm run build
) && (
  cd /d %~dp0nodeMiddleware
  npm run pkg
) && (
  copy /Y %~dp0nodeMiddleware\dist-node\nodeMiddleware.exe %~dp0release
  copy /Y %~dp0AppStart\AppStart\bin\Release\net48 %~dp0release\AppStart
  copy /Y %~dp0Backend\NetMusicPlugin\bin\Release\net48 %~dp0release\Backend
)
