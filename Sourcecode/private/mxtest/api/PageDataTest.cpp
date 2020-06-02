// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "cpul/cpulTestHarness.h"
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "mx/api/ScoreData.h"
#include "mx/api/DocumentManager.h"

using namespace std;
using namespace mx::api;

// Make sure the equality macros (i.e. operator==) works correctly with std::optional
TEST( EqualityOfOptional, PitchData )
{
    PageData a;
    PageData b;
    a.pageLayoutData = PageLayoutData{};
    CHECK( a.pageLayoutData );
    CHECK( !b.pageLayoutData );
    CHECK( a != b );
    PageData c;
    c.pageLayoutData = PageLayoutData{};
    c.pageLayoutData->pageWidth = 1;
    PageData d;
    d.pageLayoutData = PageLayoutData{};
    d.pageLayoutData->pageWidth = 2;
    CHECK( c != d );
    d.pageLayoutData->pageWidth = 1;
    CHECK( c == d );
    d.pageNumber = "foo";
    CHECK( c != d );
    c.pageNumber = "bar";
    CHECK( c != d );
    d.pageNumber = "bar";
    CHECK( c == d );
}
T_END;

namespace pageDataTest
{
    inline mx::api::ScoreData makeSomeBoringMusic(int inNumMeasures)
    {
        const auto ticksPerQuarter = 1024;
        const auto whole = ticksPerQuarter * 4;
        PartData p;
        for( int i=0; i < inNumMeasures; ++i )
        {
            NoteData n;
            n.pitchData.step = static_cast<Step>( i % 7 );
            const int accidentalSurprise = i % 3;
            if( accidentalSurprise == 1 )
            {
                n.pitchData.accidental = Accidental::sharp;
            }
            else if( accidentalSurprise == 2 )
            {
                n.pitchData.accidental = Accidental::flat;
            }
            n.durationData.durationName = DurationName::whole;
            n.durationData.durationTimeTicks = whole;
            VoiceData v;
            v.notes.emplace_back( n );
            StaffData s;
            if( i == 0 )
            {
                ClefData cd;
                cd.setAlto();
                s.clefs.emplace_back( cd );
            }
            s.voices.emplace( 0, v );
            MeasureData m;
            m.staves.emplace_back( s );
            p.measures.emplace_back( m );
        }
        ScoreData score;
        score.ticksPerQuarter = ticksPerQuarter;
        score.layout.scalingMillimeters = 7.2319;
        score.layout.scalingTenths = 40;
        score.parts.emplace_back( p );
        return score;
    }
}

TEST( TestPageData, PageData )
{
    auto score = pageDataTest::makeSomeBoringMusic( 33 );
    auto& docMgr = DocumentManager::getInstance();
    SystemData sd;
    sd.measureIndex = 1;
    score.systems.emplace( sd );
    sd = SystemData{};
    sd.measureIndex = 5;
    sd.topSystemDistance = 23;
    sd.leftMargin = 55;
    sd.rightMargin = 56;
    sd.isSystemDistanceSpecified = true;
    sd.isTopSystemDistanceSpecified = true;
    sd.isMarginSpecified = true;
    score.systems.emplace( sd );
    sd.measureIndex = 10;
    score.systems.emplace( sd );
    sd.measureIndex = 20;
    score.systems.emplace( sd );
    sd.measureIndex = 30;
    score.systems.emplace( sd );
    sd.measureIndex = 40;
    score.systems.emplace( sd );
    PageData pd{ StartSpecifier::measureIndex, 6 };
    pd.pageLayoutData = PageLayoutData{};
    pd.pageLayoutData->pageWidth = 1500;
    pd.pageLayoutData->pageHeight = 1501;
    pd.pageLayoutData->pageMargins.evenPageLeftMargin = 12;
    pd.pageLayoutData->pageMargins.oddPageLeftMargin = 12;
    score.pages.emplace( pd );
    pd = PageData{ StartSpecifier::systemIndex, 3 };
    pd.newPage = false;
    score.pages.emplace( pd );
    const auto id = docMgr.createFromScore( score );
    docMgr.writeToStream( id, std::cout );
    docMgr.writeToFile( id, "/Users/mjb/Desktop/foo.musicxml");
    docMgr.destroyDocument( id );
}

#endif
