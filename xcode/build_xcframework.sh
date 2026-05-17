#!/bin/zsh
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

VENDORED_XCFRAMEWORK="$REPO_ROOT/../komp/Frameworks/FrameworksApple/Mx.xcframework"
OUTPUT_XCFRAMEWORK="/tmp/MxRebuilt.xcframework"
CONFIGURATION="Debug"
INSTALL_OUTPUT=0
KEEP_TEMP=0

usage() {
    cat <<USAGE
Usage: $(basename "$0") [options]

Rebuilds the macOS slice of Mx.xcframework from this mx repo, recreates a full
xcframework using the existing iOS slices, and optionally installs it over the
vendored copy used by Notation.

Options:
  --vendored-xcframework PATH
                            Path to the existing vendored Mx.xcframework.
                            Default: $VENDORED_XCFRAMEWORK
  --output PATH             Path for the rebuilt xcframework.
                            Default: $OUTPUT_XCFRAMEWORK
  --configuration NAME      Xcode build configuration.
                            Default: $CONFIGURATION
  --install                 Replace the vendored xcframework with the rebuilt one.
  --keep-temp               Keep the temporary working directory.
  --help                    Show this message.

Examples:
  $(basename "$0")
  $(basename "$0") --output /tmp/Mx.xcframework
  $(basename "$0") --install
USAGE
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --vendored-xcframework)
            VENDORED_XCFRAMEWORK="$2"
            shift 2
            ;;
        --output)
            OUTPUT_XCFRAMEWORK="$2"
            shift 2
            ;;
        --configuration)
            CONFIGURATION="$2"
            shift 2
            ;;
        --install)
            INSTALL_OUTPUT=1
            shift
            ;;
        --keep-temp)
            KEEP_TEMP=1
            shift
            ;;
        --help)
            usage
            exit 0
            ;;
        *)
            echo "Unknown option: $1" >&2
            usage >&2
            exit 1
            ;;
    esac
done

require_path() {
    local path="$1"
    local label="$2"
    if [[ ! -e "$path" ]]; then
        echo "$label not found: $path" >&2
        exit 1
    fi
}

require_path "$REPO_ROOT/Xcode/Mx.xcodeproj" "mx project"
require_path "$VENDORED_XCFRAMEWORK/ios-arm64/MxiOS.framework" "iOS arm64 slice"
require_path "$VENDORED_XCFRAMEWORK/ios-arm64_x86_64-simulator/MxiOS.framework" "iOS simulator slice"
require_path "$VENDORED_XCFRAMEWORK/ios-arm64_x86_64-maccatalyst/MxiOS.framework" "Mac Catalyst slice"

TEMP_DIR="$(mktemp -d /tmp/mx-xcframework.XXXXXX)"
trap 'if [[ "$KEEP_TEMP" -eq 0 ]]; then rm -rf "$TEMP_DIR"; else echo "Keeping temp dir: $TEMP_DIR"; fi' EXIT

DERIVED_ARM64="$TEMP_DIR/MxDerived-arm64"
DERIVED_X86_64="$TEMP_DIR/MxDerived-x86_64"
UNIVERSAL_FRAMEWORK="$TEMP_DIR/MxmacOS.framework"
ARM64_FRAMEWORK="$DERIVED_ARM64/Build/Products/$CONFIGURATION/MxmacOS.framework"
X86_64_FRAMEWORK="$DERIVED_X86_64/Build/Products/$CONFIGURATION/MxmacOS.framework"
UNIVERSAL_BINARY="$UNIVERSAL_FRAMEWORK/Versions/A/MxmacOS"

build_framework() {
    local arch="$1"
    local derived_data="$2"
    echo "Building MxmacOS.framework for $arch..."
    xcodebuild \
        -project "$REPO_ROOT/Xcode/Mx.xcodeproj" \
        -scheme MxmacOS \
        -configuration "$CONFIGURATION" \
        -derivedDataPath "$derived_data" \
        build \
        CODE_SIGNING_ALLOWED=NO \
        ARCHS="$arch" \
        ONLY_ACTIVE_ARCH=NO
}

build_framework arm64 "$DERIVED_ARM64"
build_framework x86_64 "$DERIVED_X86_64"

require_path "$ARM64_FRAMEWORK/Versions/A/MxmacOS" "arm64 framework binary"
require_path "$X86_64_FRAMEWORK/Versions/A/MxmacOS" "x86_64 framework binary"

echo "Creating universal MxmacOS.framework..."
rm -rf "$UNIVERSAL_FRAMEWORK"
rsync -a "$ARM64_FRAMEWORK/" "$UNIVERSAL_FRAMEWORK/"
lipo -create \
    "$ARM64_FRAMEWORK/Versions/A/MxmacOS" \
    "$X86_64_FRAMEWORK/Versions/A/MxmacOS" \
    -output "$UNIVERSAL_BINARY"
lipo -info "$UNIVERSAL_BINARY"

echo "Recreating xcframework at $OUTPUT_XCFRAMEWORK..."
rm -rf "$OUTPUT_XCFRAMEWORK"
xcodebuild -create-xcframework \
    -framework "$VENDORED_XCFRAMEWORK/ios-arm64/MxiOS.framework" \
    -framework "$VENDORED_XCFRAMEWORK/ios-arm64_x86_64-simulator/MxiOS.framework" \
    -framework "$VENDORED_XCFRAMEWORK/ios-arm64_x86_64-maccatalyst/MxiOS.framework" \
    -framework "$UNIVERSAL_FRAMEWORK" \
    -output "$OUTPUT_XCFRAMEWORK"

if [[ "$INSTALL_OUTPUT" -eq 1 ]]; then
    echo "Installing rebuilt xcframework into $VENDORED_XCFRAMEWORK..."
    rsync -a --delete "$OUTPUT_XCFRAMEWORK/" "$VENDORED_XCFRAMEWORK/"
fi

echo "Done."
echo "Rebuilt xcframework: $OUTPUT_XCFRAMEWORK"
if [[ "$INSTALL_OUTPUT" -eq 0 ]]; then
    echo "Use --install to replace the vendored xcframework automatically."
fi
