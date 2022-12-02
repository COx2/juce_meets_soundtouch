#!/bin/bash

set -e -u -o pipefail

realpath() {
  [[ $1 = /* ]] && echo "$1" || echo "$PWD/${1#./}"
}

SCRIPT_NAME="$(basename "$(realpath "$0")")"
SCRIPT_DIR="$(dirname "$(realpath "$0")")"

cd "${SCRIPT_DIR}/../External/vcpkg"
./bootstrap-vcpkg.sh
./vcpkg install soundtouch:x64-osx
