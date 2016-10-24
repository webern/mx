// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/impl/ScoreWriter.h"

using namespace mx;
using namespace mx::impl;

// TODO - make real tests

TEST( testName, ScoreWriter)
{
    api::ScoreData scoreData;
    ScoreWriter scoreWriter{ scoreData };
    const auto scorePartwise = scoreWriter.getScorePartwise();
    CHECK( scorePartwise != nullptr );
}
T_END

#endif
