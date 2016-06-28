#pragma once

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

namespace MxTestHelpers
{
    mx::core::RestPtr tgenRest( variant v );
    void tgenRestExpected( std::ostream& os, int indentLevel, variant v );
}

#endif
