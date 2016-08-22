// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "mxtest/core/HelperFunctions.h"
#include "mx/core/DocumentHeader.h"

namespace MxTestHelpers
{
    void tgenDocumentHeaderExpected( std::ostream& os, const mx::core::DocumentType doctype );
}

#endif
