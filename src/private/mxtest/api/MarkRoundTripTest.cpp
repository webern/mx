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

#endif
