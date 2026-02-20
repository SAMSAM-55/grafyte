#!/usr/bin/env bash
set -euo pipefail

PY=/opt/python/cp310-cp310/bin/python

# Clean
rm -rf dist build src/grafyte.egg-info

# Copy built extension modules into package (Linux: *.so; Windows would be *.pyd)
# Adjust the glob if your build output dir differs.
shopt -s nullglob
for f in src/built-linux/*.so src/built-linux/*.so.*; do
  cp -f "$f" src/__grafyte_internal/
done
shopt -u nullglob

# Build wheel
"$PY" setup.py bdist_wheel
"$PY" scripts/force_platlib_wheel.py

# Repair wheel with auditwheel (manylinux)
mkdir -p dist_repaired
for whl in dist/*linux*.whl; do
  echo "Repairing $whl"
  auditwheel repair "$whl" -w dist_repaired
done

# Install repaired wheels that look like Linux wheels
shopt -s nullglob
for whl in dist_repaired/*linux*.whl; do
  echo "Installing $whl"
  "$PY" -m pip install --force-reinstall --no-cache-dir "$whl"
done
shopt -u nullglob

# Cleanup copied binaries from source tree
shopt -s nullglob
rm -f src/__grafyte_internal/*.so src/__grafyte_internal/*.so.*
shopt -u nullglob

echo "Done."