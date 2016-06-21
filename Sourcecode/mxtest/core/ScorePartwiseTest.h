#pragma once
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

namespace MxTestHelpers
{
    mx::core::ScorePartwisePtr tgenScorePartwise( variant v );
    void tgenScorePartwiseExpected( std::ostream& os, int indentLevel, variant v );
}

#endif
