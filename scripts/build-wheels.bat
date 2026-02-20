@echo off
setlocal enabledelayedexpansion

set PY=python

if exist dist rmdir /s /q dist
if exist build rmdir /s /q build
if exist src\grafyte.egg-info rmdir /s /q src\grafyte.egg-info

for %%F in (src\built-win\*.pyd) do (
    copy %%F src\__grafyte_internal
)

%PY% -m build --wheel
if errorlevel 1 exit /b 1

%PY% -m delvewheel repair dist\grafyte-*.whl -w dist_repaired --add-path .

for %%F in (dist_repaired\*win*.whl) do (
  echo Installing %%F
  %PY% -m pip install --force-reinstall "%%F"
  if errorlevel 1 exit /b 1
)

for %%F in (src\__grafyte_internal\*.pyd) do (
    del %%F
)

echo Done.
endlocal
