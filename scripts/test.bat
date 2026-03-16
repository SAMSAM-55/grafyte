@echo off
setlocal enableextensions

rem Go to the folder where this .bat lives (so paths are stable)
pushd "%~dp0/../"

rem Compute absolute project root (current folder after pushd)
set "ROOT=%CD%"

rem Ensure internal folder exists
if not exist "%ROOT%\src\__grafyte_internal" mkdir "%ROOT%\src\__grafyte_internal"

rem Copy the .pyd into place
copy /Y "%ROOT%\src\built-win\grafyte_py312.pyd" "%ROOT%\src\__grafyte_internal\grafyte_py312.pyd" >nul
if errorlevel 1 (
  echo Failed to copy grafyte_py312.pyd
  popd
  exit /b 1
)

rem Set PYTHONPATH for this process (and child processes)
set "PYTHONPATH=%ROOT%\src;%ROOT%\tests"

rem Run tests
pushd "%ROOT%\tests"
python test.py
set "TEST_RC=%ERRORLEVEL%"
popd

rem Cleanup (best-effort)
if exist "%ROOT%\src\__grafyte_internal\grafyte_py312.pyd" del /Q "%ROOT%\src\__grafyte_internal\grafyte_py312.pyd"

popd
exit /b %TEST_RC%