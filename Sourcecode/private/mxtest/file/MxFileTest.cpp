// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/file/MxFileTest.h"

#include <sstream>
#include <utility>

namespace mxtest
{
    MxFileTest::MxFileTest( mxtest::MxFile inTestFile, std::string inTestName, std::string inTestCppFileName, int inTestCppFileLineNumber )
    : myTestFile{ std::move(inTestFile) }
    , myTestName{ std::move(inTestName) }
    , myCppFileName{ std::move(inTestCppFileName) }
    , myCppFileLineNumber{ inTestCppFileLineNumber }
    , myIsSuccess{ false }
    , myFailureMessage{}
    {
        
    }
    
    
    void MxFileTest::runTest()
    {
        bool isExceptionThrown = false;
        std::string exceptionMessage;
        
        try
        {
            this->runTestCode();
        }
        catch ( std::exception& e )
        {
            isExceptionThrown = true;
            exceptionMessage = std::string{ e.what() };
        }
        catch ( ... )
        {
            isExceptionThrown = true;
            exceptionMessage = "exception of unknown (non-standard) type";
        }
        
        if( isExceptionThrown )
        {
            std::stringstream failureMessage;
            failureMessage << "'" << myTestName << "' "  << testFileName() << " exception was thrown '" << exceptionMessage << "'";
            FAIL( failureMessage.str() );
//            ::Failure exceptionFailure
//            {
//                failureMessage.str(), // std::string theCondition
//                myCppFileName,        // std::string theFileName
//                myCppFileLineNumber   // long theLineNumber
//            };
//            testResult.addFailure( exceptionFailure );
        }
        else if( !getIsSuccess() )
        {
            std::stringstream failureMessage;
            failureMessage << "'" << myTestName << "' "  << testFileName() << " '" << getFailureMessage() << "'";

            FAIL( failureMessage.str() );
//            ::Failure testFailure
//            {
//                failureMessage.str(), // std::string theCondition
//                myCppFileName,        // std::string theFileName
//                myCppFileLineNumber   // long theLineNumber
//            };
//            testResult.addFailure( testFailure );
        }
    }
    
    const std::string& MxFileTest::testFileName() const
    {
        return myTestFile.fileName;
    }
    
    
    const std::string MxFileTest::testFileNamePart() const
    {
        return myTestFile.getFileNamePart();
    }
    
    
    const std::string MxFileTest::testFileExtension() const
    {
        return myTestFile.fileName.substr( myTestFile.fileName.size() - 3, 3 );
    }
    
    
    const std::string& MxFileTest::testFilePath() const
    {
        return myTestFile.path;
    }
    
    
    const std::string& MxFileTest::testSubdirectory() const
    {
        return myTestFile.subdirectory;
    }
    
    
    bool MxFileTest::getIsSuccess() const
    {
        return myIsSuccess;
    }
    
    
    void MxFileTest::setIsSuccess( bool inIsSuccess )
    {
        myIsSuccess = inIsSuccess;
    }
    
    
    const std::string& MxFileTest::getFailureMessage() const
    {
        return myFailureMessage;
    }
    
    
    void MxFileTest::setFailureMessage( std::string inMessage )
    {
        myFailureMessage = inMessage;
    }
}
