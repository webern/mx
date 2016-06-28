#pragma once
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

namespace MxTestHelpers
{
    mx::core::LyricPtr tgenLyric( variant v );
    void tgenLyricExpected( std::ostream& os, int indentLevel, variant v );
}

#endif
