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
    sd.layout.topSystemDistance = 23;
    sd.layout.margins = LeftRight{ 55, 56 };
    sd.layout.systemDistance = 0;
    score1.systems.emplace( sd );
    sd.measureIndex = 10;
    score1.systems.emplace( sd );
    sd.measureIndex = 20;
    score1.systems.emplace( sd );
    sd.measureIndex = 30;
    score1.systems.emplace( sd );
    PageData pd;
    pd.pageLayoutData.size = SizeData{ 1501, 1500 };
    pd.pageLayoutData.margins.even = MarginsData{12.0, 0.0, 0.0, 0.0 };
    pd.pageLayoutData.margins.odd = MarginsData{12.0, 0.0, 0.0, 0.0 };
    score1.pages.emplace(6, pd );
    pd = PageData{};
    pd.newPage = Bool::no;
    score1.pages.emplace(10, pd );
    const auto id1 = docMgr.createFromScore(score1 );
    std::stringstream xml1;
    docMgr.writeToStream( id1, xml1 );
#if 1
    docMgr.writeToFile( id1, "/Users/mjb/Desktop/TestPageData.musicxml" );
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
    auto it = score.pages.find( 0 );
    REQUIRE( it != std::cend( score.pages ) );
    const auto& pageM0 = it->second;
    it = score.pages.find( 4 );
    REQUIRE( it != std::cend( score.pages ) );
    const auto& pageM4 = it->second;
    it = score.pages.find( 12 );
    REQUIRE( it != std::cend( score.pages ) );
    const auto& pageM12 = it->second;
    it = score.pages.find( 21 );
    REQUIRE( it != std::cend( score.pages ) );
    const auto& pageM21 = it->second;

    // Check that the SystemData objects are populated in the set with the appropriate measure indices.
    // TODO - std::set is a terrible data structure for this.
    int sdindex = 0;
    SystemData systemM0{};
    SystemData systemM2{};
    SystemData systemM8{};
    SystemData systemM30{};
    for( const auto& sd : score.systems )
    {
        if( sdindex == 0 )
        {
            systemM0 = sd;
        }
        else if( sdindex == 1 )
        {
            systemM2 = sd;
        }
        else if( sdindex == 2 )
        {
            systemM8 = sd;
        }
        else if( sdindex == 3 )
        {
            systemM30 = sd;
        }
        ++sdindex;
    }
    CHECK( 0 == systemM0.measureIndex );
    CHECK( 2 == systemM2.measureIndex );
    CHECK( 8 == systemM8.measureIndex );
    CHECK( 30 == systemM30.measureIndex );

    // Check that the SystemData and PageData objects contain the correct inforation

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
