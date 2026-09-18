// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPL_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/TupletData.h"
#include "mx/core/generated/Note.h"
#include "mx/core/generated/NoteType.h"
#include "mx/core/generated/NoteTypeValue.h"
#include "mx/core/generated/TimeModification.h"
#include "mx/core/generated/TimeModificationGroup.h"
#include "mx/core/generated/Tuplet.h"
#include "mx/core/generated/TupletNumber.h"
#include "mx/core/generated/TupletPortion.h"
#include "mx/impl/Cursor.h"
#include "mx/impl/TupletReader.h"

using namespace mx;
using namespace mx::impl;

// issue #440: an explicit <tuplet-actual> must survive, and the absent <tuplet-normal>
// side must be guessed from <time-modification>, not the other way around.
TEST(guessesMissingNormalWithoutClobberingExplicitActual, TupletReader)
{
    core::TupletNumber explicitActualNumber;
    explicitActualNumber.setValue(5);
    core::TupletPortion actualPortion;
    actualPortion.setTupletNumber(explicitActualNumber);

    core::Tuplet tuplet;
    tuplet.setType(core::StartStop::start());
    tuplet.setTupletActual(actualPortion);
    // tuplet-normal is left absent; it must be guessed from time-modification.

    core::TimeModification timeMod;
    timeMod.setActualNotes(3);
    timeMod.setNormalNotes(2);

    core::Note note;
    note.setTimeModification(timeMod);

    Cursor cursor{1, 480};
    TupletReader reader{tuplet, cursor, note};

    std::vector<api::TupletStart> starts;
    std::vector<api::TupletStop> stops;
    reader.parseTuplet(starts, stops);

    REQUIRE(starts.size() == 1);
    // the explicit tuplet-actual/tuplet-number must not be overwritten by the guess
    CHECK_EQUAL(5, starts.front().actualNumber);
    // the absent tuplet-normal side must be guessed from normal-notes
    CHECK_EQUAL(2, starts.front().normalNumber);
}

T_END

// The symmetric case: tuplet-normal is explicit, tuplet-actual is absent and must be guessed.
TEST(guessesMissingActualWithoutClobberingExplicitNormal, TupletReader)
{
    core::TupletNumber explicitNormalNumber;
    explicitNormalNumber.setValue(7);
    core::TupletPortion normalPortion;
    normalPortion.setTupletNumber(explicitNormalNumber);

    core::Tuplet tuplet;
    tuplet.setType(core::StartStop::start());
    tuplet.setTupletNormal(normalPortion);
    // tuplet-actual is left absent; it must be guessed from time-modification.

    core::TimeModification timeMod;
    timeMod.setActualNotes(3);
    timeMod.setNormalNotes(2);

    core::Note note;
    note.setTimeModification(timeMod);

    Cursor cursor{1, 480};
    TupletReader reader{tuplet, cursor, note};

    std::vector<api::TupletStart> starts;
    std::vector<api::TupletStop> stops;
    reader.parseTuplet(starts, stops);

    REQUIRE(starts.size() == 1);
    // the absent tuplet-actual side must be guessed from actual-notes
    CHECK_EQUAL(3, starts.front().actualNumber);
    // the explicit tuplet-normal/tuplet-number must not be overwritten by the guess
    CHECK_EQUAL(7, starts.front().normalNumber);
}

T_END

// When both tuplet-actual and tuplet-normal are absent, both numbers must still be
// guessed correctly from time-modification (this case already worked before the fix,
// because the two swapped-field bugs canceled out; it must keep working after the fix).
TEST(guessesBothFromTimeModificationWhenBothAbsent, TupletReader)
{
    core::Tuplet tuplet;
    tuplet.setType(core::StartStop::start());
    // both tuplet-actual and tuplet-normal are absent.

    core::TimeModification timeMod;
    timeMod.setActualNotes(3);
    timeMod.setNormalNotes(2);

    core::Note note;
    note.setTimeModification(timeMod);

    Cursor cursor{1, 480};
    TupletReader reader{tuplet, cursor, note};

    std::vector<api::TupletStart> starts;
    std::vector<api::TupletStop> stops;
    reader.parseTuplet(starts, stops);

    REQUIRE(starts.size() == 1);
    CHECK_EQUAL(3, starts.front().actualNumber);
    CHECK_EQUAL(2, starts.front().normalNumber);
}

T_END

// issue #440 (third bullet): time-modification's normal-type/normal-dot describe only the
// normal side; there is no actual-type. So when normal-type differs from the note's own
// written type (3 eighths in the time of 2 quarters), the guessed actual duration must come
// from the note's own <type>/<dot>, not from normal-type.
TEST(guessesActualDurationFromNoteTypeNotFromNormalType, TupletReader)
{
    core::Tuplet tuplet;
    tuplet.setType(core::StartStop::start());
    // both tuplet-actual and tuplet-normal are absent; both sides must be guessed.

    core::TimeModificationGroup group;
    group.setNormalType(core::NoteTypeValue::quarter());

    core::TimeModification timeMod;
    timeMod.setActualNotes(3);
    timeMod.setNormalNotes(2);
    timeMod.setGroup(group);

    core::NoteType noteType;
    noteType.setValue(core::NoteTypeValue::eighth());

    core::Note note;
    note.setType(noteType);
    note.setTimeModification(timeMod);

    Cursor cursor{1, 480};
    TupletReader reader{tuplet, cursor, note};

    std::vector<api::TupletStart> starts;
    std::vector<api::TupletStop> stops;
    reader.parseTuplet(starts, stops);

    REQUIRE(starts.size() == 1);
    CHECK(api::DurationName::eighth == starts.front().actualDurationName);
    CHECK(api::DurationName::quarter == starts.front().normalDurationName);
}

T_END

#endif
