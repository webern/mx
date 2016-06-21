#pragma once

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "mxtest/core/HelperFunctions.h"
#include "mx/core/DocumentPartwise.h"

namespace MxTestHelpers
{
    mx::core::DocumentPartwisePtr tgenDocumentPartwise( variant v );
    void tgenDocumentPartwiseExpected( std::ostream& os, int indentLevel, variant v );
}

#endif
