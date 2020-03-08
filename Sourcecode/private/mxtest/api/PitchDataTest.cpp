// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include <extern/ezxml/src/include/ezxml/XFactory.h>
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mx/core/Document.h"
#include "mx/core/elements/ScorePartwise.h"
#include "mx/core/elements/PartwisePart.h"
#include "mx/core/elements/PartwiseMeasure.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/NoteChoice.h"
#include "mx/core/elements/NormalNoteGroup.h"
#include "mx/core/elements/FullNoteGroup.h"
#include "mx/core/elements/FullNoteTypeChoice.h"
#include "mx/core/elements/Pitch.h"
#include "mx/core/elements/Notations.h"
#include "mx/core/elements/NotationsChoice.h"
#include "mx/core/elements/Tied.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/Direction.h"
#include "mx/core/elements/DirectionType.h"
#include "mx/core/elements/Offset.h"
#include "mx/core/elements/Pedal.h"
#include "ezxml/ezxml.h"

using namespace std;
using namespace mx::api;

namespace
{
    ezxml::XElementPtr bruteForceFindFirstElement( const ezxml::XElementPtr root, const std::string& inElementName )
    {
        if( !root )
        {
            throw std::runtime_error{ "bug in bruteForceFindFirstElement" };
        }

        if( root->getName() == inElementName )
        {
            return root;
        }

        auto iter = root->begin();
        const auto end = root->end();

        for( ; iter != end; ++iter )
        {
            const auto possible = bruteForceFindFirstElement( iter->clone(), inElementName );
            if( possible && possible->getName() == inElementName )
            {
                return possible;
            }
        }

        return nullptr;
    }

    struct Input
    {
        Step step;
        int alter;
        double cents;
        Accidental accidental;
    };

    struct Output
    {
        Step step;
        int alter;
        double cents;
        Accidental accidental;
        std::string alterString;
        std::string secondAlterString;
    };

    Output pitchDataTest( const Input& input )
    {
        ScoreData score;
        score.parts.emplace_back();
        auto& part = score.parts.back();
        part.measures.emplace_back();
        auto& measure = part.measures.back();
        measure.staves.emplace_back();
        auto& staff = measure.staves.back();
        auto& voice = staff.voices[0];
        voice.notes.emplace_back();
        auto& note = voice.notes.back();
        note.pitchData.step = input.step;
        note.pitchData.accidental = input.accidental;
        note.pitchData.alter = input.alter;
        note.pitchData.cents = input.cents;

        // round trip it through xml
        auto& mgr = DocumentManager::getInstance();
        auto docId = mgr.createFromScore( score );
        std::stringstream ss;
        mgr.writeToStream( docId, ss );
        mgr.destroyDocument( docId );

        // check the alter value that was written to xml
        const auto xdoc = ezxml::XFactory::makeXDoc();
        xdoc->loadStream( ss );
        auto elem = xdoc->getRoot();
        elem = bruteForceFindFirstElement( elem, "alter" );
        const auto alterString = elem->getValue();
        Output output;
        output.alterString = alterString;

// deserialize back to ScoreData
        const std::string xml = ss.str();
        std::istringstream iss{ xml };
        docId = mgr.createFromStream( iss );
        auto oscore = mgr.getData( docId );
        mgr.destroyDocument( docId );
        const auto& opart = oscore.parts.back();
        const auto& omeasure = opart.measures.back();
        const auto& ostaff = omeasure.staves.back();
        const auto& ovoice = ostaff.voices.at( 0 );
        const auto& onote = ovoice.notes.back();
        output.step = onote.pitchData.step;
        output.alter = onote.pitchData.alter;
        output.cents = onote.pitchData.cents;
        output.accidental = onote.pitchData.accidental;

        // serialize a second time and check the alter string again
        docId = mgr.createFromScore( score );
        ss.str( "" );
        mgr.writeToStream( docId, ss );
        mgr.destroyDocument( docId );

        // check the alter value that was written to xml
        const auto xdoc2 = ezxml::XFactory::makeXDoc();
        xdoc2->loadStream( ss );
        auto elem2 = xdoc->getRoot();
        elem2 = bruteForceFindFirstElement( elem2, "alter" );
        const auto alterString2 = elem->getValue();
        output.secondAlterString = alterString2;
        return output;
    }
}

TEST( ThreeQuarterSharp, PitchData )
{
    auto input = Input{};
    input.step = Step::f;
    input.alter = 1;
    input.cents = 50.0;
    input.accidental = Accidental::threeQuartersSharp;
    const std::string expectedAlterString = "1.5";
    const int expectedAlter = input.alter;
    const double expectedCents = input.cents;
    const Accidental expectedAccidental = input.accidental;
    const auto output = pitchDataTest( input );

    CHECK_EQUAL( expectedAlterString, output.alterString );
    CHECK_EQUAL( expectedAlterString, output.secondAlterString );
    CHECK_EQUAL( expectedAlter, output.alter );
    CHECK_DOUBLES_EQUAL( expectedCents, output.cents, MX_API_EQUALITY_EPSILON );
    CHECK( expectedAccidental == output.accidental );
}
T_END;

TEST( ThreeQuarterFlat, PitchData )
{
    auto input = Input{};
    input.step = Step::b;
    input.alter = -1;
    input.cents = -50.0;
    input.accidental = Accidental::threeQuartersFlat;
    const std::string expectedAlterString = "-1.5";
    const int expectedAlter = input.alter;
    const double expectedCents = input.cents;
    const Accidental expectedAccidental = input.accidental;
    const auto output = pitchDataTest( input );

    CHECK_EQUAL( expectedAlterString, output.alterString );
    CHECK_EQUAL( expectedAlterString, output.secondAlterString );
    CHECK_EQUAL( expectedAlter, output.alter );
    CHECK_DOUBLES_EQUAL( expectedCents, output.cents, MX_API_EQUALITY_EPSILON );
    CHECK( expectedAccidental == output.accidental );
}
T_END;

TEST( AlmostDoubleSharp, PitchData )
{
    auto input = Input{};
    input.step = Step::g;
    input.alter = 1;
    input.cents = 99.999;
    input.accidental = Accidental::doubleSharp;
    const std::string expectedAlterString = "1.99999";
    const int expectedAlter = input.alter;
    const double expectedCents = input.cents;
    const Accidental expectedAccidental = input.accidental;
    const auto output = pitchDataTest( input );

    CHECK_EQUAL( expectedAlterString, output.alterString );
    CHECK_EQUAL( expectedAlterString, output.secondAlterString );
    CHECK_EQUAL( expectedAlter, output.alter );
    CHECK_DOUBLES_EQUAL( expectedCents, output.cents, MX_API_EQUALITY_EPSILON );
    CHECK( expectedAccidental == output.accidental );
}
T_END;

TEST( AlmostDoubleFlat, PitchData )
{
    auto input = Input{};
    input.step = Step::a;
    input.alter = -1;
    input.cents = -99.9999;
    input.accidental = Accidental::flatFlat;
    const std::string expectedAlterString = "-1.999999";
    const int expectedAlter = input.alter;
    const double expectedCents = input.cents;
    const Accidental expectedAccidental = input.accidental;
    const auto output = pitchDataTest( input );

    CHECK_EQUAL( expectedAlterString, output.alterString );
    CHECK_EQUAL( expectedAlterString, output.secondAlterString );
    CHECK_EQUAL( expectedAlter, output.alter );
    CHECK_DOUBLES_EQUAL( expectedCents, output.cents, MX_API_EQUALITY_EPSILON );
    CHECK( expectedAccidental == output.accidental );
}
T_END;

TEST( CrazyEdgeCase1, PitchData )
{
    auto input = Input{};
    input.step = Step::g;
    input.alter = 1;
    input.cents = -123456789;
    input.accidental = Accidental::none;
    const std::string expectedAlterString = "-1234566.89";
    const int expectedAlter = -1234566;
    const double expectedCents = -89.0;
    const Accidental expectedAccidental = input.accidental;
    const auto output = pitchDataTest( input );

    CHECK_EQUAL( expectedAlterString, output.alterString );
    CHECK_EQUAL( expectedAlterString, output.secondAlterString );
    CHECK_EQUAL( expectedAlter, output.alter );
    CHECK_DOUBLES_EQUAL( expectedCents, output.cents, MX_API_EQUALITY_EPSILON * 10 );
    CHECK( expectedAccidental == output.accidental );
}
T_END;

TEST( CrazyEdgeCase2, PitchData )
{
    auto input = Input{};
    input.step = Step::e;
    input.alter = 21;
    input.cents = 100.01;
    input.accidental = Accidental::sori;
    const std::string expectedAlterString = "22.0001";
    const int expectedAlter = 22;
    const double expectedCents = 0.01;
    const Accidental expectedAccidental = input.accidental;
    const auto output = pitchDataTest( input );

    CHECK_EQUAL( expectedAlterString, output.alterString );
    CHECK_EQUAL( expectedAlterString, output.secondAlterString );
    CHECK_EQUAL( expectedAlter, output.alter );
    CHECK_DOUBLES_EQUAL( expectedCents, output.cents, MX_API_EQUALITY_EPSILON );
    CHECK( expectedAccidental == output.accidental );
}
T_END;

#endif
