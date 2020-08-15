// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

namespace mxtest
{
    mx::core::HarmonicPtr tgenHarmonic( TestMode v );
    void tgenHarmonicExpected(std::ostream& os, int indentLevel, TestMode v );
}

#endif
