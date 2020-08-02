#!/usr/bin/env bash
set -eo pipefail
mx="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
bld="${mx}/build"

echo "mx=${mx}"
echo "bld=${bld}"
procs=$1

mkdir -p "${bld}"
cd "${bld}"
cmake "${mx}" \
  -DMX_BUILD_TESTS=on \
  -DMX_BUILD_CORE_TESTS=on \
  -DMX_BUILD_EXAMPLES=on
make -C "${bld}/Makefile" -j"${procs:=48}"
echo "Running MxRead"
"${bld}/MxRead"
echo "Running MxWrite"
"${bld}/MxWrite"
echo "Running MxTest"
"${bld}/MxTest"
