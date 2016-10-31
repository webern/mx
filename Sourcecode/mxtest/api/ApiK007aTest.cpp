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
#include "mxtest/api/ApiK007aScoreData.h"

using namespace mx::api;

namespace mxtest
{
	constexpr const char* const testFileName = "k007a_Notations_Dynamics.xml";
    
    class ApiK007aScoreData : public ScoreDataCreator
    {
        virtual mx::api::ScoreData createScoreData() const override
        {
            return apiK007aScoreData();
        }
    };
    
    class ApiK007aScoreData;
    
    static const ApiTester ApiK007aTest
    {
        ScoreDataCreatorPtr{ new ApiK007aScoreData{} },
        MxFileRepository::getTestFile( testFileName ),
        __FILE__,
        __LINE__
    };
}

#endif


/*



*/
