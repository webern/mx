// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPL_TESTS

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
