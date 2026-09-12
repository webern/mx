// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/api/ApiTester.h"
#include "mx/api/MusicXml.h"
#include "mxtest/file/StupidFileFunctions.h"
#include "pugixml.hpp"

#include <sstream>

namespace mxtest
{
ApiTester::ApiTester(ScoreDataCreatorPtr &&inScoreDataCreator, MxFile inFile, std::string cppFileName,
                     int cppFileLineNumber)
    : MxFileTest{inFile, "ApiRoundTripTest", cppFileName, cppFileLineNumber},
      myScoreDataCreator{std::move(inScoreDataCreator)}, myIntermediateFilePath{}, myScrubbedFilePath{},
      myExpectedFilePath{}, myFinalFilePath{}
{
    std::stringstream ss;
    ss << "./apitester." << testFileNamePart() << ".intermediate.xml";
    myIntermediateFilePath = ss.str();

    ss.str("");
    ss << "./apitester." << testFileNamePart() << ".input.xml";
    myScrubbedFilePath = ss.str();

    ss.str("");
    ss << "./apitester." << testFileNamePart() << ".expected.xml";
    myExpectedFilePath = ss.str();

    ss.str("");
    ss << "./apitester." << testFileNamePart() << ".final.xml";
    myFinalFilePath = ss.str();

    deleteFiles();
}

void ApiTester::runTestCode()
{
    using namespace mx::api;
    const auto expectedScoreData = myScoreDataCreator->createScoreData();

    // load the data from disk
    auto initialLoadDocResult = MusicXml::fromFile(testFilePath());
    if (!initialLoadDocResult.ok())
    {
        setIsSuccess(false);
        setFailureMessage("fromFile failed: " + initialLoadDocResult.error().message);
        return;
    }
    MusicXml initialLoadDoc = std::move(initialLoadDocResult).value();
    const auto initialLoadScoreDataResult = getScore(initialLoadDoc);
    if (!initialLoadScoreDataResult.ok())
    {
        setIsSuccess(false);
        setFailureMessage("getScore failed: " + initialLoadScoreDataResult.error().message);
        return;
    }
    const auto initialLoadScoreData = initialLoadScoreDataResult.value();

    // save what we loaded back to disk
    auto initialScoreDataDocResult = fromScore(initialLoadScoreData);
    if (!initialScoreDataDocResult.ok())
    {
        setIsSuccess(false);
        setFailureMessage("fromScore failed");
        return;
    }
    const MusicXml initialScoreDataDoc = std::move(initialScoreDataDocResult).value();

    // save the 'intermediate' ScoreData
    initialScoreDataDoc.writeToFile(myIntermediateFilePath);

    // load what we just saved back into memory
    auto intermediateFileLoadDocResult = MusicXml::fromFile(myIntermediateFilePath);
    if (!intermediateFileLoadDocResult.ok())
    {
        setIsSuccess(false);
        setFailureMessage("fromFile(intermediate) failed");
        return;
    }
    const auto actualScoreDataResult = getScore(std::move(intermediateFileLoadDocResult).value());
    if (!actualScoreDataResult.ok())
    {
        setIsSuccess(false);
        setFailureMessage("getScore(intermediate) failed");
        return;
    }
    const auto actualScoreData = actualScoreDataResult.value();

    const bool areEqual = (expectedScoreData == actualScoreData);
    if (areEqual)
    {
        // test was successful, return without registering a failure
        setIsSuccess(true);
        deleteFiles();
        return;
    }

    // test was not successful
    setIsSuccess(false);
    setFailureMessage("after round-trips to disk the ScoreData objects were not equal");

    // save a "scrubbed" version of the input via pugixml round-trip
    pugi::xml_document pDoc;
    if (pDoc.load_file(testFilePath().c_str()))
    {
        pDoc.save_file(myScrubbedFilePath.c_str());
    }

    // save the 'expected' ScoreData
    auto expectedScoreDataDocResult = fromScore(expectedScoreData);
    if (expectedScoreDataDocResult.ok())
    {
        std::move(expectedScoreDataDocResult).value().writeToFile(myExpectedFilePath);
    }

    // save the 'actual' ScoreData
    auto finalDocResult = fromScore(actualScoreData);
    if (finalDocResult.ok())
    {
        std::move(finalDocResult).value().writeToFile(myFinalFilePath);
    }
}

void ApiTester::deleteFiles() const
{
    deleteFileNoThrow(myIntermediateFilePath);
    deleteFileNoThrow(myScrubbedFilePath);
    deleteFileNoThrow(myExpectedFilePath);
    deleteFileNoThrow(myFinalFilePath);
}
} // namespace mxtest
