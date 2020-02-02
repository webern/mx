// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#if defined(MX_COMPILE_API_TESTS) && defined(MX_COMPILE_API_ROUNDTRIP)

#include "cpul/cpulTestHarness.h"
#include "mxtest/api/RoundTrip.h"
#include "mx/api/DocumentManager.h"
#include "mx/core/Document.h"
#include "mx/core/elements/ScorePartwise.h"
#include "mx/core/elements/PartwisePart.h"
#include "mx/core/elements/PartwiseMeasure.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/NoteChoice.h"
#include "mx/core/elements/NormalNoteGroup.h"
#include "mx/core/elements/FullNoteGroup.h"
#include "mx/core/elements/FullNoteTypeChoice.h"
#include "mx/core/elements/Pitch.h"
#include "mx/core/elements/Notations.h"
#include "mx/core/elements/NotationsChoice.h"
#include "mx/core/elements/Tied.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/Direction.h"
#include "mx/core/elements/DirectionType.h"
#include "mx/core/elements/Offset.h"
#include "mx/core/elements/Pedal.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;

TEST( microtones1, PitchData )
{
    ScoreData score;
    score.parts.emplace_back();
    auto& part = score.parts.back();
    part.measures.emplace_back();
    auto& measure = part.measures.back();
    measure.staves.emplace_back();
    auto& staff = measure.staves.back();
    auto& voice = staff.voices[0];
    voice.notes.emplace_back();
    auto& note = voice.notes.back();
    note.pitchData.step = Step::f;
    note.pitchData.accidental = Accidental::threeQuartersSharp;
    note.pitchData.alter = 1;
    note.pitchData.cents = 50.0;

    // round trip it through xml
    auto& mgr = DocumentManager::getInstance();
    auto docId = mgr.createFromScore( score );
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    std::istringstream iss{ xml };
    docId = mgr.createFromStream( iss );
    auto oscore = mgr.getData(docId);

    // get the data after the round trip
    const auto& opart = oscore.parts.back();
    const auto& omeasure = opart.measures.back();
    const auto& ostaff = omeasure.staves.back();
    const auto& ovoice = ostaff.voices.at( 0 );
    const auto& onote = ovoice.notes.back();
    CHECK( note.pitchData.step == onote.pitchData.step );
    CHECK( note.pitchData.alter == onote.pitchData.alter );
    CHECK_DOUBLES_EQUAL( 50.0, onote.pitchData.cents, 0.0001 );
}
T_END;

#endif
