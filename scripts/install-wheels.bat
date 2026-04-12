@echo off
setlocal

for %%F in (dist_repaired\*win*.whl) do (
    echo Installing %%~nxF on all Python versions
    py -3.10 -m pip install "%%F" --force-reinstall
    py -3.11 -m pip install "%%F" --force-reinstall
    py -3.12 -m pip install "%%F" --force-reinstall
    py -3.13 -m pip install "%%F" --force-reinstall
    py -3.14 -m pip install "%%F" --force-reinstall
)

endlocal