// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "mxtest/api/ApiTester.h"
#include "mxtest/file/MxFileTest.h"
#include "mxtest/file/MxFileTestGroup.h"
#include "mx/api/DocumentManager.h"
#include "cpul/cpulTestHarness.h"
#include "mx/api/ScoreData.h"
#include "mxtest/api/ApiTester.h"
#include "mxtest/api/ApiChordSimpleScoreData.h"

using namespace mx::api;

namespace mxtest
{
    constexpr const char* const testFileName = "chords_simple.xml";

    class ApiChordSimpleScoreData : public ScoreDataCreator
    {
        virtual mx::api::ScoreData createScoreData() const override
        {
            return apiChordSimpleScoreData();
        }
    };

    class ApiChordSimpleScoreData;

    static const ApiTester ApiChordSimpleTest
    {
        ScoreDataCreatorPtr{ new ApiChordSimpleScoreData{} },
        MxFileRepository::getTestFile( testFileName ),
        __FILE__,
        __LINE__
    };
}

#endif
