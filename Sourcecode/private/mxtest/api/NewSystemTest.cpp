// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mx/core/Document.h"

using namespace std;
using namespace mx::api;

TEST( newSystem, doesItWork )
{
    std::vector<MeasureData> measures;
    const auto addMeasure = [&]()
    {
        VoiceData voiceData;
        NoteData n;
        n.tickTimePosition = 0;
        n.pitchData.step = Step::c;
        n.pitchData.octave = 5;
        n.durationData.durationName = DurationName::quarter;
        n.durationData.durationTimeTicks = DEFAULT_TICKS_PER_QUARTER;
        voiceData.notes.push_back( n );
        n.tickTimePosition += DEFAULT_TICKS_PER_QUARTER;
        n.pitchData.step = Step::d;
        voiceData.notes.push_back( n );
        n.tickTimePosition += DEFAULT_TICKS_PER_QUARTER;
        n.pitchData.step = Step::e;
        voiceData.notes.push_back( n );
        n.tickTimePosition += DEFAULT_TICKS_PER_QUARTER;
        n.pitchData.step = Step::f;
        voiceData.notes.push_back( n );
        StaffData staff{};
        staff.voices.emplace( 0, voiceData );
        MeasureData m;
        m.staves.push_back( staff );
        measures.push_back( m );
    };

    for( int i = 0; i < 100; ++i )
    {
        addMeasure();
    }

    PartData pd;
    pd.name = "Flute";
    pd.displayName = "Flute";
    pd.measures = measures;
    ScoreData s;
    s.parts.push_back( pd );
    LayoutData systemBreak{true };
    const auto addSystemBreak = [&]( MeasureIndex measureIndex )
    {
        s.layout.emplace( measureIndex, systemBreak );
    };
    addSystemBreak( 0 );
    addSystemBreak( 3 );
    addSystemBreak( 7 );
    addSystemBreak( 13 );
    addSystemBreak( 17 );
    addSystemBreak( 25 );
    addSystemBreak( 50 );
    addSystemBreak( 75 );
    auto& docMgr = DocumentManager::getInstance();
    const auto id = docMgr.createFromScore( s );
    const auto doc = docMgr.getDocument( id );
    docMgr.destroyDocument( id );
    const auto sp = doc->getScorePartwise();
    const auto p = sp->getPartwisePartSet().at( 0 );

    size_t index = 0;
    auto mdg = p->getPartwiseMeasureSet().at( index );
    CHECK( mdg->getAttributes()->number.getValue() == std::to_string( index + 1 ) );
    auto print = mdg->getMusicDataGroup()->getMusicDataChoiceSet().at( 0 )->getPrint();
    CHECK( print->getAttributes()->hasNewSystem );
    CHECK( print->getAttributes()->newSystem == mx::core::YesNo::yes );

    index = 3;
    mdg = p->getPartwiseMeasureSet().at( index );
    CHECK( mdg->getAttributes()->number.getValue() == std::to_string( index + 1 ) );
    print = mdg->getMusicDataGroup()->getMusicDataChoiceSet().at( 0 )->getPrint();
    CHECK( print->getAttributes()->hasNewSystem );
    CHECK( print->getAttributes()->newSystem == mx::core::YesNo::yes );

    index = 7;
    mdg = p->getPartwiseMeasureSet().at( index );
    CHECK( mdg->getAttributes()->number.getValue() == std::to_string( index + 1 ) );
    print = mdg->getMusicDataGroup()->getMusicDataChoiceSet().at( 0 )->getPrint();
    CHECK( print->getAttributes()->hasNewSystem );
    CHECK( print->getAttributes()->newSystem == mx::core::YesNo::yes );

    index = 13;
    mdg = p->getPartwiseMeasureSet().at( index );
    CHECK( mdg->getAttributes()->number.getValue() == std::to_string( index + 1 ) );
    print = mdg->getMusicDataGroup()->getMusicDataChoiceSet().at( 0 )->getPrint();
    CHECK( print->getAttributes()->hasNewSystem );
    CHECK( print->getAttributes()->newSystem == mx::core::YesNo::yes );

    index = 17;
    mdg = p->getPartwiseMeasureSet().at( index );
    CHECK( mdg->getAttributes()->number.getValue() == std::to_string( index + 1 ) );
    print = mdg->getMusicDataGroup()->getMusicDataChoiceSet().at( 0 )->getPrint();
    CHECK( print->getAttributes()->hasNewSystem );
    CHECK( print->getAttributes()->newSystem == mx::core::YesNo::yes );

    index = 25;
    mdg = p->getPartwiseMeasureSet().at( index );
    CHECK( mdg->getAttributes()->number.getValue() == std::to_string( index + 1 ) );
    print = mdg->getMusicDataGroup()->getMusicDataChoiceSet().at( 0 )->getPrint();
    CHECK( print->getAttributes()->hasNewSystem );
    CHECK( print->getAttributes()->newSystem == mx::core::YesNo::yes );

    index = 50;
    mdg = p->getPartwiseMeasureSet().at( index );
    CHECK( mdg->getAttributes()->number.getValue() == std::to_string( index + 1 ) );
    print = mdg->getMusicDataGroup()->getMusicDataChoiceSet().at( 0 )->getPrint();
    CHECK( print->getAttributes()->hasNewSystem );
    CHECK( print->getAttributes()->newSystem == mx::core::YesNo::yes );

    index = 75;
    mdg = p->getPartwiseMeasureSet().at( index );
    CHECK( mdg->getAttributes()->number.getValue() == std::to_string( index + 1 ) );
    print = mdg->getMusicDataGroup()->getMusicDataChoiceSet().at( 0 )->getPrint();
    CHECK( print->getAttributes()->hasNewSystem );
    CHECK( print->getAttributes()->newSystem == mx::core::YesNo::yes );

    // check a couple of negative cases  to make sure not every measure is new system

    index = 76;
    mdg = p->getPartwiseMeasureSet().at( index );
    CHECK( mdg->getAttributes()->number.getValue() == std::to_string( index + 1 ) );
    print = mdg->getMusicDataGroup()->getMusicDataChoiceSet().at( 0 )->getPrint();
    CHECK( !print->getAttributes()->hasNewSystem );

    index = 4;
    mdg = p->getPartwiseMeasureSet().at( index );
    CHECK( mdg->getAttributes()->number.getValue() == std::to_string( index + 1 ) );
    print = mdg->getMusicDataGroup()->getMusicDataChoiceSet().at( 0 )->getPrint();
    CHECK( !print->getAttributes()->hasNewSystem );
}

#endif