// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/MusicXml.h"

#include <sstream>
#include <vector>

using namespace std;
using namespace mx::api;

namespace
{
// Round-trips a single note-attached mark of the given MarkType through the
// full serialize -> deserialize path and returns the marks read back from the
// first note. Used by the enum data-loss regression tests below.
std::vector<MarkData> roundTripMarkData(const MarkData &inMarkData)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();
    auto &voice = staff.voices[0];
    voice.notes.emplace_back();
    auto &note = voice.notes.back();
    note.noteAttachmentData.marks.push_back(inMarkData);

    auto r1 = fromScore(score);
    if (!r1.ok())
        return {};
    std::stringstream ss;
    std::move(r1).value().writeToStream(ss);
    const std::string xml = ss.str();

    std::istringstream iss{xml};
    auto r2 = MusicXml::fromStream(iss);
    if (!r2.ok())
        return {};
    const auto rd = getScore(std::move(r2).value());
    if (!rd.ok())
        return {};
    const auto &oscore = rd.value();

    const auto &onote = oscore.parts.back().measures.back().staves.back().voices.at(0).notes.back();
    return onote.noteAttachmentData.marks;
}

std::vector<MarkData> roundTripMark(MarkType inMarkType)
{
    return roundTripMarkData(MarkData{Placement::unspecified, inMarkType});
}

std::vector<MarkData> roundTripDynamic(StandardDynamic inDynamic)
{
    return roundTripMarkData(MarkData{inDynamic});
}

bool hasDynamic(const std::vector<MarkData> &marks, StandardDynamic inDynamic)
{
    for (const auto &m : marks)
    {
        if (m.markType == MarkType::dynamics && m.choice.isDynamic() && m.choice.dynamic() == inDynamic)
        {
            return true;
        }
    }
    return false;
}

bool hasMark(const std::vector<MarkData> &marks, MarkType inMarkType)
{
    for (const auto &m : marks)
    {
        if (m.markType == inMarkType)
        {
            return true;
        }
    }
    return false;
}
} // namespace

// #193 - dynamics n, pf, sfzp were silently dropped (fell back to unspecified)
// because the api enum lacked the members and standardDynamicsMap lacked the rows.
TEST(DynamicsN, MarkRoundTrip)
{
    const auto marks = roundTripDynamic(StandardDynamic::n);
    CHECK(hasDynamic(marks, StandardDynamic::n));
}

T_END;

TEST(DynamicsPf, MarkRoundTrip)
{
    const auto marks = roundTripDynamic(StandardDynamic::pf);
    CHECK(hasDynamic(marks, StandardDynamic::pf));
}

T_END;

TEST(DynamicsSfzp, MarkRoundTrip)
{
    const auto marks = roundTripDynamic(StandardDynamic::sfzp);
    CHECK(hasDynamic(marks, StandardDynamic::sfzp));
}

T_END;

// #194 - articulation soft-accent (MusicXML 3.1) had no api::MarkType member
// and articulationsMap dropped it to unspecified on read.
TEST(SoftAccent, MarkRoundTrip)
{
    const auto marks = roundTripMark(MarkType::softAccent);
    CHECK(hasMark(marks, MarkType::softAccent));
}

T_END;

// #195 - ornaments haydn and inverted-vertical-turn (MusicXML 3.1) had no
// api::MarkType members and ornamentsMap dropped them to unspecified.
TEST(OrnamentHaydn, MarkRoundTrip)
{
    const auto marks = roundTripMark(MarkType::haydn);
    CHECK(hasMark(marks, MarkType::haydn));
}

T_END;

TEST(OrnamentInvertedVerticalTurn, MarkRoundTrip)
{
    const auto marks = roundTripMark(MarkType::invertedVerticalTurn);
    CHECK(hasMark(marks, MarkType::invertedVerticalTurn));
}

T_END;

// #196 - technical marks brass-bend, flip, smear, open, half-muted,
// harmon-mute, golpe (MusicXML 3.1-4.0) had no api::MarkType members and
// technicalMarkMap dropped them to unspecified. These carry no payload.
TEST(TechnicalBrassBend, MarkRoundTrip)
{
    const auto marks = roundTripMark(MarkType::brassBend);
    CHECK(hasMark(marks, MarkType::brassBend));
}

T_END;

TEST(TechnicalFlip, MarkRoundTrip)
{
    const auto marks = roundTripMark(MarkType::flip);
    CHECK(hasMark(marks, MarkType::flip));
}

T_END;

TEST(TechnicalSmear, MarkRoundTrip)
{
    const auto marks = roundTripMark(MarkType::smear);
    CHECK(hasMark(marks, MarkType::smear));
}

T_END;

TEST(TechnicalOpen, MarkRoundTrip)
{
    const auto marks = roundTripMark(MarkType::open);
    CHECK(hasMark(marks, MarkType::open));
}

T_END;

TEST(TechnicalHalfMuted, MarkRoundTrip)
{
    const auto marks = roundTripMark(MarkType::halfMuted);
    CHECK(hasMark(marks, MarkType::halfMuted));
}

T_END;

TEST(TechnicalHarmonMute, MarkRoundTrip)
{
    const auto marks = roundTripMark(MarkType::harmonMute);
    CHECK(hasMark(marks, MarkType::harmonMute));
}

T_END;

TEST(TechnicalGolpe, MarkRoundTrip)
{
    const auto marks = roundTripMark(MarkType::golpe);
    CHECK(hasMark(marks, MarkType::golpe));
}

T_END;

// #181 - fermata 4.0 shapes double-angled, double-square, double-dot,
// half-curve, curlew had no api::MarkType members and fermataMap dropped them
// to unspecified.
TEST(FermataDoubleAngled, MarkRoundTrip)
{
    const auto marks = roundTripMark(MarkType::fermataDoubleAngled);
    CHECK(hasMark(marks, MarkType::fermataDoubleAngled));
}

T_END;

TEST(FermataDoubleSquare, MarkRoundTrip)
{
    const auto marks = roundTripMark(MarkType::fermataDoubleSquare);
    CHECK(hasMark(marks, MarkType::fermataDoubleSquare));
}

T_END;

TEST(FermataDoubleDot, MarkRoundTrip)
{
    const auto marks = roundTripMark(MarkType::fermataDoubleDot);
    CHECK(hasMark(marks, MarkType::fermataDoubleDot));
}

T_END;

TEST(FermataHalfCurve, MarkRoundTrip)
{
    const auto marks = roundTripMark(MarkType::fermataHalfCurve);
    CHECK(hasMark(marks, MarkType::fermataHalfCurve));
}

T_END;

TEST(FermataCurlew, MarkRoundTrip)
{
    const auto marks = roundTripMark(MarkType::fermataCurlew);
    CHECK(hasMark(marks, MarkType::fermataCurlew));
}

T_END;

TEST(ArpeggiateAttributes, MarkRoundTrip)
{
    MarkData mark{Placement::unspecified, MarkType::arpeggiateUp};
    ArpeggiateMarkData payload{};
    payload.number = 2;
    payload.unbroken = Bool::yes;
    payload.id = "arp-1";
    mark.choice = payload;

    const auto marks = roundTripMarkData(mark);
    REQUIRE(marks.size() == 1);
    CHECK(marks.front().markType == MarkType::arpeggiateUp);
    REQUIRE(marks.front().choice.isArpeggiate());
    CHECK(payload == marks.front().choice.arpeggiate());
}

T_END;

TEST(NonArpeggiateAttributes, MarkRoundTrip)
{
    MarkData mark{Placement::unspecified, MarkType::nonArpeggiate};
    NonArpeggiateMarkData payload{};
    payload.placement = NonArpeggiatePlacement::bottom;
    payload.number = 1;
    payload.id = "nonarp-1";
    mark.choice = payload;

    const auto marks = roundTripMarkData(mark);
    REQUIRE(marks.size() == 1);
    CHECK(marks.front().markType == MarkType::nonArpeggiate);
    REQUIRE(marks.front().choice.isNonArpeggiate());
    CHECK(payload == marks.front().choice.nonArpeggiate());
}

T_END;

TEST(HarmonicArtificialPair, MarkRoundTrip)
{
    // The two notes of an artificial harmonic each state what they are: the stopped note carries
    // the base pitch, the diamond-notehead note the touching pitch.
    MarkData stopped{Placement::unspecified, MarkType::harmonic};
    HarmonicMarkData stoppedPayload{};
    stoppedPayload.kind = HarmonicKind::artificial;
    stoppedPayload.pitch = HarmonicPitch::basePitch;
    stopped.choice = stoppedPayload;

    const auto stoppedMarks = roundTripMarkData(stopped);
    REQUIRE(stoppedMarks.size() == 1);
    CHECK(stoppedMarks.front().markType == MarkType::harmonic);
    REQUIRE(stoppedMarks.front().choice.isHarmonic());
    CHECK(stoppedPayload == stoppedMarks.front().choice.harmonic());

    MarkData touched{Placement::unspecified, MarkType::harmonic};
    HarmonicMarkData touchedPayload{};
    touchedPayload.kind = HarmonicKind::artificial;
    touchedPayload.pitch = HarmonicPitch::touchingPitch;
    touched.choice = touchedPayload;

    const auto touchedMarks = roundTripMarkData(touched);
    REQUIRE(touchedMarks.size() == 1);
    REQUIRE(touchedMarks.front().choice.isHarmonic());
    CHECK(touchedPayload == touchedMarks.front().choice.harmonic());
}

T_END;

TEST(HarmonicKindAndPitchCombinations, MarkRoundTrip)
{
    const HarmonicKind kinds[] = {HarmonicKind::unspecified, HarmonicKind::natural, HarmonicKind::artificial};
    const HarmonicPitch pitches[] = {HarmonicPitch::unspecified, HarmonicPitch::basePitch, HarmonicPitch::touchingPitch,
                                     HarmonicPitch::soundingPitch};

    for (const auto kind : kinds)
    {
        for (const auto pitch : pitches)
        {
            if (kind == HarmonicKind::unspecified && pitch == HarmonicPitch::unspecified)
            {
                continue; // covered by HarmonicDefaultsToNoPayload below
            }

            MarkData mark{Placement::unspecified, MarkType::harmonic};
            HarmonicMarkData payload{};
            payload.kind = kind;
            payload.pitch = pitch;
            mark.choice = payload;

            const auto marks = roundTripMarkData(mark);
            REQUIRE(marks.size() == 1);
            REQUIRE(marks.front().choice.isHarmonic());
            CHECK(payload == marks.front().choice.harmonic());
        }
    }
}

T_END;

TEST(HarmonicDefaultsToNoPayload, MarkRoundTrip)
{
    // An undecorated <harmonic/> must not acquire a natural/artificial or pitch value, and must
    // still round-trip for callers written before the payload existed.
    const auto marks = roundTripMark(MarkType::harmonic);
    REQUIRE(marks.size() == 1);
    CHECK(marks.front().markType == MarkType::harmonic);
    CHECK(marks.front().choice.isNone());
}

T_END;

TEST(CaesuraEmpty, MarkRoundTrip)
{
    // The common empty form <caesura/> must not acquire a "normal" text value on write.
    const auto marks = roundTripMark(MarkType::caesura);
    CHECK(hasMark(marks, MarkType::caesura));
}

T_END;

TEST(CaesuraVariants, MarkRoundTrip)
{
    CHECK(hasMark(roundTripMark(MarkType::caesuraNormal), MarkType::caesuraNormal));
    CHECK(hasMark(roundTripMark(MarkType::caesuraThick), MarkType::caesuraThick));
    CHECK(hasMark(roundTripMark(MarkType::caesuraShort), MarkType::caesuraShort));
    CHECK(hasMark(roundTripMark(MarkType::caesuraCurved), MarkType::caesuraCurved));
    CHECK(hasMark(roundTripMark(MarkType::caesuraSingle), MarkType::caesuraSingle));
}

T_END;

// Returns the diagnostics carrying exactly the given message.
inline std::vector<Diagnostic> markRoundTripDiagnosticsWithMessage(const Diagnostics &diagnostics,
                                                                   const std::string &message)
{
    std::vector<Diagnostic> out;
    for (const auto &diagnostic : diagnostics.all())
    {
        if (diagnostic.message == message)
        {
            out.push_back(diagnostic);
        }
    }
    return out;
}

// Writes a one-note score holding the given mark and returns the XML, collecting the diagnostics
// the write reported.
inline std::string markRoundTripWriteWithDiagnostics(const MarkData &inMarkData, Diagnostics &outDiagnostics)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &note = score.parts.back().measures.emplace_back().staves.emplace_back().voices[0].notes.emplace_back();
    note.noteAttachmentData.marks.push_back(inMarkData);
    auto document = fromScore(score, outDiagnostics);
    if (!document.ok())
        return {};
    std::stringstream ss;
    std::move(document).value().writeToStream(ss);
    return ss.str();
}

TEST(OrnamentAccidentals, MarkRoundTrip)
{
    OrnamentAccidental sharp;
    sharp.accidental = Accidental::sharp;
    sharp.positionData.placement = Placement::above;
    sharp.positionData.isDefaultYSpecified = true;
    sharp.positionData.defaultY = 12.5;

    MarkData trill{Placement::unspecified, MarkType::trillMark};
    trill.choice = OrnamentMarkData{{sharp}};
    const auto trillMarks = roundTripMarkData(trill);
    REQUIRE(trillMarks.size() == 1);
    CHECK(trillMarks.front().markType == MarkType::trillMark);
    REQUIRE(trillMarks.front().choice.isOrnament());
    CHECK(trill.choice == trillMarks.front().choice);

    OrnamentAccidental natural;
    natural.accidental = Accidental::natural;
    natural.positionData.placement = Placement::below;
    OrnamentAccidental flat;
    flat.accidental = Accidental::flat;

    MarkData mordent{Placement::above, MarkType::mordent};
    mordent.choice = OrnamentMarkData{{natural, flat}};
    const auto mordentMarks = roundTripMarkData(mordent);
    REQUIRE(mordentMarks.size() == 1);
    CHECK(mordentMarks.front().markType == MarkType::mordent);
    CHECK(Placement::above == mordentMarks.front().positionData.placement);
    CHECK(mordent.choice == mordentMarks.front().choice);
}

T_END;

// An accidental left at none is not drawn, so it is not written, and the write reports the loss.
// The ornament's other accidentals are still written.
TEST(OrnamentAccidentalOfNoneIsDropped, MarkRoundTrip)
{
    OrnamentAccidental none;
    OrnamentAccidental sharp;
    sharp.accidental = Accidental::sharp;

    MarkData trill{Placement::unspecified, MarkType::trillMark};
    trill.choice = OrnamentMarkData{{none, sharp}};

    Diagnostics diagnostics;
    const auto xml = markRoundTripWriteWithDiagnostics(trill, diagnostics);
    CHECK(xml.find(">sharp</accidental-mark>") != std::string::npos);
    const auto dropped =
        markRoundTripDiagnosticsWithMessage(diagnostics, "an ornament accidental of none is not written");
    REQUIRE(dropped.size() == 1);
    CHECK(Severity::error == dropped.front().severity);
    CHECK(DiagnosticCode::droppedData == dropped.front().code);

    const auto marks = roundTripMarkData(trill);
    REQUIRE(marks.size() == 1);
    CHECK(MarkDataChoice{OrnamentMarkData{{sharp}}} == marks.front().choice);
}

T_END;

// Ornament accidentals on a mark that is not written in <ornaments> have nowhere to go.
TEST(OrnamentAccidentalsOnANonOrnamentAreDropped, MarkRoundTrip)
{
    OrnamentAccidental sharp;
    sharp.accidental = Accidental::sharp;
    MarkData accent{Placement::unspecified, MarkType::accent};
    accent.choice = OrnamentMarkData{{sharp}};

    Diagnostics diagnostics;
    const auto xml = markRoundTripWriteWithDiagnostics(accent, diagnostics);
    CHECK(xml.find("<accent") != std::string::npos);
    CHECK(xml.find("<accidental-mark") == std::string::npos);
    const auto dropped = markRoundTripDiagnosticsWithMessage(
        diagnostics, "ornament accidentals on a mark that cannot hold them are not written");
    REQUIRE(dropped.size() == 1);
    CHECK(Severity::error == dropped.front().severity);
    CHECK(DiagnosticCode::droppedData == dropped.front().code);
}

T_END;

// A tremolo or an other-ornament cannot hold ornament accidentals, so they are not written after
// it; otherwise they would read back as standalone accidental marks.
TEST(OrnamentAccidentalsOnATremoloOrOtherOrnamentAreDropped, MarkRoundTrip)
{
    OrnamentAccidental sharp;
    sharp.accidental = Accidental::sharp;

    for (const auto markType : {MarkType::tremoloSingleThree, MarkType::otherOrnament})
    {
        MarkData mark{Placement::unspecified, markType};
        mark.choice = OrnamentMarkData{{sharp}};

        Diagnostics diagnostics;
        const auto xml = markRoundTripWriteWithDiagnostics(mark, diagnostics);
        CHECK(xml.find("<ornaments>") != std::string::npos);
        CHECK(xml.find("<accidental-mark") == std::string::npos);
        const auto dropped = markRoundTripDiagnosticsWithMessage(
            diagnostics, "ornament accidentals on a mark that cannot hold them are not written");
        REQUIRE(dropped.size() == 1);
        CHECK(Severity::error == dropped.front().severity);
        CHECK(DiagnosticCode::droppedData == dropped.front().code);
    }
}

T_END;

// Every ornament that can hold accidentals writes them back and reads them again.
TEST(EveryOrnamentKeepsItsAccidentals, MarkRoundTrip)
{
    OrnamentAccidental sharp;
    sharp.accidental = Accidental::sharp;
    sharp.positionData.placement = Placement::above;

    for (const auto markType :
         {MarkType::trillMark, MarkType::turn, MarkType::delayedTurn, MarkType::invertedTurn,
          MarkType::delayedInvertedTurn, MarkType::verticalTurn, MarkType::invertedVerticalTurn, MarkType::shake,
          MarkType::mordent, MarkType::invertedMordent, MarkType::schleifer, MarkType::haydn})
    {
        MarkData mark{Placement::unspecified, markType};
        mark.choice = OrnamentMarkData{{sharp}};
        const auto marks = roundTripMarkData(mark);
        REQUIRE(marks.size() == 1);
        CHECK(markType == marks.front().markType);
        CHECK(mark.choice == marks.front().choice);
    }
}

T_END;

// An ornament without accidentals needs no payload, so it is none however it was built.
TEST(EmptyOrnamentCollapses, MarkRoundTrip)
{
    const MarkDataChoice empty{OrnamentMarkData{}};
    CHECK(empty.isNone());
    CHECK(!empty.isOrnament());
    CHECK(MarkDataChoice::Kind::none == empty.kind());
    CHECK(empty == MarkDataChoice{});

    MarkData trill{Placement::unspecified, MarkType::trillMark};
    trill.choice = OrnamentMarkData{};
    const auto marks = roundTripMarkData(trill);
    REQUIRE(marks.size() == 1);
    CHECK(marks.front().choice.isNone());
}

T_END;

// Asking for the wrong kind returns a default value, and every field of the payload takes part in
// equality.
TEST(OrnamentChoiceAccessorsAndEquality, MarkRoundTrip)
{
    OrnamentAccidental sharp;
    sharp.accidental = Accidental::sharp;
    const MarkDataChoice ornament{OrnamentMarkData{{sharp}}};
    CHECK(MarkDataChoice::Kind::ornament == ornament.kind());
    CHECK(ornament.harmonic() == HarmonicMarkData{});
    CHECK(MarkDataChoice{}.ornament().accidentals.empty());
    CHECK(MarkDataChoice{HarmonicMarkData{}}.ornament().accidentals.empty());

    OrnamentAccidental flat = sharp;
    flat.accidental = Accidental::flat;
    CHECK(sharp != flat);

    OrnamentAccidental below = sharp;
    below.positionData.placement = Placement::below;
    CHECK(sharp != below);

    CHECK(OrnamentMarkData{{sharp}} != OrnamentMarkData{{sharp, sharp}});
    CHECK(ornament != MarkDataChoice{OrnamentMarkData{{flat}}});
    CHECK(ornament != MarkDataChoice{OrnamentMarkData{{below}}});
}

T_END;

#endif
