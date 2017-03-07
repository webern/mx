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


TEST( x, Freezing )
{
    const int partIndex = 1; // bass clarinet
    const int staffIndex = 0;
    const int measureIndex = 85;
    const int directionIndex = 0;

    const std::string fileName{ "freezing.xml" };
    const std::string path{ MxFileRepository::getFullPath( fileName ) };
    auto& docMgr = mx::api::DocumentManager::getInstance();
    auto docId = docMgr.createFromFile( path );
    auto scoreData = docMgr.getData( docId );
    docMgr.destroyDocument( docId );

    const auto& part = scoreData.parts.at( partIndex );
    const auto& measure = part.measures.at( measureIndex );
    const auto& staff = measure.staves.at( staffIndex );
    const auto& direction = staff.directions.at( directionIndex);
//    const auto& wedge = direction.wedgeStarts.at( 0 );

    CHECK_EQUAL( 72, direction.tickTimePosition );
    CHECK( Placement::below == direction.placement );
    CHECK( direction.isOffsetSpecified );
    CHECK_EQUAL( 5, direction.offset );

}
T_END

#endif
