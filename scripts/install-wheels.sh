#!/usr/bin/env bash
set -euo pipefail

for pyver in 10 11 12 13 14; do
  python3.$pyver -m pip install dist_repaired/*linux*.whl --force-reinstall
done