// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

namespace MxTestHelpers
{
    mx::core::DirectionPtr tgenDirection( variant v );
    mx::core::EditorialVoiceDirectionGroupPtr tgenEditorialVoiceDirectionGroup( variant v );
    void tgenDirectionExpected( std::ostream& os, int indentLevel, variant v );
}

#endif
