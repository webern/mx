// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/MusicXml.h"
#include "mx/core/generated/Document.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/Print.h"
#include "mx/core/generated/YesNo.h"

using namespace std;
using namespace mx::api;

TEST(newSystem, doesItWork)
{
    std::vector<MeasureData> measures;
    const auto addMeasure = [&]() {
        VoiceData voiceData;
        NoteData n;
        n.tickTimePosition = 0;
        n.pitchData.step = Step::c;
        n.pitchData.octave = 5;
        n.durationData.durationName = DurationName::quarter;
        n.durationData.durationTimeTicks = DEFAULT_TICKS_PER_QUARTER;
        voiceData.notes.push_back(n);
        n.tickTimePosition += DEFAULT_TICKS_PER_QUARTER;
        n.pitchData.step = Step::d;
        voiceData.notes.push_back(n);
        n.tickTimePosition += DEFAULT_TICKS_PER_QUARTER;
        n.pitchData.step = Step::e;
        voiceData.notes.push_back(n);
        n.tickTimePosition += DEFAULT_TICKS_PER_QUARTER;
        n.pitchData.step = Step::f;
        voiceData.notes.push_back(n);
        StaffData staff{};
        staff.voices.emplace(0, voiceData);
        MeasureData m;
        m.staves.push_back(staff);
        measures.push_back(m);
    };

    for (int i = 0; i < 100; ++i)
    {
        addMeasure();
    }

    PartData pd;
    pd.name = "Flute";
    pd.displayName = "Flute";
    pd.measures = measures;
    ScoreData s;
    s.parts.push_back(pd);
    LayoutData systemBreak{true};
    const auto addSystemBreak = [&](MeasureIndex measureIndex) { s.layout.emplace(measureIndex, systemBreak); };
    addSystemBreak(0);
    addSystemBreak(3);
    addSystemBreak(7);
    addSystemBreak(13);
    addSystemBreak(17);
    addSystemBreak(25);
    addSystemBreak(50);
    addSystemBreak(75);
    auto rId = fromScore(s);
    REQUIRE(rId.ok());
    const auto doc = std::move(rId).value();
    const auto &sp = doc.getCoreDocument().asScorePartwise();
    const auto &p = sp.part()[0];

    size_t index = 0;
    auto checkNewSystem = [&](size_t idx, bool expectNewSystem) {
        const auto &mdg = p.measure()[idx];
        CHECK_EQUAL(std::to_string(idx + 1), mdg.number());
        const auto &mdcSpan = mdg.musicData();
        // Find the Print element (if any) in this measure's music data.
        const mx::core::Print *printPtr = nullptr;
        for (const auto &mdc : mdcSpan)
        {
            if (mdc.isPrint())
            {
                printPtr = &mdc.asPrint();
                break;
            }
        }
        if (expectNewSystem)
        {
            REQUIRE(printPtr != nullptr);
            CHECK(printPtr->newSystem().has_value());
            if (printPtr->newSystem().has_value())
            {
                CHECK(mx::core::YesNo::Tag::yes == printPtr->newSystem()->tag());
            }
        }
        else
        {
            // For non-system-break measures, either no Print element exists, or Print
            // exists without new-system="yes".
            if (printPtr != nullptr)
            {
                CHECK(!printPtr->newSystem().has_value());
            }
        }
    };

    checkNewSystem(0, true);
    checkNewSystem(3, true);
    checkNewSystem(7, true);
    checkNewSystem(13, true);
    checkNewSystem(17, true);
    checkNewSystem(25, true);
    checkNewSystem(50, true);
    checkNewSystem(75, true);

    // check a couple of negative cases to make sure not every measure is new system
    checkNewSystem(76, false);
    checkNewSystem(4, false);
}

#endif
