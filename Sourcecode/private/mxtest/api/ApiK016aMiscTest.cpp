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
#include "mxtest/api/ApiK016aMiscScoreData.h"

using namespace mx::api;

namespace mxtest
{
	constexpr const char* const testFileName = "k016a_Miscellaneous_Fields.xml";
    
    class ApiK016aMiscScoreData : public ScoreDataCreator
    {
        virtual mx::api::ScoreData createScoreData() const override
        {
            return apiK016aMiscScoreData();
        }
    };
    
    class ApiK016aMiscScoreData;
    
    static const ApiTester ApiK016aMiscTest
    {
        ScoreDataCreatorPtr{ new ApiK016aMiscScoreData{} },
        MxFileRepository::getTestFile( testFileName ),
        __FILE__,
        __LINE__
    };
}

#endif
