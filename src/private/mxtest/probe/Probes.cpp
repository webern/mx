// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Compile-time negative probes: each PROBE number selects one construct
// that MUST NOT compile -- the unrepresentability
// half of valid-by-construction, verified by `make probe-cpp`, which
// asserts that compilation fails for every PROBE > 0 and succeeds for the
// PROBE=0 control (proving the probe failures are not include/setup rot).

#include "mx/core/Token.h"
#include "mx/core/generated/AboveBelow.h"
#include "mx/core/generated/Color.h"
#include "mx/core/generated/Note.h"
#include "mx/core/generated/NoteChoice.h"
#include "mx/core/generated/Pitch.h"
#include "mx/core/generated/ScorePart.h"

#include <string>
#include <variant>

#ifndef PROBE
#define PROBE 0
#endif

using namespace mx::core;

int main()
{
#if PROBE == 0
    // Control: the public API spellings these probes subvert.
    AboveBelow enumValue = AboveBelow::above();
    (void)enumValue;
    Color color{0xFF, 0x00, 0x00};
    (void)color;
    Note note;
    (void)note.addBeam(Beam{});
#elif PROBE == 1
    // An enum wrapper cannot be constructed from a raw int.
    AboveBelow v{0};
    (void)v;
#elif PROBE == 2
    // ...nor from its own Tag (the named factories are the only door, so a
    // static_cast-forged Tag cannot enter).
    AboveBelow v{AboveBelow::Tag::above};
    (void)v;
#elif PROBE == 3
    // A bounded repeated member exposes no mutable container: the span is
    // read-only and the only append is the Result-returning add.
    Note note;
    note.beam()[0] = Beam{};
#elif PROBE == 4
    // A choice's variant storage is private; alternatives cannot be reached
    // (or corrupted) positionally from outside.
    NoteChoice c;
    (void)std::get<0>(c);
#elif PROBE == 5
    // Color has no lexical constructor: a malformed hex string has no door.
    Color c{std::string{"#NOTHEX"}};
    (void)c;
#elif PROBE == 6
    // Sequence fields have no public members: a required child cannot be
    // overwritten except through the typed setter.
    Pitch p;
    p.m_step = Step::c();
#elif PROBE == 7
    // An xs:ID attribute is a Token, not a raw string: a bare string
    // literal cannot enter (Token's converting constructor is explicit, so
    // the lexically-unchecked door the audit found is closed at compile
    // time). The caller must say Token{...}, which repairs.
    ScorePart sp;
    sp.setID("has spaces and #bad!");
#endif
    return 0;
}
