#! /bin/bash
set -e

sep() {
    echo "----------------------------------------------------------------"
}

sep
echo "version info"
c++ --version && cmake --version && git --version && cat /etc/os-release

sep
cmake \
    -DMX_BUILD_TESTS=on \
    -DMX_BUILD_CORE_TESTS=off \
    -DMX_BUILD_EXAMPLES=on \
    .

sep
make -j12

sep
echo "running mxhide"
./mxhide
echo "success"

sep
echo "running mxwrite"
./mxwrite
echo "success"

sep
echo "running mxread"
./mxread
echo "success"

sep
echo "running mxtest"
./mxtest
echo "success"

echo ""
echo "done running script - success"
exit 0
