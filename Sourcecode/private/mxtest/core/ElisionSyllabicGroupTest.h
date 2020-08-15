// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#pragma once
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

namespace mxtest
{
    mx::core::ElisionSyllabicGroupPtr tgenElisionSyllabicGroup( TestMode v );
    void tgenElisionSyllabicGroupExpected(std::ostream& os, int indentLevel, TestMode v );
}

#endif
