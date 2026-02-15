@echo off
setlocal EnableExtensions EnableDelayedExpansion

REM ============================================================
REM 1) Définir la base Python (USER ou SYSTEM)
REM ============================================================

if defined PYTHON_HOME (
    set PYBASE=%PYTHON_HOME%
) else (
    echo ERROR: PYTHON_HOME is not defined
    echo Please set PYTHON_HOME to your Python installations root
    pause
    exit /b 1
)

for %%V in (310 311 312 313) do (
    if exist "%PYBASE%\Python%%V\include" (
        echo Found Python %%V at %PYBASE%\Python%%V
        set PYTHON%%V=%PYBASE%\Python%%V
    )
)

echo.
echo ===== Python env detected =====
set PYTHON3
echo.

pushd "%~dp0..\engine"

del GrafyteLib.sln 2>nul
del GrafyteLib.vcxproj 2>nul
del GrafyteLib.vcxproj.user 2>nul

del "%CD%\vendor\freetype\freetype.vcxproj" 2>nul
del "%CD%\vendor\freetype\freetype.vcxproj.filters" 2>nul
del "%CD%\vendor\glew\glew.vcxproj" 2>nul
del "%CD%\vendor\glew\glew.vcxproj.filters" 2>nul
del "%CD%\vendor\glfw\glfw.vcxproj" 2>nul
del "%CD%\vendor\glfw\glfw.vcxproj.filters" 2>nul
del "%CD%\vendor\imgui\imgui.vcxproj" 2>nul
del "%CD%\vendor\imgui\imgui.vcxproj.filters" 2>nul
del "%CD%\vendor\sandbox\GrafyteSandbox.vcxproj" 2>nul
del "%CD%\vendor\sandbox\GrafyteSandbox.vcxproj.filters" 2>nul

"%CD%\vendor\bin\premake5.exe" vs2022

popd
pause
