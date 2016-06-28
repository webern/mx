#pragma once
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

namespace MxTestHelpers
{
    mx::core::GroupNameDisplayPtr tgenGroupNameDisplay( variant v );
    void tgenGroupNameDisplayExpected( std::ostream& os, int indentLevel, variant v );
}

#endif
