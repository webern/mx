
#include "mxtest/api/ApiTester.h"
#include "mx/api/DocumentManager.h"
#include "ezxml/XFactory.h"
#include "ezxml/XDoc.h"
//#include "cpul/cpulFailure.h"
#include "mxtest/file/StupidFileFunctions.h"

#include <sstream>

namespace mxtest
{
    ApiTester::ApiTester(
		ScoreDataCreatorPtr&& inScoreDataCreator,
		MxFile inFile,
	  	std::string cppFileName,
        int cppFileLineNumber)
    : MxFileTest{ inFile, "ApiRoundTripTest" , cppFileName, cppFileLineNumber }
    , myScoreDataCreator{ std::move( inScoreDataCreator ) }
    , myIntermediateFilePath{}
    , myScrubbedFilePath{}
    , myExpectedFilePath{}
    , myFinalFilePath{}
    {
        std::stringstream ss;
        ss << "./apitester." << testFileNamePart() << ".intermediate.xml";
        myIntermediateFilePath = ss.str();
        
        ss.str( "" );
        ss << "./apitester." << testFileNamePart() << ".input.xml";
        myScrubbedFilePath = ss.str();
        
        ss.str( "" );
        ss << "./apitester." << testFileNamePart() << ".expected.xml";
        myExpectedFilePath = ss.str();

        ss.str( "" );
        ss << "./apitester." << testFileNamePart() << ".final.xml";
        myFinalFilePath = ss.str();
        
        deleteFiles();
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
        
        // save the 'intermediate' ScoreData
        docMgr.writeToFile( initialScoreDataDocId, myIntermediateFilePath );
        
        // load what what we just saved back up into memory
        auto intermediateFileLoadDocId = docMgr.createFromFile( myIntermediateFilePath );
        const auto actualScoreData = docMgr.getData( intermediateFileLoadDocId );
        
        const bool areEqual = ( expectedScoreData == actualScoreData );
        if( areEqual )
        {
            // test was successful, return without registering a failure
            setIsSuccess( true );
            docMgr.destroyDocument( initialLoadDocId );
            docMgr.destroyDocument( initialScoreDataDocId );
            docMgr.destroyDocument( intermediateFileLoadDocId );
            deleteFiles();
            return;
        }
        
        // test was not successful
        setIsSuccess( false );
        setFailureMessage( "after round-trips to disk the ScoreData objects were not equal" );
        
        // save a "scrubbed" version of the input
        auto xdoc = ::ezxml::XFactory::makeXDoc();
        xdoc->loadFile( testFilePath() );
        xdoc->saveFile( myScrubbedFilePath );
        
        // save the 'expected' ScoreData
        auto expectedScoeDataDocId = docMgr.createFromScore( expectedScoreData );
        docMgr.writeToFile( expectedScoeDataDocId, myExpectedFilePath );
        
        // save the 'actual' ScoreData
        auto finalDocId = docMgr.createFromScore( actualScoreData );
        docMgr.writeToFile( finalDocId, myFinalFilePath );
        
        docMgr.destroyDocument( initialLoadDocId );
        docMgr.destroyDocument( initialScoreDataDocId );
        docMgr.destroyDocument( intermediateFileLoadDocId );
        docMgr.destroyDocument( expectedScoeDataDocId );
        docMgr.destroyDocument( finalDocId );
        
    }
        
    void ApiTester::deleteFiles() const
    {
        deleteFileNoThrow( myIntermediateFilePath );
        deleteFileNoThrow( myScrubbedFilePath );
        deleteFileNoThrow( myExpectedFilePath );
        deleteFileNoThrow( myFinalFilePath );
    }
}
