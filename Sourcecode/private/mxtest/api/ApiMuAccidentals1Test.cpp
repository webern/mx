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
#include "mxtest/api/ApiMuAccidentals1ScoreData.h"

using namespace mx::api;

namespace mxtest
{
	constexpr const char* const testFileName = "testAccidentals1.xml";
    
    class ApiMuAccidentals1ScoreData : public ScoreDataCreator
    {
        virtual mx::api::ScoreData createScoreData() const override
        {
            return apiMuAccidentals1ScoreData();
        }
    };
    
    class ApiMuAccidentals1ScoreData;
    
    static const ApiTester ApiMuAccidentals1Test
    {
        ScoreDataCreatorPtr{ new ApiMuAccidentals1ScoreData{} },
        MxFileRepository::getTestFile( testFileName ),
        __FILE__,
        __LINE__
    };
}

#endif
