#pragma once
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "mxtest/core/HelperFunctions.h"
#include "mx/core/DocumentTimewise.h"

namespace MxTestHelpers
{
    mx::core::DocumentTimewisePtr tgenDocumentTimewise( variant v );
    void tgenDocumentTimewiseExpected( std::ostream& os, int indentLevel, variant v );
}

#endif
