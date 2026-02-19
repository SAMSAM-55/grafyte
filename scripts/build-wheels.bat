@echo off
setlocal enabledelayedexpansion

rem Always run pip through the same interpreter that built the wheel
set PY=python

if exist dist rmdir /s /q dist
if exist build rmdir /s /q build
if exist src\Grafyte.egg-info rmdir /s /q src\Grafyte.egg-info

%PY% -m build --wheel
if errorlevel 1 exit /b 1

if exist dist_repaired rmdir /s /q dist_repaired
%PY% -m delvewheel repair dist\grafyte-*.whl -w dist_repaired --add-path .

for %%F in (dist_repaired\*.whl) do (
  echo Installing %%F
  %PY% -m pip install --force-reinstall "%%F"
  if errorlevel 1 exit /b 1
)

echo Done.
endlocal
