// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "mxtest/core/HelperFunctions.h"
#include "mx/core/DocumentHeader.h"

namespace mxtest
{
    void tgenDocumentHeaderExpected( std::ostream& os, const mx::core::DocumentType doctype );
}

#endif
