
#include "mxtest/api/ApiTester.h"
#include "mx/api/DocumentManager.h"
#include "mx/xml/XFactory.h"
#include "mx/xml/XDoc.h"
#include "cpul/cpulFailure.h"
#include "mxtest/file/StupidFileFunctions.h"

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
        auto initialLoadDocId = docMgr.createFromFile( testFilePath() );
        const auto initialLoadScoreData = docMgr.getData( initialLoadDocId );
        
        // save what we loaded back to disk
        auto initialScoreDataDocId = docMgr.createFromScore( initialLoadScoreData );
        std::stringstream intermediateFilePathSstr;
        intermediateFilePathSstr << "./" << testFileNamePart();
        intermediateFilePathSstr << ".intermediate.xml";
        const std::string intermediateFilePath = intermediateFilePathSstr.str();
        
        // save the 'intermediate' ScoreData
        docMgr.writeToFile( initialScoreDataDocId, intermediateFilePath );
        
        // load what what we just saved back up into memory
        auto intermediateFileLoadDocId = docMgr.createFromFile( intermediateFilePath );
        const auto actualScoreData = docMgr.getData( intermediateFileLoadDocId );
        
        const bool areEqual = ( expectedScoreData == actualScoreData );
        if( areEqual )
        {
            // test was successful, return without registering a failure
            setIsSuccess( true );
            docMgr.destroyDocument( initialLoadDocId );
            docMgr.destroyDocument( initialScoreDataDocId );
            docMgr.destroyDocument( intermediateFileLoadDocId );
            // delete the intermediate score data.
            deleteFileNoThrow( intermediateFilePath );
            return;
        }
        
        // test was not successful
        setIsSuccess( false );
        setFailureMessage( "after round-trips to disk the ScoreData objects were not equal" );
        
        // save a "scrubbed" version of the input
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
        auto expectedScoeDataDocId = docMgr.createFromScore( expectedScoreData );
        docMgr.writeToFile( expectedScoeDataDocId, expectedXmlFilePath.str() );
        
        // save the 'actual' ScoreData
        std::stringstream actualXmlFilePath;
        actualXmlFilePath << "./" << testFileNamePart();
        actualXmlFilePath << ".final.xml";
        auto finalDocId = docMgr.createFromScore( actualScoreData );
        docMgr.writeToFile( finalDocId, actualXmlFilePath.str() );
        
        docMgr.destroyDocument( initialLoadDocId );
        docMgr.destroyDocument( initialScoreDataDocId );
        docMgr.destroyDocument( intermediateFileLoadDocId );
        docMgr.destroyDocument( expectedScoeDataDocId );
        docMgr.destroyDocument( finalDocId );
        
    }
}
