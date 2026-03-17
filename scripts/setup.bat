@echo off
setlocal enabledelayedexpansion

echo Creating Python virtual environment...
python -m venv .venv

if %errorlevel% neq 0 (
    echo [ERROR] Failed to create virtual environment.
    exit /b %errorlevel%
)

echo Activating virtual environment...
call .venv\Scripts\activate.bat

if %errorlevel% neq 0 (
    echo [ERROR] Failed to activate virtual environment.
    exit /b %errorlevel%
)

echo Installing build-related dependencies...
pip install build delvewheel

if %errorlevel% neq 0 (
    echo [ERROR] Failed to install dependencies.
    exit /b %errorlevel%
)

echo.
echo [SUCCESS] Setup complete!
echo Virtual environment created and dependencies installed.
echo.
echo To activate the virtual environment, run:
echo call .venv\Scripts\activate
echo.

endlocal


