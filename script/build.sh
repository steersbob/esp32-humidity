#!/usr/bin/env bash
set -euo pipefail
pushd "$(git rev-parse --show-toplevel)" >/dev/null

if [ "$#" -eq 0 ]; then
    CMD="idf.py build"
else
    CMD="idf.py"
fi

set +u # export.sh can use undefined variables
source /opt/esp/idf/export.sh >/dev/null
set -u

$CMD "$@"
