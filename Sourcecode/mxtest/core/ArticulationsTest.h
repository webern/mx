// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#pragma once
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

namespace mxtest
{
    mx::core::ArticulationsChoicePtr tgenArticulationsChoice( variant v );
    mx::core::ArticulationsPtr tgenArticulations( variant v );
    void tgenArticulationsExpected( std::ostream& os, int indentLevel, variant v );
}

#endif
