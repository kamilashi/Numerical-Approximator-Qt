@echo off
:: Path to your Qt deployment tool
set QT_DEPLOY="C:\Qt\6.9.1\msvc2022_64\bin\windeployqt.exe"

:: Path to the built executable (adjust paths as needed)
set EXE_PATH="out\build\code\application\Debug\application.exe"

:: Run deployment
%QT_DEPLOY% %EXE_PATH%

echo.
echo === Deployment complete ===
pause