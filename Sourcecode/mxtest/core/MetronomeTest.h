#pragma once
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

using namespace mx::core;
using namespace std;

namespace MxTestHelpers
{
    BeatUnitGroupPtr tgenBeatUnitGroup( variant v );
    MetronomeRelationGroupPtr tgenMetronomeRelationGroup( variant v );
    NoteRelationNotePtr tgenNoteRelationNote( variant v );
    PerMinuteOrBeatUnitChoicePtr tgenPerMinuteOrBeatUnitChoice( variant v );
    BeatUnitPerPtr tgenBeatUnitPer( variant v );
    BeatUnitPerOrNoteRelationNoteChoicePtr tgenBeatUnitPerOrNoteRelationNoteChoice( variant v );
    MetronomeAttributesPtr tgenMetronomeAttributesPtr( variant v );
    MetronomePtr tgenMetronome( variant v );
}

#endif
