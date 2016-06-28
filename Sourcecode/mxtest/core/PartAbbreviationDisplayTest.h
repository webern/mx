#pragma once
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

namespace MxTestHelpers
{
    mx::core::PartAbbreviationDisplayPtr tgenPartAbbreviationDisplay( variant v );
    void tgenPartAbbreviationDisplayExpected( std::ostream& os, int indentLevel, variant v );
}

#endif
