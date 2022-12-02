#!/bin/sh

set -e -u

# 参考: https://stackoverflow.com/questions/3572030/bash-script-absolute-path-with-os-x
realpath() {
  [[ $1 = /* ]] && echo "$1" || echo "$PWD/${1#./}"
}

SCRIPT_NAME="$(basename "$(realpath "${BASH_SOURCE:-$0}")")"
SCRIPT_DIRECTORY="$(dirname "$(realpath "${BASH_SOURCE:-$0}")")"

cd "${SCRIPT_DIRECTORY}/.."

echo "[juce_meets_soundtouch] Build Debug version..."
rm -rf Build-Debug
cmake -B Build-Debug -G "Ninja" . -DCMAKE_BUILD_TYPE=Debug -DCMAKE_OSX_ARCHITECTURES=x86_64 -DVCPKG_TARGET_TRIPLET=x64-osx
cmake --build Build-Debug --config Debug

echo "[juce_meets_soundtouch] Build Release version..."
rm -rf Build-Release
cmake -B Build-Release  -G "Ninja" . -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=x86_64 -DVCPKG_TARGET_TRIPLET=x64-osx
cmake --build Build-Release --config Release
