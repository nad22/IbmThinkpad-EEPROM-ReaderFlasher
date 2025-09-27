@echo off
title EEPROM Quick Converter
color 0A

echo.
echo ========================================
echo   EEPROM Quick Paste Converter
echo ========================================
echo.

REM Check if Python is available
python --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: Python not found!
    echo Please install Python from https://python.org
    pause
    exit /b 1
)

echo Ready to convert EEPROM hex dumps to C arrays!
echo.
echo Instructions:
echo 1. Copy hex dump from Arduino Serial Monitor
echo 2. This window will ask you to paste it
echo 3. Press Enter twice when done pasting
echo 4. Copy the generated C array
echo.
pause

:convert
cls
echo ========================================
echo   Paste Your Hex Dump Here
echo ========================================
echo.

python quick_converter.py

echo.
echo ========================================
echo   Conversion Complete!
echo ========================================
echo.
echo Want to convert another block? (y/n)
set /p again="Convert another? (y/n): "
if /i "%again%"=="y" goto convert
if /i "%again%"=="yes" goto convert

echo.
echo Thanks for using EEPROM Quick Converter!
pause