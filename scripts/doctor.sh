#!/bin/sh
# Reports which make targets this machine can run, in Docker mode and in
# native mode (MX_TOOLCHAIN=native). Run it with `make doctor`, which passes in
# the tool names the Makefile uses. It only reports; it always exits 0.
#
# The target groups below follow the native recipes in the Makefile. When a
# recipe starts to use a new tool, add it here too.

ROOT=$(cd "$(dirname "$0")/.." && pwd)

DOCKER=${DOCKER:-docker}
CMAKE=${CMAKE:-cmake}
QUALITY_VENV=${QUALITY_VENV:-$ROOT/build/quality-venv}
DESLAG=${DESLAG:-$ROOT/build/cargo-home/bin/deslag}
if [ -z "$CLANG_FORMAT" ]; then
    if command -v clang-format-18 >/dev/null 2>&1; then CLANG_FORMAT=clang-format-18; else CLANG_FORMAT=clang-format; fi
fi
if [ -z "$GCOV" ]; then
    if command -v gcov-14 >/dev/null 2>&1; then GCOV=gcov-14; else GCOV=gcov; fi
fi

# CMake uses $CXX and $CC when they are set, else c++ and cc. Only the first
# word is the program, as in CXX="ccache g++".
CXX_TOOL=$(printf '%s\n' "${CXX:-c++}" | awk '{print $1}')
CC_TOOL=$(printf '%s\n' "${CC:-cc}" | awk '{print $1}')

has() {
    command -v "$1" >/dev/null 2>&1
}

# Prints "ok", or "MISSING" and the names of the tools that are not found.
need() {
    missing=''
    for tool in "$@"; do
        has "$tool" || missing="$missing $tool"
    done
    if [ -z "$missing" ]; then echo 'ok'; else echo "MISSING$missing"; fi
}

# The C target links libxml2. CMake can find it without pkg-config, as it does
# in the macOS SDK, so look in each of those places.
libxml2_found() {
    if has pkg-config && pkg-config --exists libxml-2.0 2>/dev/null; then return 0; fi
    if has xml2-config; then return 0; fi
    [ -d /usr/include/libxml2 ] && return 0
    if has xcrun; then
        sdk=$(xcrun --show-sdk-path 2>/dev/null)
        [ -n "$sdk" ] && [ -d "$sdk/usr/include/libxml2" ] && return 0
    fi
    return 1
}

row() {
    printf '  %-36s %s\n' "$1" "$2"
}

echo 'mx doctor: which make targets this machine can run.'
echo ''

# --- Docker -----------------------------------------------------------------

echo "Docker mode (the default):"
docker_ok=0
if ! has "$DOCKER"; then
    echo "  $DOCKER: not found."
else
    # Ask the daemon for its version, and give up after about 10 seconds so a
    # stuck daemon cannot hang the report.
    out=$(mktemp "${TMPDIR:-/tmp}/mx-doctor.XXXXXX")
    "$DOCKER" info --format '{{.ServerVersion}}' >"$out" 2>/dev/null &
    pid=$!
    waited=0
    while kill -0 "$pid" 2>/dev/null && [ "$waited" -lt 10 ]; do
        sleep 1
        waited=$((waited + 1))
    done
    kill "$pid" 2>/dev/null
    if wait "$pid" 2>/dev/null; then
        docker_ok=1
        echo "  $DOCKER: ok, daemon reachable (server $(head -n 1 "$out"))."
    else
        echo "  $DOCKER: found, but the daemon did not answer. Is Docker running?"
    fi
    rm -f "$out"
fi
if [ "$docker_ok" -eq 1 ]; then
    echo '  Docker mode works for every target: make <target>'
else
    echo '  Docker mode will not work. Use MX_TOOLCHAIN=native make <target>,'
    echo '  for the targets marked ok below.'
fi
echo ''

# --- Native -----------------------------------------------------------------

echo 'Native mode (MX_TOOLCHAIN=native make <target>):'
row 'test-all, core-*, api-* (see below)' "$(need "$CMAKE" "$CXX_TOOL" make)"
row 'symbol-check' "$(need nm c++filt)"
row 'backend-pugixml-test' "$(need "$CMAKE" "$CXX_TOOL" git nm c++filt)"
row 'wasm-lib, wasm-build, wasm-test' "$(need "$CMAKE" emcmake emmake node)"
row 'core-coverage, api-coverage' "$(need "$CMAKE" "$CXX_TOOL" gcovr "$GCOV")"
row 'gen, gen-<lang>, gen-test, audit*' "$(need python3)"
row 'api-roundtrip-classify' "$(need python3)"
if [ -x "$QUALITY_VENV/bin/python" ]; then
    row 'gen-quality, gen-lint' "$(need python3)"
else
    row 'gen-quality, gen-lint' 'MISSING venv; run make quality-venv'
fi
row 'fmt, fmt-check' "$(need "$CLANG_FORMAT")"
row 'build-go, test-go' "$(need go)"
c_status=$(need "$CMAKE" "$CC_TOOL")
if ! libxml2_found; then
    if [ "$c_status" = 'ok' ]; then c_status='MISSING libxml2'; else c_status="$c_status libxml2"; fi
fi
row 'build-c, test-c' "$c_status"
if [ -x "$DESLAG" ]; then
    row 'deslag (not used by targets yet)' 'ok'
elif has cargo; then
    row 'deslag (not used by targets yet)' 'MISSING; run make deslag'
else
    row 'deslag (not used by targets yet)' 'MISSING; make deslag needs cargo'
fi
if has ccache; then
    row 'ccache (optional)' 'found; native builds use it'
else
    row 'ccache (optional)' 'not found; it only makes rebuilds faster'
fi
echo ''

# --- Versions ---------------------------------------------------------------

echo 'Versions found, for information:'
version() {
    has "$1" || return 0
    v=$("$@" 2>/dev/null | awk 'NF { print; exit }' | cut -c1-60)
    row "$1" "${v:-found}"
}
version "$CMAKE" --version
version "$CXX_TOOL" --version
version python3 --version
version go version
version node --version
version "$CLANG_FORMAT" --version
version gcovr --version
version "$GCOV" --version
version ccache --version
if [ -x "$DESLAG" ]; then
    row deslag "$("$DESLAG" --version 2>/dev/null | awk 'NF { print; exit }' | cut -c1-60)"
fi
echo 'CI pins its own tool versions in the mx-sdk image; a different local'
echo 'version is usually fine and CI is the final check.'

exit 0
