#!/bin/zsh
source ~/.zshrc
export WRK="${TMP}/mx-$(uuidgen)"
export MX="${WRK}/mx"
export BLD="${WRK}/build"
cd "${tmp}"
rm -rf "${WRK}"
mkdir -p "${WRK}"
cd "${WRK}"
git clone git@github.com:webern/mx.git "${MX}"
cd "${MX}"
git config user.name "Matthew James Briggs"
git config user.email "matthew.james.briggs@gmail.com"
mx(){
  cd "${MX}"
}
test() {
  "${BLD}/mxtest"
}
build() {
  WASDIR="$(pwd)"
  rm -rf "${BLD}"
  mkdir -p "${BLD}"
  cd "${BLD}"
  cmake "${MX}" \
    -DMX_BUILD_TESTS=on \
    -DMX_BUILD_EXAMPLES=on \
    -DMX_BUILD_CORE_TESTS=on
  make -j9 -f "${BLD}/makefile"
  cd "${WASDIR}"
}
build:test() {
  build
  test
}
