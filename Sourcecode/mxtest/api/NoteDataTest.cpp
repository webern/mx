// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/api/RoundTrip.h"
#include "mx/api/DocumentManager.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;


TEST( miscFields, NoteData )
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
    note.miscData.push_back( "Hello,There" );
    note.miscData.push_back( "Bones" );
    note.miscData.push_back( "Bishop" );

    // round trip it through xml
    auto& mgr = DocumentManager::getInstance();
    auto docId = mgr.createFromScore( score );
    std::stringstream ss;
    mgr.writeToStream(docId, ss);
    mgr.destroyDocument(docId);
    const std::string xml = ss.str();
    std::istringstream iss{ xml };
    docId = mgr.createFromStream( iss );
    auto oscore = mgr.getData(docId);

    // get the data after the round trip
    auto& opart = oscore.parts.back();
    auto& omeasure = opart.measures.back();
    auto& ostaff = omeasure.staves.back();
    auto& ovoice = ostaff.voices[0];
    auto& onote = ovoice.notes.back();
    auto& omisc = onote.miscData;
    auto iter = omisc.cbegin();
    const auto end = omisc.cend();

    std::cout << xml << std::endl;

    // assert
    CHECK( iter != end );
    CHECK_EQUAL( "Hello_There", *iter);
    ++iter;

    CHECK( iter != end );
    CHECK_EQUAL( "Bones", *iter);
    ++iter;

    CHECK( iter != end );
    CHECK_EQUAL( "Bishop", *iter);
    ++iter;

    CHECK( iter == end );
}
T_END

#endif
