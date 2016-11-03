// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "mxtest/api/ApiTester.h"
#include "mxtest/file/MxFileTest.h"
#include "mxtest/file/MxFileTestGroup.h"
#include "mx/api/DocumentManager.h"
#include "cpul/cpulTestHarness.h"
#include "mx/api/ScoreData.h"
#include "mxtest/api/ApiTester.h"
#include "mxtest/api/ApiK009bSlurScoreData.h"

using namespace mx::api;

namespace mxtest
{
	constexpr const char* const testFileName = "k009b_Slur_Attributes.xml";
    
    class ApiK009bSlurAttributesScoreData : public ScoreDataCreator
    {
        virtual mx::api::ScoreData createScoreData() const override
        {
            return apiK009bSlurAttributesScoreData();
        }
    };
    
    class ApiK009bSlurAttributesScoreData;
    
    static const ApiTester ApiK009bSlurAttributesTest
    {
        ScoreDataCreatorPtr{ new ApiK009bSlurAttributesScoreData{} },
        MxFileRepository::getTestFile( testFileName ),
        __FILE__,
        __LINE__
    };
}

#endif
