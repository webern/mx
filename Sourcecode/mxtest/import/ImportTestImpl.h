// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPORT_TESTS

#include "cpul/cpulTestHarness.h"
#include "cpul/cpulTestTimer.h"
#include "cpul/cpulTest.h"
#include "mx/xml/XFactory.h"
#include "mx/xml/XDoc.h"
#include "mxtest/control/Path.h"
#include "mxtest/control/TestFiles.h"
#include "mx/core/Document.h"

#include <iostream>
#include <string>
#include <sstream>

#define MXTEST_IMPORT_ROUNDTRIP(GROUPNAME,FNAME_PART,FEXT,IMPORT_DIR)           \
class GROUPNAME##FNAME_PART##Test : public Test                                 \
{                                                                               \
public:                                                                         \
    GROUPNAME##FNAME_PART##Test ()                                              \
    :Test ("TEST(" #GROUPNAME ", " #FNAME_PART ")", __FILE__, __LINE__) {}      \
void runTest (TestResult& rEsUlT_);                                             \
} GROUPNAME##FNAME_PART##Instance;                                              \
void GROUPNAME##FNAME_PART##Test::runTest (TestResult& rEsUlT_)                 \
{                                                                               \
    MxTest::ImportRoundTripTest test(                                           \
    #FNAME_PART,                                                                \
    #FEXT,                                                                      \
    #GROUPNAME,                                                                 \
    __FILE__,                                                                   \
    __LINE__,                                                                   \
    IMPORT_DIR);                                                                \
                                                                                \
    bool isSuccess = false;                                                     \
    std::stringstream msgsstr;                                                  \
    test.setIsMxBypassed( false );                                              \
                                                                                \
                                                                                \


#define MXTEST_IMPORT_ROUNDTRIP_END }                                           \
                                                                                \


namespace MxTest
{
    
    constexpr const char* const TEST_RECORD_FILENAME = "_MxImportTestResults.csv";
    inline std::string getTestRecordFilepath()
    {
        std::stringstream ss;
        ss << RESOURCES_DIRECTORY_PATH << "testOutput" << FILE_PATH_SEPARATOR;
        ss << TEST_RECORD_FILENAME;
        return ss.str();
    }
    
    struct ImportRoundTripResult
    {
        std::string testName;
        std::string testResult;
        double percentMatch;
        double totalMs;
        double xdocLoadMs;
        double mxLoadMs;
        double mxSaveMs;
        double strCompareMs;
        double xdocSaveMs;
        std::string inputFileName;
        std::string message;

        static std::string getHeaderRow();
        std::string getMessageNoCommas() const;
        void writeRow( std::ostream& os ) const;
    };
    
    class ImportRoundTripTest
    {
    public:
        
        ImportRoundTripTest(
            const char* const paramInputFileNamePart,
            const char* const paramInputFileExtensionPart,
            const char* const paramInputSuiteName,
            const char* const paramTestCppFileName,
            const int paramTestCppLineNum,
            const char* const paramResourceDirectoryPath );

        bool runTest( std::stringstream& outMessage );
        std::string getTestName() const;
        std::string getFileName() const;
        std::ostream& streamValues( std::ostream& os ) const;
        
        void setIsMxBypassed( bool value );
        
    private:
        const std::string myInputFileNamePart;
        const std::string myInputFileExtensionPart;
        const std::string myInputSuiteName;
        const std::string myTestCppFileName;
        const int myTestCppLineNum;
        const std::string myResourceDirectoryPath;
        cpul::TestTimer myTestTimer;
        cpul::TestTimer myXdocLoadTimer;
        cpul::TestTimer myMxDeserializeTimer;
        cpul::TestTimer myMxReserializeTimer;
        cpul::TestTimer myTestCompareTimer;
        cpul::TestTimer myXdocWriteTimer;
        bool myIsMxBypassed;
        
        bool loadTestFile( std::ostream& msg, mx::xml::XDocPtr& outXDoc, const std::string& filepath );
        bool loadExpectedFile( std::ostream& msg, mx::xml::XDocPtr& outXDoc, const std::string& filepath );
        bool loadIntoMxClasses( std::ostream& msg, const mx::xml::XDocPtr& inXdoc, mx::core::DocumentPtr& outMx );
        bool reserializeToXDoc( std::ostream& msg, const mx::core::DocumentPtr& inMx, mx::xml::XDocPtr& outXDoc );
        bool checkEqual( std::ostream& msg, const std::string& inExpected, const std::string& inActual, double& outPercentMatch );
        ImportRoundTripResult createInfo( const std::string& msg, bool success, const double percentMatch );
        bool writeResultToOutputFile( const ImportRoundTripResult& info );
        bool writeErrorFiles( bool isSuccess, const std::string& expectedContents, const std::string& actualContents );
    };
    
    inline std::ostream& operator<<( std::ostream& os, const ImportRoundTripTest& value )
    {
        return value.streamValues( os );
    }
}

class ImportTestCpul : public Test
{
public:
    ImportTestCpul( MxTest::TestFile testFile )
    : Test( "ImportTestCpul", testFile.fileName, 0 )
    , myTestFile{ testFile }
    {}
    
    inline void runTest(TestResult& rEsUlT_)
    {
        MxTest::ImportRoundTripTest test{
        myTestFile.getFileNamePart().c_str(),
        "xml",
        myTestFile.subdirectory.c_str(),
        myTestFile.fileName.c_str(),
        0,
        myTestFile.subdirectory.c_str() };
        
        bool isSuccess = false;
        std::stringstream msgsstr;
        msgsstr << "filename - " << myTestFile.fileName << " ";
        test.setIsMxBypassed( false );
        
        isSuccess = test.runTest( msgsstr );
        if( myTestFile.isLoadFailureExpected )
        {
            CHECK_WITH_MESSAGE( !isSuccess, myTestFile.fileName );
        }
        else
        {
            CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
        }
    }
    
private:
    MxTest::TestFile myTestFile;
};

#endif
