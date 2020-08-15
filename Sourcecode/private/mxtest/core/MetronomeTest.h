// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "mxtest/core/HelperFunctions.h"
#include "mx/core/Elements.h"

using namespace mx::core;
using namespace std;

namespace mxtest
{
    BeatUnitGroupPtr tgenBeatUnitGroup( TestMode v );
    MetronomeRelationGroupPtr tgenMetronomeRelationGroup( TestMode v );
    NoteRelationNotePtr tgenNoteRelationNote( TestMode v );
    PerMinuteOrBeatUnitChoicePtr tgenPerMinuteOrBeatUnitChoice( TestMode v );
    BeatUnitPerPtr tgenBeatUnitPer( TestMode v );
    BeatUnitPerOrNoteRelationNoteChoicePtr tgenBeatUnitPerOrNoteRelationNoteChoice( TestMode v );
    MetronomeAttributesPtr tgenMetronomeAttributesPtr( TestMode v );
    MetronomePtr tgenMetronome( TestMode v );
}

#endif
