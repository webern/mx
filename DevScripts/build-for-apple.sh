#!/usr/bin/env bash
set -eo pipefail

for tool in lipo xcodebuild mktemp uuidgen; do
  if ! command -v ${tool} > /dev/null; then
    echo "* Can't find executable '${tool}'" >&2
    exit 2
  fi
done

# default values for optional parameters

DEFAULT_BUILD_DIR="$(mktemp -d)/$(uuidgen)"
DEFAULT_TEST="false"
DEFAULT_CONFIGURATION="Release"
DEFAULT_CLEANUP="true"

####################################################################################################################
# Parsing Command Line Input
####################################################################################################################

usage() {
  cat >&2 <<EOF
${0##*/}
                   --outdir /where/i/want/the/artifacts
                 [ --build-dir /defaults/to/system/tempdir ]
                 [ --test false ]
                 [ --configuration Release ]
                 [ --cleanup true ]


Builds mx libraries for consumption by iOS and macOS applications.

Required:
   --outdir                     The directory to which build artifacts, such as MxiOS.framework, will be moved
                                when the build is finished.
Optional:
   --build-dir                  The directory that will be used by xcode during the build process. This will
                                default to a random tempdir. To do multiple builds without starting from scratch,
                                specify a build-dir.

   --test                       Whether or not we should build and run the test project first. true/false
   --configuration              Whether to build Release or Debug.
   --cleanup                    Whether or not to delete the build-dir when done. true/false
EOF
}

required_arg() {
  local arg="${1:?}"
  local value="${2}"
  if [ -z "${value}" ]; then
    echo "ERROR: ${arg} is required" >&2
    exit 2
  fi
}

parse_args() {
  while [ ${#} -gt 0 ] ; do
    case "${1}" in
      # required
      --outdir ) shift; OUTDIR="${1}" ;;
      
      # optional
      --build-dir ) shift; BUILD_DIR="${1}" ;;
      --test ) shift; TEST="${1}" ;;
      --configuration ) shift; CONFIGURATION="${1}" ;;
      --cleanup ) shift; CLEANUP="${1}" ;;

      --help ) usage; exit 0 ;;
      *)
        echo "ERROR: Unknown argument: ${1}" >&2
        usage
        exit 2
        ;;
    esac
    shift
  done

  BUILD_DIR="${BUILD_DIR:-${DEFAULT_BUILD_DIR}}"
  TEST="${TEST:-${DEFAULT_TEST}}"
  CLEANUP="${CLEANUP:-${DEFAULT_CLEANUP}}"
  CONFIGURATION="${CONFIGURATION:-${DEFAULT_CONFIGURATION}}"

  # fail if required arguments are missing
  required_arg "--outdir" "${OUTDIR}"

  # check that all optional args are also set by now (with defaults or passed args)
  required_arg "--build-dir" "${BUILD_DIR}"
  required_arg "--test" "${TEST}"
  required_arg "--configuration" "${CONFIGURATION}"
  required_arg "--cleanup" "${CLEANUP}"
}


####################################################################################################################
# Helper Functions
####################################################################################################################

delete_build_dir() {
  if [ "${CLEANUP}" = "true" ]; then
    rm -rf "${BUILD_DIR}"
  fi;
}

####################################################################################################################
# Set Variables
####################################################################################################################

set_variables() {
  # directories
  this_script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
  mx_dir="${this_script_dir}/.."
  xcworkspace="${mx_dir}/Xcode/mx.xcworkspace"

  # artifact names
  ios_name="MxiOS"
  ios_lib_name="lib${ios_name}"
  ios_framework_name="${ios_name}.framework"
  macos_name="MxmacOS"
  macos_lib_name="lib${macos_name}"
  macos_framework_name="${macos_name}.framework"

  # build-dir output paths
  macos_build_path="${BUILD_DIR}/Build/Products/${CONFIGURATION}/${macos_framework_name}"
  ios_build_path="${BUILD_DIR}/Build/Products/${CONFIGURATION}-iphoneos/${ios_framework_name}"
  ios_simulator_build_path="${BUILD_DIR}/Build/Products/${CONFIGURATION}-iphonesimulator/${ios_framework_name}"
  ios_build_binary="${ios_build_path}/${ios_name}"
  ios_simulator_build_binary="${ios_simulator_build_path}/${ios_name}"
  ios_binary_temp_path="${BUILD_DIR}/temp-fat-ios-binary"
}

####################################################################################################################
# Do Builds
####################################################################################################################

do_tests() {
  if [ "${TEST}" -ne "true" ]; then
    return 0
  fi;

  echo "TODO: test is not implemented, skipping"
}

build_for_ios() {
  xcodebuild \
    -workspace "${xcworkspace}" \
    -scheme "${ios_name}" \
    build \
    -derivedDataPath "${BUILD_DIR}" \
    -destination 'generic/platform=iOS' \
    -destination 'generic/platform=iOS simulator' \
    -configuration "${CONFIGURATION}"
}

build_for_macos() {
  xcodebuild \
    -workspace "${xcworkspace}" \
    -scheme "${macos_name}" \
    build \
    -derivedDataPath "${BUILD_DIR}" \
    -destination 'generic/platform=macOS' \
    -destination 'platform=OS X,arch=x86_64' \
    -configuration "${CONFIGURATION}"
}

create_ios_fat_binary() {
  lipo -create -output "${ios_binary_temp_path}" \
    "${ios_build_binary}" \
    "${ios_simulator_build_binary}"

    # in the ios framework, replace the 'skinny' binary with the 'fat' one
    cp -f "${ios_binary_temp_path}" "${ios_build_binary}"
}

move_outputs() {
  mv -f "${ios_build_path}/" "${OUTDIR}/"
  mv -f "${macos_build_path}/" "${OUTDIR}/"
}

trap 'delete_build_dir' EXIT SIGINT SIGTERM

main() {
  parse_args "${@}"
  set_variables

  mkdir -p "${BUILD_DIR}"
  mkdir -p "${OUTDIR}"
  
  if [ "${TEST}" = "true" ]; then
    do_tests    
  fi;
  
  build_for_ios
  build_for_macos
  create_ios_fat_binary
  move_outputs
  delete_build_dir
  echo "Output:"
  echo "${OUTDIR}/${ios_framework_name}"
  echo "${OUTDIR}/${macos_framework_name}"
}

main "${@}"