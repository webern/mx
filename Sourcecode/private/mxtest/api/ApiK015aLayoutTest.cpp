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
#include "mxtest/api/ApiK015aLayoutScoreData.h"

using namespace mx::api;

namespace mxtest
{
    // TODO - finish writing out the score data for this test in ApiK015aLayoutScoreData.h
//	constexpr const char* const testFileName = "k015a_System_Layout.xml";
//    
//    class ApiK015aLayoutScoreData : public ScoreDataCreator
//    {
//        virtual mx::api::ScoreData createScoreData() const override
//        {
//            return apiK015aLayoutScoreData();
//        }
//    };
//    
//    class ApiK015aLayoutScoreData;
//    
//    static const ApiTester ApiK015aLayoutTest
//    {
//        ScoreDataCreatorPtr{ new ApiK015aLayoutScoreData{} },
//        MxFileRepository::getTestFile( testFileName ),
//        __FILE__,
//        __LINE__
//    };
}

#endif
