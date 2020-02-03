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
echo "running MxHide"
./MxHide
echo "success"

sep
echo "running MxWrite"
./MxWrite
echo "success"

sep
echo "running MxRead"
./MxRead
echo "success"

sep
echo "running MxTest"
./MxTest
echo "success"

echo ""
echo "done running script - success"
exit 0
