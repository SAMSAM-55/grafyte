#!/usr/bin/env bash
set -euo pipefail

IMAGE="manylinux-clion"
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PY=/opt/python/cp310-cp310/bin/python

docker run --rm -it -v ${PWD}:/io -w /io manylinux-clion bash -lc "
    set -euo pipefail
    ${PY} --version
    ${PY} -m pip install -U pip build wheel auditwheel setuptools
    chmod +x scripts/build-wheels.sh
    scripts/build-wheels.sh"