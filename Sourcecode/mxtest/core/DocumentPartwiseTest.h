// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Document.h"

namespace mxtest
{
    mx::core::DocumentPtr tgenDocumentPartwise( variant v );
    void tgenDocumentPartwiseExpected( std::ostream& os, int indentLevel, variant v );
}

#endif
