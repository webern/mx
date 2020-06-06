// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "cpul/cpulTestHarness.h"
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "mx/api/ScoreData.h"
#include "mx/api/DocumentManager.h"
#include <sstream>

using namespace std;
using namespace mx::api;

// Make sure the equality macros (i.e. operator==) works correctly with std::optional
TEST( EqualityOfOptional, PitchData )
{
    PageData a;
    PageData b;
    a.pageNumber = "x";
    CHECK( a.pageNumber );
    CHECK( !b.pageNumber );
    CHECK( a != b );
    PageData c;
    c.pageNumber = "1";
    PageData d;
    d.pageNumber = "2";
    CHECK( c != d );
    d.pageNumber = "1";
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
        p.uniqueId = "BORING_MUSIC";
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
            n.userRequestedVoiceNumber = 1;
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
    auto score1 = pageDataTest::makeSomeBoringMusic(33 );
    auto& docMgr = DocumentManager::getInstance();
    SystemData sd;
    sd.measureIndex = 1;
    score1.systems.emplace( sd );
    sd = SystemData{};
    sd.measureIndex = 5;
    sd.topSystemDistance = 23;
    sd.leftMargin = 55;
    sd.rightMargin = 56;
    sd.isSystemDistanceSpecified = true;
    sd.isTopSystemDistanceSpecified = true;
    sd.isMarginSpecified = true;
    score1.systems.emplace( sd );
    sd.measureIndex = 10;
    score1.systems.emplace( sd );
    sd.measureIndex = 20;
    score1.systems.emplace( sd );
    sd.measureIndex = 30;
    score1.systems.emplace( sd );
    PageData pd;
    pd.pageLayoutData.pageWidth = 1500;
    pd.pageLayoutData.pageHeight = 1501;
    pd.pageLayoutData.pageMargins.evenPageLeftMargin = 12;
    pd.pageLayoutData.pageMargins.oddPageLeftMargin = 12;
    score1.pages.emplace(6, pd );
    pd = PageData{};
    pd.newPage = Bool::no;
    score1.pages.emplace(10, pd );
    const auto id1 = docMgr.createFromScore(score1 );
    std::stringstream xml1;
    // TODO - remove this debug write
//    docMgr.writeToStream( id1, std::cout );
    docMgr.writeToStream( id1, xml1 );
    docMgr.writeToFile( id1, "/Users/mjb/Desktop/expected.xml" );
    // TODO - remove this debug write
    docMgr.writeToFile( id1, "/Users/mjb/Desktop/foo.musicxml");
    docMgr.destroyDocument( id1 );
    std::istringstream xml1is{ xml1.str() };
    const auto id2 = docMgr.createFromStream( xml1is );
    const auto score2 = docMgr.getData( id2 );
    CHECK( score1 == score2 );
    docMgr.destroyDocument( id2 );
    const auto id3 = docMgr.createFromScore( score2 );
    std::stringstream xml3;
    docMgr.writeToStream( id3, xml3 );
    docMgr.writeToFile( id3, "/Users/mjb/Desktop/actual.xml" );
    CHECK( xml1.str() == xml3.str() );
}

#endif
