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
    mx::core::TechnicalChoicePtr tgenTechnicalChoice( variant v );
    void tgenTechnicalChoiceExpected( std::ostream& os, int indentLevel, variant v );
}

#endif
