# MusicXML Class Library
# Copyright (c) by Matthew James Briggs
# Distributed under the MIT License

# Build-time git SHA capture, run via `cmake -P` as a build step so the value
# survives an IDE rebuild that never re-runs CMake. Rewrites the output TU only
# when the SHA changes (copy_if_different), keeping incremental builds warm.
# Falls back to "unknown" with no git or no checkout, so source-archive builds
# still compile.
#
# Inputs (-D): GIT_EXECUTABLE, MX_GIT_SHA_SRC_DIR, MX_GIT_SHA_TEMPLATE,
# MX_GIT_SHA_OUT.

set(MX_GIT_SHA "unknown")

if(GIT_EXECUTABLE)
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" -C "${MX_GIT_SHA_SRC_DIR}" rev-parse --short=12 HEAD
        OUTPUT_VARIABLE _sha
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE _rc
        ERROR_QUIET)
    if(_rc EQUAL 0 AND _sha)
        set(MX_GIT_SHA "${_sha}")
        # -dirty marks a build from an unclean tree: any modified tracked file
        # or any untracked file (gitignored build artifacts never appear here).
        execute_process(
            COMMAND "${GIT_EXECUTABLE}" -C "${MX_GIT_SHA_SRC_DIR}" status --porcelain
            OUTPUT_VARIABLE _dirty
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET)
        if(_dirty)
            set(MX_GIT_SHA "${MX_GIT_SHA}-dirty")
        endif()
    endif()
endif()

configure_file("${MX_GIT_SHA_TEMPLATE}" "${MX_GIT_SHA_OUT}.tmp" @ONLY)
execute_process(COMMAND "${CMAKE_COMMAND}" -E copy_if_different
    "${MX_GIT_SHA_OUT}.tmp" "${MX_GIT_SHA_OUT}")
file(REMOVE "${MX_GIT_SHA_OUT}.tmp")
