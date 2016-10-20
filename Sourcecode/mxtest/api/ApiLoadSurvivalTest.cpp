// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "mxtest/file/MxFileTest.h"
#include "mxtest/file/MxFileTestGroup.h"
#include "mx/api/DocumentManager.h"
#include "cpul/cpulTestHarness.h"

namespace mxtest
{
    class ApiLoadSurvivalTest : public mxtest::MxFileTest
    {
    public:
        ApiLoadSurvivalTest(
                       mxtest::MxFile inTestFile,
                       std::string inTestName,
                       std::string inTestCppFileName,
                       int inTestCppFileLineNumber )
        : mxtest::MxFileTest( inTestFile, inTestName, inTestCppFileName, inTestCppFileLineNumber )
        {}

        inline void runTestCode()
        {
            auto& docMgr = mx::api::DocumentManager::getInstance();
            const auto docId = docMgr.createFromFile( testFilePath() );
            const auto scoreData = docMgr.getData( docId );
            docMgr.destroyDocument( docId );
            bool isSuccess = scoreData.parts.size() > 0;
            setIsSuccess( isSuccess );
            setFailureMessage( "either docMgr.createFromFile or docMgr.getData failed" );
        }
    };
    
    MxFileTestGroup<ApiLoadSurvivalTest> instance
    {
        mxtest::MX_COMPILE_MAX_FILE_SIZE_BYTES, // maxFileSizeBytes
        "Api Load Survival Test",
        __FILE__,
        __LINE__
    };
}

#endif
