// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "mxtest/api/ApiTester.h"
#include "mxtest/file/MxFileTest.h"
#include "mxtest/file/MxFileTestGroup.h"
#include "mx/api/DocumentManager.h"
#include "cpul/testFramework.h"
#include "mx/api/ScoreData.h"
#include "mxtest/api/ApiTester.h"
#include "mxtest/api/ApiK014aFermatasScoreData.h"

using namespace mx::api;

namespace mxtest
{
	constexpr const char* const testFileName = "k014a_Fermatas.xml";
    
    class ApiK014aFermatasScoreData : public ScoreDataCreator
    {
        virtual mx::api::ScoreData createScoreData() const override
        {
            return apiK014aFermatasScoreData();
        }
    };
    
    class ApiK014aFermatasScoreData;
    
    static const ApiTester ApiK014aFermatasTest
    {
        ScoreDataCreatorPtr{ new ApiK014aFermatasScoreData{} },
        MxFileRepository::getTestFile( testFileName ),
        __FILE__,
        __LINE__
    };
}

#endif
