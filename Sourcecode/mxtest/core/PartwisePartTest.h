#pragma once
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

namespace MxTestHelpers
{
    mx::core::PartwisePartPtr tgenPartwisePart( variant v );
    void tgenPartwisePartExpected( std::ostream& os, int indentLevel, variant v );
}

#endif
