@echo off
setlocal enabledelayedexpansion

py -3.10 -m pip install dist_repaired/grafyte-0.2.4-py3-none-win_amd64.whl --force-reinstall
py -3.11 -m pip install dist_repaired/grafyte-0.2.4-py3-none-win_amd64.whl --force-reinstall
py -3.12 -m pip install dist_repaired/grafyte-0.2.4-py3-none-win_amd64.whl --force-reinstall
py -3.13 -m pip install dist_repaired/grafyte-0.2.4-py3-none-win_amd64.whl --force-reinstall
py -3.14 -m pip install dist_repaired/grafyte-0.2.4-py3-none-win_amd64.whl --force-reinstall

endlocal