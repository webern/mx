#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#pragma once
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

namespace MxTestHelpers
{
    mx::core::CreditChoicePtr tgenCreditChoice( variant v );
    void tgenCreditChoiceExpected( std::ostream& os, int indentLevel, variant v );
}

#endif
