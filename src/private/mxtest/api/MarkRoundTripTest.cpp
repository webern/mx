// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"

#include <sstream>
#include <vector>

using namespace std;
using namespace mx::api;

namespace
{
// Round-trips a single note-attached mark of the given MarkType through the
// full serialize -> deserialize path and returns the marks read back from the
// first note. Used by the enum data-loss regression tests below.
std::vector<MarkData> roundTripMark(MarkType inMarkType)
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
    note.noteAttachmentData.marks.emplace_back(Placement::unspecified, inMarkType);

    auto &mgr = DocumentManager::getInstance();
    const auto r1 = mgr.createFromScore(score);
    if (!r1.ok())
        return {};
    auto docId = r1.value();
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();

    std::istringstream iss{xml};
    const auto r2 = mgr.createFromStream(iss);
    if (!r2.ok())
        return {};
    docId = r2.value();
    const auto rd = mgr.getData(docId);
    mgr.destroyDocument(docId);
    if (!rd.ok())
        return {};
    const auto &oscore = rd.value();

    const auto &onote = oscore.parts.back().measures.back().staves.back().voices.at(0).notes.back();
    return onote.noteAttachmentData.marks;
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
// because the api enum lacked the members and dynamicsMap lacked the rows.
TEST(DynamicsN, MarkRoundTrip)
{
    const auto marks = roundTripMark(MarkType::n);
    CHECK(hasMark(marks, MarkType::n));
}

T_END;

TEST(DynamicsPf, MarkRoundTrip)
{
    const auto marks = roundTripMark(MarkType::pf);
    CHECK(hasMark(marks, MarkType::pf));
}

T_END;

TEST(DynamicsSfzp, MarkRoundTrip)
{
    const auto marks = roundTripMark(MarkType::sfzp);
    CHECK(hasMark(marks, MarkType::sfzp));
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

#endif
