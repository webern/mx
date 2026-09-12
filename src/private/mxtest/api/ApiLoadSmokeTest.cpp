// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/MusicXml.h"
#include "mxtest/file/MxFileTest.h"
#include "mxtest/file/MxFileTestGroup.h"

namespace mxtest
{
// Smoke test: proves each registered file imports and produces at least one part
// (createFromFile -> getData succeeds with a non-empty ScoreData::parts), NOT that the
// data is correct -- dropped or mistranslated elements still pass. Fidelity is the
// round-trip gate (roundtrip-baseline.txt).
class ApiLoadSmokeTest : public mxtest::MxFileTest
{
  public:
    ApiLoadSmokeTest(mxtest::MxFile inTestFile, std::string inTestName, std::string inTestCppFileName,
                     int inTestCppFileLineNumber)
        : mxtest::MxFileTest(inTestFile, inTestName, inTestCppFileName, inTestCppFileLineNumber)
    {
    }

    inline void runTestCode()
    {
        if (isLoadFailureExpected())
        {
            setIsSuccess(true);
            return;
        }
        auto docResult = mx::api::MusicXml::fromFile(testFilePath());
        if (!docResult.ok())
        {
            setIsSuccess(false);
            setFailureMessage("MusicXml::fromFile failed: " + docResult.error().message);
            return;
        }
        const auto scoreDataResult = mx::api::getScore(std::move(docResult).value());
        if (!scoreDataResult.ok())
        {
            setIsSuccess(false);
            setFailureMessage("getScore failed: " + scoreDataResult.error().message);
            return;
        }
        bool isSuccess = scoreDataResult.value().parts.size() > 0;
        setIsSuccess(isSuccess);
        setFailureMessage("scoreData has no parts");
    }
};

MxFileTestGroup<ApiLoadSmokeTest> instance{MX_COMPILE_MAX_FILE_SIZE_BYTES, // maxFileSizeBytes
                                           "Api Load Smoke Test", __FILE__, __LINE__};
} // namespace mxtest

#endif
