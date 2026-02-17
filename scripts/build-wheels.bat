@echo off
setlocal enabledelayedexpansion

rem Always run pip through the same interpreter that built the wheel
set PY=python

if exist dist rmdir /s /q dist
if exist build rmdir /s /q build
if exist src\Grafyte.egg-info rmdir /s /q src\Grafyte.egg-info

python -m build --wheel
if errorlevel 1 exit /b 1

for %%F in (dist\*.whl) do (
  echo Installing %%F
  python -m pip install --force-reinstall "%%F"
  if errorlevel 1 exit /b 1
)

echo Done.
endlocal


rem Optional: repair with delvewheel (uncomment if needed)
rem if exist dist_repaired rmdir /s /q dist_repaired
rem %PY% -m delvewheel repair dist\grafyte-*.whl -w dist_repaired --add-path .
rem %PY% -m pip install --force-reinstall dist_repaired\*.whl

python -m pip install --force-reinstall dist\grafyte-0.2.0-py3-none-any.whl

echo Done.
endlocal
