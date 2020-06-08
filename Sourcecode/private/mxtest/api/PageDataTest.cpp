// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "cpul/cpulTestHarness.h"
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "mx/api/ScoreData.h"
#include "mx/api/DocumentManager.h"
#include "mxtest/file/MxFileRepository.h"
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
        score.defaults.scalingMillimeters = 7.2319;
        score.defaults.scalingTenths = 40;
        score.parts.emplace_back( p );
        return score;
    }
}

//#define MJB_DEBUG

TEST( TestPageData, PageData )
{
    auto score1 = pageDataTest::makeSomeBoringMusic(33 );
    auto& docMgr = DocumentManager::getInstance();
    SystemData sd{ Bool::yes };
    MeasureIndex measureIndex = 1;
    score1.xxbadnames[measureIndex].system = sd;
    measureIndex = 5;
    sd = SystemData{ Bool::yes };
    sd.layout.topSystemDistance = 1;
    sd.layout.margins = LeftRight{ 2, 3 };
    sd.layout.systemDistance = 0;
    score1.xxbadnames[measureIndex].system = sd;
    measureIndex = 10;
    sd = SystemData{ Bool::yes };
    sd.layout.topSystemDistance = 4;
    sd.layout.margins = LeftRight{ 5, 6 };
    sd.layout.systemDistance = std::nullopt;
    score1.xxbadnames[measureIndex].system = sd;
    measureIndex = 20;
    sd = SystemData{ Bool::yes };
    sd.layout.topSystemDistance = std::nullopt;
    sd.layout.margins = LeftRight{ 7, 8 };
    sd.layout.systemDistance = 9;
    score1.xxbadnames[measureIndex].system = sd;
    measureIndex = 30;
    sd = SystemData{ Bool::yes };
    sd.layout.topSystemDistance = 10;
    sd.layout.margins = LeftRight{ 11, 12 };
    sd.layout.systemDistance = 13;
    score1.xxbadnames[measureIndex].system = sd;
    PageData pd;
    pd.pageLayoutData.size = SizeData{ 1501, 1500 };
    pd.pageLayoutData.margins.even = MarginsData{12.0, 0.0, 0.0, 0.0 };
    pd.pageLayoutData.margins.odd = MarginsData{12.0, 0.0, 0.0, 0.0 };
    score1.xxbadnames[6].page = pd;
    pd = PageData{};
    pd.newPage = Bool::no;
    score1.xxbadnames[10].page = pd;
    const auto id1 = docMgr.createFromScore(score1 );
    std::stringstream xml1;
    docMgr.writeToStream( id1, xml1 );
#ifdef MJB_DEBUG
    docMgr.writeToFile( id1, "/Users/mjb/Desktop/xml1.xml" );
#endif
    docMgr.destroyDocument( id1 );
    std::istringstream xml1is{ xml1.str() };
    const auto id2 = docMgr.createFromStream( xml1is );
    const auto score2 = docMgr.getData( id2 );
    CHECK( score1 == score2 );
    docMgr.destroyDocument( id2 );
    const auto id3 = docMgr.createFromScore( score2 );
    std::stringstream xml3;
    docMgr.writeToStream( id3, xml3 );
#ifdef MJB_DEBUG
    docMgr.writeToFile( id3, "/Users/mjb/Desktop/xml3.xml" );
#endif
    CHECK( xml1.str() == xml3.str() );
}

TEST( LoadFinaleExport, PageData )
{
    const auto filepath = mxtest::MxFileRepository::getFullPath( "systems-and-pages.xml" );
    auto& docMgr = DocumentManager::getInstance();
    const auto id = docMgr.createFromFile( filepath );
    const auto score = docMgr.getData( id );
    docMgr.destroyDocument( id );

    // The loaded file has page and system information as follows:
    // measure number 1  index 0  : page : system
    // measure number 3  index 2  :      : system
    // measure number 5  index 4  : page :
    // measure number 9  index 8  :      : system
    // measure number 13 index 12 : page :
    // measure number 22 index 21 : page :
    // measure number 31 index 30 :      : system

    // Check that PageData objects are populated in the map for the appropriate measures.
    auto it = score.xxbadnames.find( 0 );
    REQUIRE( it != std::cend( score.xxbadnames ) );
    const auto& pageM0 = it->second.page;
    it = score.xxbadnames.find( 4 );
    REQUIRE( it != std::cend( score.xxbadnames ) );
    const auto& pageM4 = it->second.page;
    it = score.xxbadnames.find( 12 );
    REQUIRE( it != std::cend( score.xxbadnames ) );
    const auto& pageM12 = it->second.page;
    it = score.xxbadnames.find( 21 );
    REQUIRE( it != std::cend( score.xxbadnames ) );
    const auto& pageM21 = it->second.page;

    // Check that the SystemData objects are populated in the set with the appropriate measure indices.
    it = score.xxbadnames.find( 0 );
    REQUIRE( it != std::cend( score.xxbadnames ) );
    const auto systemM0 = it->second.system;
    it = score.xxbadnames.find( 2 );
    REQUIRE( it != std::cend( score.xxbadnames ) );
    const auto systemM2 = it->second.system;
    it = score.xxbadnames.find( 8 );
    REQUIRE( it != std::cend( score.xxbadnames ) );
    const auto systemM8 = it->second.system;
    it = score.xxbadnames.find( 30 );
    REQUIRE( it != std::cend( score.xxbadnames ) );
    const auto systemM30 = it->second.system;

    // Check that the SystemData and PageData objects contain the correct information

    // Measure 1 (measure index 0)
    // <print page-number="1">
    //   <page-layout>
    //     <page-height>1545</page-height>
    //     <page-width>1194</page-width>
    //   </page-layout>
    //   <system-layout>
    //     <system-margins>
    //       <left-margin>70</left-margin>
    //       <right-margin>0</right-margin>
    //     </system-margins>
    //     <top-system-distance>211</top-system-distance>
    //   </system-layout>
    //   <measure-numbering>system</measure-numbering>
    // </print>
    CHECK( pageM0.isUsed() );
    CHECK( Bool::unspecified == pageM0.newPage);
    CHECK( pageM0.pageLayoutData.isUsed() );
    CHECK_DOUBLES_EQUAL( 1545.0, pageM0.pageLayoutData.size.value().height, MX_API_EQUALITY_EPSILON );
    CHECK_DOUBLES_EQUAL( 1194.0, pageM0.pageLayoutData.size.value().width, MX_API_EQUALITY_EPSILON );
    CHECK( !pageM0.pageLayoutData.margins.isUsed() );
    CHECK( pageM0.pageNumber );
    CHECK( "1" == *pageM0.pageNumber );
    // TODO check for new-system = unspecified
    CHECK( systemM0.layout.margins );
    CHECK_DOUBLES_EQUAL( 0.0, systemM0.layout.margins.value().right, MX_API_EQUALITY_EPSILON );
    CHECK_DOUBLES_EQUAL( 70.0, systemM0.layout.margins.value().left, MX_API_EQUALITY_EPSILON );
    CHECK( !systemM0.layout.systemDistance );
    CHECK( systemM0.layout.topSystemDistance );
    CHECK_DOUBLES_EQUAL( 211.0, systemM0.layout.topSystemDistance.value(), MX_API_EQUALITY_EPSILON );

    // Measure 3 (measure index 2)
    // <print new-system="yes">
    //   <system-layout>
    //     <system-distance>114</system-distance>
    //   </system-layout>
    // </print>
    // TODO check for new-system = yes
    CHECK( !systemM2.layout.margins );
    CHECK( systemM2.layout.systemDistance );
    CHECK_DOUBLES_EQUAL( 114.0, systemM2.layout.systemDistance.value(), MX_API_EQUALITY_EPSILON );
    CHECK( !systemM2.layout.topSystemDistance );

    // Measure 5 (measure index 4)
    // <print new-page="yes" page-number="2"/>
    CHECK( pageM4.isUsed() );
    CHECK( Bool::yes == pageM4.newPage );
    CHECK( !pageM4.pageLayoutData.isUsed() );
    CHECK( !pageM4.pageLayoutData.margins.isUsed() );
    CHECK( pageM4.pageNumber );
    CHECK( "2" == *pageM4.pageNumber );

    // Measure 9 (measure index 8)
    // <print new-system="yes">
    //   <system-layout>
    //     <system-distance>114</system-distance>
    //   </system-layout>
    // </print>
    // TODO check for new-system = yes
    CHECK( !systemM8.layout.margins );
    CHECK( systemM8.layout.systemDistance );
    CHECK_DOUBLES_EQUAL( 114.0, systemM8.layout.systemDistance.value(), MX_API_EQUALITY_EPSILON );
    CHECK( !systemM8.layout.topSystemDistance );

    // Measure 13 (measure index 12)
    // <print new-page="yes" page-number="3">
    //   <page-layout>
    //     <page-height>1545</page-height>
    //     <page-width>1194</page-width>
    //   </page-layout>
    // </print>
    CHECK( Bool::yes == pageM12.newPage);
    CHECK( pageM12.pageLayoutData.isUsed() );
    CHECK_DOUBLES_EQUAL( 1599.0, pageM12.pageLayoutData.size.value().height, MX_API_EQUALITY_EPSILON );
    CHECK_DOUBLES_EQUAL( 1203.0, pageM12.pageLayoutData.size.value().width, MX_API_EQUALITY_EPSILON );
    CHECK( !pageM12.pageLayoutData.margins.isUsed() );
    CHECK( pageM12.pageNumber );
    CHECK( "3" == *pageM12.pageNumber );

    // Measure 22 (measure index 21)
    // <print new-page="yes" page-number="4">
    //   <page-layout>
    //     <page-height>1545</page-height>
    //     <page-width>1194</page-width>
    //   </page-layout>
    // </print>
    CHECK( Bool::yes == pageM21.newPage);
    CHECK( pageM21.pageLayoutData.isUsed() );
    CHECK_DOUBLES_EQUAL( 1775.0, pageM21.pageLayoutData.size.value().height, MX_API_EQUALITY_EPSILON );
    CHECK_DOUBLES_EQUAL( 1776.0, pageM21.pageLayoutData.size.value().width, MX_API_EQUALITY_EPSILON );
    CHECK( !pageM21.pageLayoutData.margins.isUsed() );
    CHECK( pageM21.pageNumber );
    CHECK( "4" == *pageM21.pageNumber );
}

#endif
