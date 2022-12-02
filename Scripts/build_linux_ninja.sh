#!/bin/bash

set -e -u -o pipefail

realpath() {
  [[ $1 = /* ]] && echo "$1" || echo "$PWD/${1#./}"
}

SCRIPT_NAME="$(basename "$(realpath "$0")")"
SCRIPT_DIRECTORY="$(dirname "$(realpath "$0")")"

cd "${SCRIPT_DIRECTORY}/../"

echo "[juce_meets_soundtouch] Build Debug version..."
rm -rf Build-Debug
cmake -B Build-Debug -G "Ninja" . -DCMAKE_BUILD_TYPE=Debug -DVCPKG_TARGET_TRIPLET=x64-linux
cmake --build Build-Debug --config Debug

echo "[juce_meets_soundtouch] Build Release version..."
rm -rf Build-Release
cmake -B Build-Release  -G "Ninja" . -DCMAKE_BUILD_TYPE=Release -DVCPKG_TARGET_TRIPLET=x64-linux
cmake --build Build-Release --config Release
