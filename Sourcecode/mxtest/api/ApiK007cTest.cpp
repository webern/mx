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
#include "mxtest/api/ApiK007cScoreData.h"

using namespace mx::api;

namespace mxtest
{
	constexpr const char* const testFileName = "k007c_Directions_Dynamics.xml";
    
    class ApiK007cScoreData : public ScoreDataCreator
    {
        virtual mx::api::ScoreData createScoreData() const override
        {
            return apiK007cScoreData();
        }
    };
    
    class ApiK007cScoreData;
    
    static const ApiTester ApiK007cTest
    {
        ScoreDataCreatorPtr{ new ApiK007cScoreData{} },
        MxFileRepository::getTestFile( testFileName ),
        __FILE__,
        __LINE__
    };
}

#endif
