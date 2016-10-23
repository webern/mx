
#include "mxtest/api/ApiTester.h"
#include "mx/api/DocumentManager.h"
#include "mx/xml/XFactory.h"
#include "mx/xml/XDoc.h"
#include "cpul/cpulFailure.h"

#include <sstream>

namespace mxtest
{
    ApiTester::ApiTester(
		ScoreDataCreatorPtr&& inScoreDataCreator,
		MxFile inFile,
	  	std::string cppFileName,
        int cppFileLineNumber)
    : MxFileTest{ inFile, std::string{ "ApiRoundTripTest_" } + std::string{ inFile.getFileNamePart() }, cppFileName, cppFileLineNumber }
    , myScoreDataCreator{ std::move( inScoreDataCreator ) }
    {

    }

    void ApiTester::runTestCode()
    {
        using namespace mx::api;
        const auto expectedScoreData = myScoreDataCreator->createScoreData();
        
        // load the data from disk
        auto& docMgr = DocumentManager::getInstance();
        auto docId = docMgr.createFromFile( testFilePath() );
        const auto initialLoadScoreData = docMgr.getData( docId );
        docMgr.destroyDocument( docId );
        
        // save what we loaded back to disk
        docId = docMgr.createFromScore( initialLoadScoreData );
        std::stringstream intermediateFilePathSstr;
        intermediateFilePathSstr << "./" << testFileNamePart();
        intermediateFilePathSstr << ".intermediate.xml";
        const std::string intermediateFilePath = intermediateFilePathSstr.str();
        docMgr.writeToFile( docId, intermediateFilePath );
        docMgr.destroyDocument( docId );
        
        // load what what we just saved back up into memory
        docId = docMgr.createFromFile( intermediateFilePath );
        const auto actualScoreData = docMgr.getData( docId );
        docMgr.destroyDocument( docId );
        
        const bool areEqual = ( expectedScoreData == actualScoreData );
        if( areEqual )
        {
            // test was successful, return without registering a failure
            setIsSuccess( true );
            return;
        }
        
        // test was not successful
        setIsSuccess( false );
        setFailureMessage( "after round-trips to disk the ScoreData objects were not equal" );
        
        // save a "scrubbed" version of the imput
        auto xdoc = mx::xml::XFactory::makeXDoc();
        xdoc->loadFile( testFilePath() );
        std::stringstream inputXmlFilePath;
        inputXmlFilePath << "./" << testFileNamePart();
        inputXmlFilePath << ".input.xml";
        xdoc->saveFile( inputXmlFilePath.str() );
        
        // save the 'expected' ScoreData
        std::stringstream expectedXmlFilePath;
        expectedXmlFilePath << "./" << testFileNamePart();
        expectedXmlFilePath << ".expected.xml";
        docId = docMgr.createFromScore( expectedScoreData );
        docMgr.writeToFile( docId, expectedXmlFilePath.str() );
        docMgr.destroyDocument( docId );
        
        // save the 'actual' ScoreData
        std::stringstream actualXmlFilePath;
        actualXmlFilePath << "./" << testFileNamePart();
        actualXmlFilePath << ".final.xml";
        docId = docMgr.createFromScore( actualScoreData );
        docMgr.writeToFile( docId, actualXmlFilePath.str() );
        docMgr.destroyDocument( docId );
    }
}
