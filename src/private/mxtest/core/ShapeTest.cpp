// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Shape tests for the generated complex types: sequence order
// unrepresentable, variant exclusivity, maxOccurs rejection,
// default-construction validity, and the strict-structure parse boundary
// (wrong order / unknown names / missing required content reject with the
// right ErrorCode).

#include "cpul/cpulTestHarness.h"

#include "mx/core/Error.h"
#include "mx/core/Xml.h"
#include "mx/core/generated/Beam.h"
#include "mx/core/generated/BeamValue.h"
#include "mx/core/generated/Dynamics.h"
#include "mx/core/generated/FullNoteGroup.h"
#include "mx/core/generated/HarmonicChoice.h"
#include "mx/core/generated/Key.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/Note.h"
#include "mx/core/generated/PartwiseMeasure.h"
#include "mx/core/generated/Pitch.h"
#include "mx/core/generated/Step.h"

#include "pugixml/pugixml.hpp"

#include <string>

using namespace mx::core;

namespace
{

// Serialize through a scratch document and return the XML text, compact.
std::string toXml(const Pitch &p)
{
    pugi::xml_document doc;
    serializePitch(p, doc, "pitch");

    struct Writer : pugi::xml_writer
    {
        std::string out;

        void write(const void *data, size_t size) override
        {
            out.append(static_cast<const char *>(data), size);
        }
    } w;

    doc.save(w, "", pugi::format_raw | pugi::format_no_declaration);
    return w.out;
}

ErrorCode parsePitchExpectingError(const char *xml)
{
    pugi::xml_document doc;
    CHECK(doc.load_string(xml));
    try
    {
        parsePitch(doc.first_child());
    }
    catch (const ParseError &e)
    {
        return e.error().code;
    }
    FAIL("expected a ParseError");
    return ErrorCode::invalidDocument;
}

} // namespace

TEST(SequenceOrderIsStructural, Shapes)
{
    // Fields are written in schema order regardless of mutation order: a
    // wrong order is unrepresentable.
    Pitch p;
    p.setOctave(Octave{4});
    p.setAlter(Semitones{Decimal{1.0}});
    p.setStep(Step::c());
    CHECK_EQUAL(std::string{"<pitch><step>C</step><alter>1</alter><octave>4</octave></pitch>"}, toXml(p));
}

TEST(DefaultConstructionIsValid, Shapes)
{
    // The natural zero serializes schema-valid content recursively.
    CHECK_EQUAL(std::string{"<pitch><step>A</step><octave>0</octave></pitch>"}, toXml(Pitch{}));
}

TEST(VariantExclusivity, Shapes)
{
    // Exactly one alternative by construction; duplicate alternative TYPES
    // (Empty/Empty) are distinguished positionally, never by type.
    HarmonicChoice natural = HarmonicChoice::natural(Empty{});
    HarmonicChoice artificial = HarmonicChoice::artificial(Empty{});
    CHECK(natural.isNatural());
    CHECK(!natural.isArtificial());
    CHECK(natural.kind() == HarmonicChoice::Kind::natural);
    CHECK(natural != artificial); // index discriminates equal payloads
}

TEST(MaxOccursAppendRejected, Shapes)
{
    Note note;
    for (int i = 0; i < 8; ++i)
    {
        CHECK(note.addBeam(Beam{}).ok());
    }
    const auto rejected = note.addBeam(Beam{});
    CHECK(!rejected.ok());
    CHECK(rejected.error().code == ErrorCode::tooManyElements);
    CHECK_EQUAL(std::size_t{8}, note.beam().size());
}

TEST(NullableChoiceNaturalZeroIsEmpty, Shapes)
{
    // key's choice can be satisfied empty (non-traditional-key*): its
    // natural zero is the empty alternative, so a default <key/> is valid.
    Key key;
    CHECK(key.choice().isNonTraditionalKey());
    CHECK_EQUAL(std::size_t{0}, key.choice().asNonTraditionalKey().size());
}

TEST(PresenceFlag, Shapes)
{
    FullNoteGroup fn;
    CHECK(!fn.chord());
    fn.setChord(true);
    CHECK(fn.chord());
}

TEST(BoxedChoiceValueSemantics, Shapes)
{
    MusicDataChoice a = MusicDataChoice::note(Note{});
    MusicDataChoice b = a; // deep copy, value semantics
    CHECK(a == b);
    Note louder;
    louder.setDynamics(NonNegativeDecimal{Decimal{80.0}});
    b = MusicDataChoice::note(std::move(louder));
    CHECK(a != b);
    CHECK(a.isNote());
    // The copy did not alias: mutating b's pointee left a untouched.
    CHECK(!a.asNote().dynamics());
}

TEST(StrictStructureParse, Shapes)
{
    // Wrong sequence order: known name in the wrong position.
    CHECK(parsePitchExpectingError("<pitch><octave>4</octave><step>C</step></pitch>") == ErrorCode::wrongElementOrder);
    // Unknown element and attribute names are errors, never data.
    CHECK(parsePitchExpectingError("<pitch><step>C</step><octave>4</octave><banana/></pitch>") ==
          ErrorCode::unknownElement);
    CHECK(parsePitchExpectingError("<pitch banana=\"1\"><step>C</step><octave>4</octave></pitch>") ==
          ErrorCode::unknownAttribute);
    // Missing required child (octave alone: step truly absent).
    CHECK(parsePitchExpectingError("<pitch><octave>4</octave></pitch>") == ErrorCode::missingRequiredElement);
    CHECK(parsePitchExpectingError("<pitch></pitch>") == ErrorCode::missingRequiredElement);
    // Duplicate of a maxOccurs=1 element.
    CHECK(parsePitchExpectingError("<pitch><step>C</step><step>D</step><octave>4</octave></pitch>") ==
          ErrorCode::wrongElementOrder);
    // A round trip of valid input parses cleanly (values lenient).
    pugi::xml_document doc;
    CHECK(doc.load_string("<pitch><step>G</step><alter>-1</alter><octave>11</octave></pitch>"));
    const Pitch p = parsePitch(doc.first_child());
    CHECK(p.step() == Step::g());
    CHECK_EQUAL(9, p.octave().value()); // out-of-range value clamps
}
