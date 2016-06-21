#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#pragma once
#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

namespace MxTestHelpers
{
    mx::core::MidiInstrumentPtr tgenMidiInstrument( variant v );
    void tgenMidiInstrumentExpected( std::ostream& os, int indentLevel, variant v );
}

#endif
