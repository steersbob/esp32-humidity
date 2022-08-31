#!/usr/bin/env bash
set -euo pipefail
pushd "$(git rev-parse --show-toplevel)" >/dev/null

docker buildx build \
    --tag kargathia/esp32-humidity-devcontainer:focal \
    --build-arg USERNAME=vscode \
    --push \
    .devcontainer
