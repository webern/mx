#pragma once
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

namespace MxTestHelpers
{
    mx::core::TupletPtr tgenTuplet( variant v );
    void tgenTupletExpected( std::ostream& os, int indentLevel, variant v );
}

#endif
