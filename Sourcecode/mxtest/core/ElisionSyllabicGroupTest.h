// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#pragma once
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

namespace MxTestHelpers
{
    mx::core::ElisionSyllabicGroupPtr tgenElisionSyllabicGroup( variant v );
    void tgenElisionSyllabicGroupExpected( std::ostream& os, int indentLevel, variant v );
}

#endif
