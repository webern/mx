#pragma once
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

namespace MxTestHelpers
{
    mx::core::StickPtr tgenStick( variant v );
    void tgenStickExpected( std::ostream& os, int indentLevel, variant v );
}

#endif
