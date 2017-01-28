// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/file/MxFileRepository.h"
#include "mx/api/DocumentManager.h"

using namespace std;
using namespace mx::api;
using namespace mxtest;

namespace
{
    constexpr const char* const fileName = "testAccidentals1.xml";
}


TEST( implicitCarryover, TimeSignatureApi )
{
    const auto scoreData = mxtest::MxFileRepository::loadFile( fileName );
    CHECK_EQUAL( 1, scoreData.parts.size() );
    const auto& part = scoreData.parts.front();
    CHECK_EQUAL( 2, part.measures.size() )
    auto measureIter = part.measures.cbegin();
    auto t = measureIter->timeSignature;
    
    CHECK( !t.isImplicit );
    CHECK_EQUAL( 3, t.beats );
    CHECK_EQUAL( 4, t.beatType );
    CHECK( t.symbol == TimeSignatureSymbol::unspecified );
    
    ++measureIter;
    t = measureIter->timeSignature;
    CHECK( t.isImplicit );
    CHECK_EQUAL( 3, t.beats );
    CHECK_EQUAL( 4, t.beatType );
    CHECK( t.symbol == TimeSignatureSymbol::unspecified );
}
T_END

#endif
