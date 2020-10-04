// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS
#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mx/core/Document.h"
#include "mx/core/elements/Chromatic.h"
#include "mx/core/elements/Diatonic.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/Properties.h"
#include "mx/core/elements/Transpose.h"
#include "mx/impl/Converter.h"
#include "mxtest/file/MxFileRepository.h"

#include <sstream>

namespace mxtest
{
    // There seems to be an existing notion of an API 'round trip' test which requires specifying the
    // music data by hand. In this version of an API round trip test, we will load the file into API,
    // save the file back to disk, load it back up into the API and assert equality.
    inline mx::api::ScoreData roundtrip( const mx::api::ScoreData& inOriginal )
    {
        auto& docMgr = mx::api::DocumentManager::getInstance();
        const auto id = docMgr.createFromScore( inOriginal );
        std::ostringstream oss;
        docMgr.writeToStream( id, oss );
        std::istringstream iss{ oss.str() };
        const auto id2 = docMgr.createFromStream( iss );
        auto result = docMgr.getData( id2 );
        docMgr.destroyDocument( id );
        docMgr.destroyDocument( id2 );
        return result;
    }

    // create a score with one part an any number of measures
    inline mx::api::ScoreData makeScore( int measures )
    {
        auto score = mx::api::ScoreData{};
        score.parts.emplace_back( mx::api::PartData{} );
        auto& part = score.parts.back();
        for( int i = 0; i < measures; ++ i )
        {
            part.measures.emplace_back( mx::api::MeasureData{} );
        }
        return score;
    }

    inline void checkCoreTransposeElement(
            const mx::api::ScoreData& inScore,
            int inExpectedChromatic,
            std::optional<int> inExpectedDiatonic,
            std::optional<int> inExpectedOctave )
    {
        auto& docMgr = mx::api::DocumentManager::getInstance();
        const auto id = docMgr.createFromScore( inScore );
        const auto core = docMgr.getDocument( id );
        docMgr.destroyDocument( id );
        CHECK( core->getChoice() == mx::core::DocumentChoice::partwise );
        const auto& score = core->getScorePartwise();
        REQUIRE( !score->getPartwisePartSet().empty() );
        const auto& part = score->getPartwisePartSet().front();
        REQUIRE( !part->getPartwiseMeasureSet().empty() );
        const auto& measure = part->getPartwiseMeasureSet().front();
        const auto& choices = measure->getMusicDataGroup()->getMusicDataChoiceSet();
        REQUIRE( !choices.empty() );
        mx::core::PropertiesPtr properties;
        for( const auto& choice : choices )
        {
            if( choice->getChoice() == mx::core::MusicDataChoice::Choice::properties )
            {
                // there should be only 1 properties element
                CHECK( properties == nullptr );
                properties = choice->getProperties();
                // continue looping to make sure we don't hit another properties element
            }
        }
        // we should have found exactly one properties element
        CHECK( properties != nullptr );
        // assert that the transpose element matches
        REQUIRE( properties->getTransposeSet().size() == 1 );
        const auto& transpose = properties->getTransposeSet().front();
        CHECK_EQUAL( inExpectedChromatic, static_cast<int>( transpose->getChromatic()->getValue().getValue() ) );
        if( inExpectedDiatonic.has_value() )
        {
            CHECK( transpose->getHasDiatonic() );
            CHECK_EQUAL( inExpectedDiatonic.value(), static_cast<int>( transpose->getDiatonic()->getValue().getValue() ) );
        }
        if( inExpectedOctave.has_value() )
        {
            CHECK( transpose->getHasOctaveChange() );
            CHECK_EQUAL( inExpectedOctave.value(), static_cast<int>( transpose->getOctaveChange()->getValue().getValue() ) );
        }
    }

    inline mx::core::TransposePtr makeTranspose( int inChromatic, std::optional<int> inDiatonic, std::optional<int> inOctave )
    {
        auto transpose = mx::core::makeTranspose();
        transpose->getChromatic()->setValue( mx::core::Semitones{ static_cast<mx::core::DecimalType>( inChromatic ) } );
        if( inDiatonic.has_value() )
        {
            transpose->setHasDiatonic( true );
            transpose->getDiatonic()->setValue( mx::core::Integer{ inDiatonic.value() } );
        }
        if( inOctave.has_value() )
        {
            transpose->setHasOctaveChange( true );
            transpose->getOctaveChange()->setValue( mx::core::Integer{ inOctave.value() } );
        }
        return transpose;
    }
}

TEST( convertToTransposeData01, Transposition )
{
    // in Db (up) with no diatonic
    const int expectedChromatic = 1;
    const int expectedDiatonic = 1;
    const auto transpose = mxtest::makeTranspose( 1, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData02, Transposition )
{
    // in B (down) with no diatonic
    const int expectedChromatic = -1;
    const int expectedDiatonic = -1;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData03, Transposition )
{
    // in D with no diatonic
    const int expectedChromatic = 2;
    const int expectedDiatonic = 1;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData04, Transposition )
{
    // in Bb with no diatonic
    const int expectedChromatic = -2;
    const int expectedDiatonic = -1;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData05, Transposition )
{
    // in Eb (up) with no diatonic
    const int expectedChromatic = 3;
    const int expectedDiatonic = 2;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData06, Transposition )
{
    // in A with no diatonic
    const int expectedChromatic = -3;
    const int expectedDiatonic = -2;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData07, Transposition )
{
    // in E with no diatonic
    const int expectedChromatic = 4;
    const int expectedDiatonic = 2;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData08, Transposition )
{
    // in Ab with no diatonic
    const int expectedChromatic = -4;
    const int expectedDiatonic = -2;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData09, Transposition )
{
    // in F with no diatonic
    const int expectedChromatic = 5;
    const int expectedDiatonic = 3;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData10, Transposition )
{
    // in G with no diatonic
    const int expectedChromatic = -5;
    const int expectedDiatonic = -3;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData11, Transposition )
{
    // in F# with no diatonic
    const int expectedChromatic = 6;
    const int expectedDiatonic = 3;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData12, Transposition )
{
    // in Gb with no diatonic
    const int expectedChromatic = -6;
    const int expectedDiatonic = -3;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData13, Transposition )
{
    // in G (up) with no diatonic
    const int expectedChromatic = 7;
    const int expectedDiatonic = 4;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData14, Transposition )
{
    // in F (down) with no diatonic
    const int expectedChromatic = -7;
    const int expectedDiatonic = -4;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData15, Transposition )
{
    // in Ab (up) with no diatonic
    const int expectedChromatic = 8;
    const int expectedDiatonic = 5;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData16, Transposition )
{
    // in E (down) with no diatonic
    const int expectedChromatic = -8;
    const int expectedDiatonic = -5;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData17, Transposition )
{
    // in A (up) with no diatonic
    const int expectedChromatic = 9;
    const int expectedDiatonic = 5;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData18, Transposition )
{
    // in Eb (down) with no diatonic
    const int expectedChromatic = -9;
    const int expectedDiatonic = -5;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData19, Transposition )
{
    // in Bb (up) with no diatonic
    const int expectedChromatic = 10;
    const int expectedDiatonic = 6;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData20, Transposition )
{
    // in D (down) with no diatonic
    const int expectedChromatic = -10;
    const int expectedDiatonic = -6;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData21, Transposition )
{
    // in B (up) with no diatonic
    const int expectedChromatic = 11;
    const int expectedDiatonic = 6;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData22, Transposition )
{
    // in Db (down) with no diatonic
    const int expectedChromatic = -11;
    const int expectedDiatonic = -6;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData23, Transposition )
{
    // in C (up) with no diatonic and no octave (technically incorrect musicxml)
    const int expectedChromatic = 12;
    const int expectedDiatonic = 7;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData24, Transposition )
{
    // in C (down) with no diatonic and no octave (technically incorrect musicxml)
    const int expectedChromatic = -12;
    const int expectedDiatonic = -7;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData25, Transposition )
{
    // in Db+1 (up) with no diatonic and no octave (technically incorrect musicxml)
    const int expectedChromatic = 13;
    const int expectedDiatonic = 8;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData26, Transposition )
{
    // in B-1 (down) with no diatonic and no octave (technically incorrect musicxml)
    const int expectedChromatic = -13;
    const int expectedDiatonic = -8;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, std::nullopt, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData27, Transposition )
{
    // crazy numbers, both chromatic and diatonic specified (and technically invalid), octave not specified
    const int expectedChromatic = -130;
    const int expectedDiatonic = 147;
    const auto transpose = mxtest::makeTranspose( expectedChromatic, expectedDiatonic, std::nullopt );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData28, Transposition )
{
    // crazy numbers, both chromatic and diatonic specified (and technically invalid), octave is specified
    const int expectedChromatic = -138;
    const int expectedDiatonic = 147;
    const int octave = 2;
    const auto transpose = mxtest::makeTranspose( -162, 133, octave );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData29, Transposition )
{
    // normal, correct usage with octave offset, e.g. Tenor Saxophone in Bb
    const int expectedChromatic = -14;
    const int expectedDiatonic = -8;
    const int octave = -1;
    const auto transpose = mxtest::makeTranspose( -2, -1, octave );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData30, Transposition )
{
    // normal, correct usage with octave offset, e.g. Piccolo Clarinet in Bb
    const int expectedChromatic = 10;
    const int expectedDiatonic = 6;
    const int octave = 1;
    const auto transpose = mxtest::makeTranspose( -2, -1, octave );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( convertToTransposeData31, Transposition )
{
    // complete nonsense input, chromatic is out of range, diatonic is not specified, octave is specified.
    // the behaviour is undefined, but this test locks down the behaviour as we have it now.
    const int expectedChromatic = -1306;
    const int expectedDiatonic = -762;
    const int octave = -6;
    const auto transpose = mxtest::makeTranspose( -1234, std::nullopt, octave );
    const auto actual = mx::impl::Converter::convertToTransposeData( *transpose );
    CHECK_EQUAL( expectedChromatic, actual.chromatic );
    CHECK_EQUAL( expectedDiatonic, actual.diatonic );
}

TEST( Conversion01, Transposition )
{
    const auto expectedChromatic = 2;
    const auto expectedDiatonic = 1;
    const auto expectedOctave = 0;
    const auto transposeDataChromatic = 2;
    const auto transposeDataDiatonic = 1;
    const auto original = mx::api::TransposeData{ transposeDataChromatic, transposeDataDiatonic };
    const auto converted = mx::impl::Converter::convertToTranspose( original );
    REQUIRE( converted != nullptr );
    CHECK_EQUAL( expectedChromatic, static_cast<int>( converted->getChromatic()->getValue().getValue() ) );
    CHECK( converted->getHasDiatonic() == ( expectedDiatonic != 0 ) );
    CHECK_EQUAL( expectedDiatonic, static_cast<int>( converted->getDiatonic()->getValue().getValue() ) );
    CHECK( converted->getHasOctaveChange() == ( expectedOctave != 0 ) );
    CHECK_EQUAL( expectedOctave, static_cast<int>( converted->getOctaveChange()->getValue().getValue() ) );
    const auto final = mx::impl::Converter::convertToTransposeData( *converted );
    CHECK_EQUAL( transposeDataChromatic, final.chromatic );
    CHECK_EQUAL( transposeDataDiatonic, final.diatonic );
    CHECK_EQUAL( original, final );
}

TEST( Conversion02, Transposition )
{
    const auto expectedChromatic = 0;
    const auto expectedDiatonic = 0;
    const auto expectedOctave = 1;
    const auto transposeDataChromatic = 12;
    const auto transposeDataDiatonic = 7;
    const auto original = mx::api::TransposeData{ transposeDataChromatic, transposeDataDiatonic };
    const auto converted = mx::impl::Converter::convertToTranspose( original );
    REQUIRE( converted != nullptr );
    CHECK_EQUAL( expectedChromatic, static_cast<int>( converted->getChromatic()->getValue().getValue() ) );
    CHECK( converted->getHasDiatonic() == ( expectedDiatonic != 0 ) );
    CHECK_EQUAL( expectedDiatonic, static_cast<int>( converted->getDiatonic()->getValue().getValue() ) );
    CHECK( converted->getHasOctaveChange() == ( expectedOctave != 0 ) );
    CHECK_EQUAL( expectedOctave, static_cast<int>( converted->getOctaveChange()->getValue().getValue() ) );
    const auto final = mx::impl::Converter::convertToTransposeData( *converted );
    CHECK_EQUAL( transposeDataChromatic, final.chromatic );
    CHECK_EQUAL( transposeDataDiatonic, final.diatonic );
    CHECK_EQUAL( original, final );
}

TEST( Conversion03, Transposition )
{
    const auto expectedChromatic = 4;
    const auto expectedDiatonic = 2;
    const auto expectedOctave = 1;
    const auto transposeDataChromatic = 16;
    const auto transposeDataDiatonic = 9;
    const auto original = mx::api::TransposeData{ transposeDataChromatic, transposeDataDiatonic };
    const auto converted = mx::impl::Converter::convertToTranspose( original );
    REQUIRE( converted != nullptr );
    CHECK_EQUAL( expectedChromatic, static_cast<int>( converted->getChromatic()->getValue().getValue() ) );
    CHECK( converted->getHasDiatonic() == ( expectedDiatonic != 0 ) );
    CHECK_EQUAL( expectedDiatonic, static_cast<int>( converted->getDiatonic()->getValue().getValue() ) );
    CHECK( converted->getHasOctaveChange() == ( expectedOctave != 0 ) );
    CHECK_EQUAL( expectedOctave, static_cast<int>( converted->getOctaveChange()->getValue().getValue() ) );
    const auto final = mx::impl::Converter::convertToTransposeData( *converted );
    CHECK_EQUAL( transposeDataChromatic, final.chromatic );
    CHECK_EQUAL( transposeDataDiatonic, final.diatonic );
    CHECK_EQUAL( original, final );
}

TEST( Conversion04, Transposition )
{
    const auto expectedChromatic = -4;
    const auto expectedDiatonic = -2;
    const auto expectedOctave = -1;
    const auto transposeDataChromatic = -16;
    const auto transposeDataDiatonic = -9;
    const auto original = mx::api::TransposeData{ transposeDataChromatic, transposeDataDiatonic };
    const auto converted = mx::impl::Converter::convertToTranspose( original );
    REQUIRE( converted != nullptr );
    CHECK_EQUAL( expectedChromatic, static_cast<int>( converted->getChromatic()->getValue().getValue() ) );
    CHECK( converted->getHasDiatonic() == ( expectedDiatonic != 0 ) );
    CHECK_EQUAL( expectedDiatonic, static_cast<int>( converted->getDiatonic()->getValue().getValue() ) );
    CHECK( converted->getHasOctaveChange() == ( expectedOctave != 0 ) );
    CHECK_EQUAL( expectedOctave, static_cast<int>( converted->getOctaveChange()->getValue().getValue() ) );
    const auto final = mx::impl::Converter::convertToTransposeData( *converted );
    CHECK_EQUAL( transposeDataChromatic, final.chromatic );
    CHECK_EQUAL( transposeDataDiatonic, final.diatonic );
    CHECK_EQUAL( original, final );
}

TEST( Tests01, Transposition )
{
    const int chromatic = 2;
    const int diatonic = 1;
    const int expectedXmlChromatic = 2;
    const int expectedXmlDiatonic = 1;
    const std::optional<int> expectedXmlOctave = std::nullopt;
    auto score = mxtest::makeScore( 1 );
    auto& part = score.parts.back();
    part.transposition = mx::api::TransposeData{ chromatic, diatonic };
    CHECK( part.transposition->isUsed() );
    mxtest::checkCoreTransposeElement( score, expectedXmlChromatic, expectedXmlDiatonic, expectedXmlOctave );
    const auto result = mxtest::roundtrip( score );
    REQUIRE( result.parts.back().transposition.has_value() );
    CHECK( result.parts.back().transposition );
    CHECK( result.parts.back().transposition->isUsed() );
    CHECK_EQUAL( chromatic, result.parts.back().transposition->chromatic );
    CHECK_EQUAL( diatonic, result.parts.back().transposition->diatonic );
}

TEST( Tests02, Transposition )
{
    const int chromatic = 11;
    const int diatonic = 6;
    const int expectedXmlChromatic = 11;
    const int expectedXmlDiatonic = 6;
    const std::optional<int> expectedXmlOctave = std::nullopt;
    auto score = mxtest::makeScore( 1 );
    auto& part = score.parts.back();
    part.transposition = mx::api::TransposeData{ chromatic, diatonic };
    CHECK( part.transposition->isUsed() );
    mxtest::checkCoreTransposeElement( score, expectedXmlChromatic, expectedXmlDiatonic, expectedXmlOctave );
    const auto result = mxtest::roundtrip( score );
    REQUIRE( result.parts.back().transposition.has_value() );
    CHECK( result.parts.back().transposition );
    CHECK( result.parts.back().transposition->isUsed() );
    CHECK_EQUAL( chromatic, result.parts.back().transposition->chromatic );
    CHECK_EQUAL( diatonic, result.parts.back().transposition->diatonic );
}

TEST( Tests03, Transposition )
{
    const int chromatic = 12;
    const int diatonic = 7;
    const int expectedXmlChromatic = 0;
    const std::optional<int> expectedXmlDiatonic = std::nullopt;
    const std::optional<int> expectedXmlOctave = std::nullopt;
    auto score = mxtest::makeScore( 1 );
    auto& part = score.parts.back();
    part.transposition = mx::api::TransposeData{ chromatic, diatonic };
    CHECK( part.transposition->isUsed() );
    mxtest::checkCoreTransposeElement( score, expectedXmlChromatic, expectedXmlDiatonic, expectedXmlOctave );
    const auto result = mxtest::roundtrip( score );
    REQUIRE( result.parts.back().transposition.has_value() );
    CHECK( result.parts.back().transposition );
    CHECK( result.parts.back().transposition->isUsed() );
    CHECK_EQUAL( chromatic, result.parts.back().transposition->chromatic );
    CHECK_EQUAL( diatonic, result.parts.back().transposition->diatonic );
}

TEST( Tests04, Transposition )
{
    const int chromatic = 13;
    const int diatonic = 8;
    const int expectedXmlChromatic = 1;
    const int expectedXmlDiatonic = 1;
    const std::optional<int> expectedXmlOctave = 1;
    auto score = mxtest::makeScore( 1 );
    auto& part = score.parts.back();
    part.transposition = mx::api::TransposeData{ chromatic, diatonic };
    CHECK( part.transposition->isUsed() );
    mxtest::checkCoreTransposeElement( score, expectedXmlChromatic, expectedXmlDiatonic, expectedXmlOctave );
    const auto result = mxtest::roundtrip( score );
    REQUIRE( result.parts.back().transposition.has_value() );
    CHECK( result.parts.back().transposition );
    CHECK( result.parts.back().transposition->isUsed() );
    CHECK_EQUAL( chromatic, result.parts.back().transposition->chromatic );
    CHECK_EQUAL( diatonic, result.parts.back().transposition->diatonic );
}

TEST( Tests05, Transposition )
{
    const int chromatic = 14;
    const int diatonic = 8;
    const int expectedXmlChromatic = 2;
    const int expectedXmlDiatonic = 1;
    const std::optional<int> expectedXmlOctave = 1;
    auto score = mxtest::makeScore( 1 );
    auto& part = score.parts.back();
    part.transposition = mx::api::TransposeData{ chromatic, diatonic };
    CHECK( part.transposition->isUsed() );
    mxtest::checkCoreTransposeElement( score, expectedXmlChromatic, expectedXmlDiatonic, expectedXmlOctave );
    const auto result = mxtest::roundtrip( score );
    REQUIRE( result.parts.back().transposition.has_value() );
    CHECK( result.parts.back().transposition );
    CHECK( result.parts.back().transposition->isUsed() );
    CHECK_EQUAL( chromatic, result.parts.back().transposition->chromatic );
    CHECK_EQUAL( diatonic, result.parts.back().transposition->diatonic );
}

TEST( Tests06, Transposition )
{
    // super wonky test case, maybe it doesn't matter if it fails
    const int chromatic = 14;
    const int diatonic = -8;
    const int expectedXmlChromatic = 2;
    const int expectedXmlDiatonic = -15;
    const std::optional<int> expectedXmlOctave = 1;
    auto score = mxtest::makeScore( 1 );
    auto& part = score.parts.back();
    part.transposition = mx::api::TransposeData{ chromatic, diatonic };
    CHECK( part.transposition->isUsed() );
    mxtest::checkCoreTransposeElement( score, expectedXmlChromatic, expectedXmlDiatonic, expectedXmlOctave );
    const auto result = mxtest::roundtrip( score );
    REQUIRE( result.parts.back().transposition.has_value() );
    CHECK( result.parts.back().transposition );
    CHECK( result.parts.back().transposition->isUsed() );
    CHECK_EQUAL( chromatic, result.parts.back().transposition->chromatic );
    CHECK_EQUAL( diatonic, result.parts.back().transposition->diatonic );
}

TEST( Tests07, Transposition )
{
    const int chromatic = -1;
    const int diatonic = -1;
    const int expectedXmlChromatic = -1;
    const int expectedXmlDiatonic = -1;
    const std::optional<int> expectedXmlOctave = std::nullopt;
    auto score = mxtest::makeScore( 1 );
    auto& part = score.parts.back();
    part.transposition = mx::api::TransposeData{ chromatic, diatonic };
    CHECK( part.transposition->isUsed() );
    mxtest::checkCoreTransposeElement( score, expectedXmlChromatic, expectedXmlDiatonic, expectedXmlOctave );
    const auto result = mxtest::roundtrip( score );
    REQUIRE( result.parts.back().transposition.has_value() );
    CHECK( result.parts.back().transposition );
    CHECK( result.parts.back().transposition->isUsed() );
    CHECK_EQUAL( chromatic, result.parts.back().transposition->chromatic );
    CHECK_EQUAL( diatonic, result.parts.back().transposition->diatonic );
}

TEST( Tests08, Transposition )
{
    const int chromatic = -11;
    const int diatonic = -6;
    const int expectedXmlChromatic = -11;
    const int expectedXmlDiatonic = -6;
    const std::optional<int> expectedXmlOctave = std::nullopt;
    auto score = mxtest::makeScore( 1 );
    auto& part = score.parts.back();
    part.transposition = mx::api::TransposeData{ chromatic, diatonic };
    CHECK( part.transposition->isUsed() );
    mxtest::checkCoreTransposeElement( score, expectedXmlChromatic, expectedXmlDiatonic, expectedXmlOctave );
    const auto result = mxtest::roundtrip( score );
    REQUIRE( result.parts.back().transposition.has_value() );
    CHECK( result.parts.back().transposition );
    CHECK( result.parts.back().transposition->isUsed() );
    CHECK_EQUAL( chromatic, result.parts.back().transposition->chromatic );
    CHECK_EQUAL( diatonic, result.parts.back().transposition->diatonic );
}

TEST( Tests09, Transposition )
{
    const int chromatic = -12;
    const int diatonic = -7;
    const int expectedXmlChromatic = 0;
    const std::optional<int> expectedXmlDiatonic = std::nullopt;
    const std::optional<int> expectedXmlOctave = -1;
    auto score = mxtest::makeScore( 1 );
    auto& part = score.parts.back();
    part.transposition = mx::api::TransposeData{ chromatic, diatonic };
    CHECK( part.transposition->isUsed() );
    mxtest::checkCoreTransposeElement( score, expectedXmlChromatic, expectedXmlDiatonic, expectedXmlOctave );
    const auto result = mxtest::roundtrip( score );
    REQUIRE( result.parts.back().transposition.has_value() );
    CHECK( result.parts.back().transposition );
    CHECK( result.parts.back().transposition->isUsed() );
    CHECK_EQUAL( chromatic, result.parts.back().transposition->chromatic );
    CHECK_EQUAL( diatonic, result.parts.back().transposition->diatonic );
}

TEST( noMeasures, Transposition )
{
    const int chromatic = 2;
    const int diatonic = 1;
    const int expectedXmlChromatic = 2;
    const std::optional<int> expectedXmlDiatonic = 1;
    const std::optional<int> expectedXmlOctave = std::nullopt;
    auto score = mxtest::makeScore( 0 ); // NO MEASURES!
    auto& part = score.parts.back();
    part.transposition = mx::api::TransposeData{ chromatic, diatonic };
    CHECK( part.transposition->isUsed() );
    mxtest::checkCoreTransposeElement( score, expectedXmlChromatic, expectedXmlDiatonic, expectedXmlOctave );
    const auto result = mxtest::roundtrip( score );
    REQUIRE( result.parts.back().transposition.has_value() );
    CHECK( result.parts.back().transposition );
    CHECK( result.parts.back().transposition->isUsed() );
    CHECK_EQUAL( chromatic, result.parts.back().transposition->chromatic );
    CHECK_EQUAL( diatonic, result.parts.back().transposition->diatonic );
}

TEST( noTransposition, Transposition )
{
    const int chromatic = 0;
    const int diatonic = 0;
    auto score = mxtest::makeScore( 0 );
    auto& part = score.parts.back();
    part.transposition = mx::api::TransposeData{ chromatic, diatonic };
    CHECK( !part.transposition->isUsed() );
    const auto result = mxtest::roundtrip( score );
    CHECK( !result.parts.back().transposition );
}

TEST( Tests10, Transposition )
{
    const int chromatic = -13;
    const int diatonic = -8;
    const int expectedXmlChromatic = -1;
    const std::optional<int> expectedXmlDiatonic = -1;
    const std::optional<int> expectedXmlOctave = -1;
    auto score = mxtest::makeScore( 1 );
    auto& part = score.parts.back();
    part.transposition = mx::api::TransposeData{ chromatic, diatonic };
    CHECK( part.transposition->isUsed() );
    mxtest::checkCoreTransposeElement( score, expectedXmlChromatic, expectedXmlDiatonic, expectedXmlOctave );
    const auto result = mxtest::roundtrip( score );
    REQUIRE( result.parts.back().transposition.has_value() );
    CHECK( result.parts.back().transposition );
    CHECK( result.parts.back().transposition->isUsed() );
    CHECK_EQUAL( chromatic, result.parts.back().transposition->chromatic );
    CHECK_EQUAL( diatonic, result.parts.back().transposition->diatonic );
}

TEST( Tests11, Transposition )
{
    // this is a funky test case. these values are actually nonsense.
    const int chromatic = -14;
    const int diatonic = 8;
    const int expectedXmlChromatic = -2;
    const std::optional<int> expectedXmlDiatonic = 15;
    const std::optional<int> expectedXmlOctave = -1;
    auto score = mxtest::makeScore( 1 );
    auto& part = score.parts.back();
    part.transposition = mx::api::TransposeData{ chromatic, diatonic };
    CHECK( part.transposition->isUsed() );
    mxtest::checkCoreTransposeElement( score, expectedXmlChromatic, expectedXmlDiatonic, expectedXmlOctave );
    const auto result = mxtest::roundtrip( score );
    REQUIRE( result.parts.back().transposition.has_value() );
    CHECK( result.parts.back().transposition );
    CHECK( result.parts.back().transposition->isUsed() );
    CHECK_EQUAL( chromatic, result.parts.back().transposition->chromatic );
    CHECK_EQUAL( diatonic, result.parts.back().transposition->diatonic );
}

TEST( ApiRoundTrip, Transposition )
{
    const auto original = mxtest::MxFileRepository::loadFile( "transposition.musicxml" );
    const auto result = mxtest::roundtrip( original );
    REQUIRE( result.parts.size() == 31 );
    // manually check some of the part transpositions
    size_t partIndex = 0;
    std::string partId = "P" + std::to_string( partIndex + 1 );
    auto part = result.parts.at( partIndex );
    CHECK_EQUAL( partId, part.uniqueId );
    CHECK_WITH_MESSAGE( part.transposition.has_value(), partId + " is missing transposition" );
    CHECK( part.transposition->isUsed() );
    CHECK_EQUAL( 12, part.transposition->chromatic );
    CHECK_EQUAL( 7, part.transposition->diatonic );

    partIndex = 6;
    partId = "P" + std::to_string( partIndex + 1 );
    part = result.parts.at( partIndex );
    CHECK_EQUAL( partId, part.uniqueId );
    CHECK_WITH_MESSAGE( !part.transposition.has_value(), partId + " should have no transposition" );

    partIndex = 13;
    partId = "P" + std::to_string( partIndex + 1 );
    part = result.parts.at( partIndex );
    CHECK_EQUAL( partId, part.uniqueId );
    CHECK_WITH_MESSAGE( part.transposition.has_value(), partId + " is missing transposition" );
    CHECK( part.transposition->isUsed() );
    CHECK_EQUAL( -9, part.transposition->chromatic );
    CHECK_EQUAL( -5, part.transposition->diatonic );

    partIndex = 14;
    partId = "P" + std::to_string( partIndex + 1 );
    part = result.parts.at( partIndex );
    CHECK_EQUAL( partId, part.uniqueId );
    CHECK_WITH_MESSAGE( part.transposition.has_value(), partId + " is missing transposition" );
    CHECK( part.transposition->isUsed() );
    CHECK_EQUAL( -14, part.transposition->chromatic );
    CHECK_EQUAL( -8, part.transposition->diatonic );

    partIndex = 15;
    partId = "P" + std::to_string( partIndex + 1 );
    part = result.parts.at( partIndex );
    CHECK_EQUAL( partId, part.uniqueId );
    CHECK_WITH_MESSAGE( part.transposition.has_value(), partId + " is missing transposition" );
    CHECK( part.transposition->isUsed() );
    CHECK_EQUAL( -21, part.transposition->chromatic );
    CHECK_EQUAL( -12, part.transposition->diatonic );

    partIndex = 27;
    partId = "P" + std::to_string( partIndex + 1 );
    part = result.parts.at( partIndex );
    CHECK_EQUAL( partId, part.uniqueId );
    CHECK_WITH_MESSAGE( !part.transposition.has_value(), partId + " should have no transposition" );

    partIndex = 30;
    partId = "P" + std::to_string( partIndex + 1 );
    part = result.parts.at( partIndex );
    CHECK_EQUAL( partId, part.uniqueId );
    CHECK_WITH_MESSAGE( !part.transposition.has_value(), partId + " should have no transposition" );

    CHECK_EQUAL( original, result );
}

#endif