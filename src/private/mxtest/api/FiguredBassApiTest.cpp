// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/ScoreData.h"
#include "mxtest/api/RoundTrip.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;

namespace
{
// Builds a single-part, single-measure score: one quarter note on beat 1 with a figured-bass
// direction attached at the same tick. The caller fills the returned direction's figuredBasses.
ScoreData makeScoreWithFiguredBass(FiguredBassData figuredBass)
{
    ScoreData score;
    score.ticksPerQuarter = 4;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.name = "P1";
    part.uniqueId = "P1";
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();

    NoteData note{};
    note.pitchData.step = Step::c;
    note.pitchData.octave = 3;
    note.durationData.durationTimeTicks = 4;
    note.durationData.durationName = DurationName::quarter;
    note.tickTimePosition = 0;
    staff.voices[0].notes.push_back(note);

    DirectionData direction{};
    direction.tickTimePosition = 0;
    direction.isStaffValueSpecified = true;
    direction.placement = Placement::below;
    direction.figuredBasses.emplace_back(std::move(figuredBass));
    staff.directions.emplace_back(std::move(direction));

    return score;
}

const FiguredBassData *firstFiguredBass(const ScoreData &score)
{
    const auto &staff = score.parts.front().measures.front().staves.front();
    for (const auto &dir : staff.directions)
    {
        if (!dir.figuredBasses.empty())
        {
            return &dir.figuredBasses.front();
        }
    }
    return nullptr;
}
} // namespace

TEST(figuredBassFiguresRoundTrip, FiguredBassApi)
{
    FiguredBassData fb{};
    FigureData f0{};
    f0.figureNumber = "6";
    FigureData f1{};
    f1.prefix = "flat";
    f1.figureNumber = "5";
    f1.suffix = "slash";
    fb.figures.push_back(f0);
    fb.figures.push_back(f1);

    const auto score = makeScoreWithFiguredBass(fb);
    const auto out = mxtest::roundTrip(score);

    const auto *outFb = firstFiguredBass(out);
    REQUIRE(outFb != nullptr);
    REQUIRE(outFb->figures.size() == 2);
    CHECK_EQUAL("6", outFb->figures.at(0).figureNumber);
    CHECK_EQUAL("", outFb->figures.at(0).prefix);
    CHECK_EQUAL("flat", outFb->figures.at(1).prefix);
    CHECK_EQUAL("5", outFb->figures.at(1).figureNumber);
    CHECK_EQUAL("slash", outFb->figures.at(1).suffix);
}

T_END;

TEST(figuredBassParenthesesAndDurationRoundTrip, FiguredBassApi)
{
    FiguredBassData fb{};
    FigureData f0{};
    f0.figureNumber = "7";
    fb.figures.push_back(f0);
    fb.parentheses = Bool::yes;
    fb.durationTimeTicks = 4;

    const auto score = makeScoreWithFiguredBass(fb);
    const auto out = mxtest::roundTrip(score);

    const auto *outFb = firstFiguredBass(out);
    REQUIRE(outFb != nullptr);
    CHECK(Bool::yes == outFb->parentheses);
    CHECK_EQUAL(4, outFb->durationTimeTicks);
    REQUIRE(outFb->figures.size() == 1);
    CHECK_EQUAL("7", outFb->figures.at(0).figureNumber);
}

T_END;

TEST(figuredBassEmptyFiguresOmitted, FiguredBassApi)
{
    // A FiguredBassData carrying no figures must not produce a <figured-bass> on output. The core
    // figure list is never-empty (OneOrMore), so emitting one anyway would fabricate a stray empty
    // <figure/> that reads back as a one-figure figured-bass -- a round-trip that invents content.
    FiguredBassData fb{};

    const auto score = makeScoreWithFiguredBass(fb);
    const auto out = mxtest::roundTrip(score);

    CHECK(firstFiguredBass(out) == nullptr);
}

T_END;

#endif
