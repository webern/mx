// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPORT_TESTS

#include "ImportTestImpl.h"
#include "mx/xml/XFactory.h"
#include "mxtest/file/StupidFileFunctions.h"
#include "mxtest/file/Path.h"
#include "mxtest/import/ExpectedFiles.h"
#include "mxtest/import/SortAttributes.h"

#include <fstream>

using namespace mx::xml;
using namespace mx::core;

namespace mxtest
{
    
    std::string ImportRoundTripResult::getHeaderRow()
    {
        std::stringstream ss;
        ss << "testName" << ",";
        ss << "testResult" << ",";
        ss << "percentMatch" << ",";
        ss << "totalMs" << ",";
        ss << "xdocLoadMs" << ",";
        ss << "mxLoadMs" << ",";
        ss << "mxSaveMs" << ",";
        ss << "strCompareMs" << ",";
        ss << "xdocSaveMs" << ",";
        ss << "inputFileName" << ",";
        ss << "message";
        return ss.str();
    }
    
    
    std::string ImportRoundTripResult::getMessageNoCommas() const
    {
        std::stringstream ss;
        for( auto c = message.cbegin(); c != message.cend(); ++c )
        {
            if( *c != ',' && *c != '\n' && *c != '\r' && *c != '\t' && *c != ';' )
            {
                ss << (*c);
            }
            else
            {
                ss << "_";
            }
        }
        return ss.str().substr( 0, 128 );
    }
    
    void ImportRoundTripResult::writeRow( std::ostream& os ) const
    {
        os << testName << ",";
        os << testResult << ",";
        os.precision(4);
        os << percentMatch << ",";
        os << totalMs << ",";
        os << xdocLoadMs << ",";
        os << mxLoadMs << ",";
        os << mxSaveMs << ",";
        os << strCompareMs << ",";
        os << xdocSaveMs << ",";
        os << inputFileName << ",";
        os << getMessageNoCommas() << std::endl;
    }
    
    
    ImportRoundTripTest::ImportRoundTripTest(
        const char* const paramInputFileNamePart,
        const char* const paramInputFileExtensionPart,
        const char* const paramInputSuiteName,
        const char* const paramTestCppFileName,
        const int paramTestCppLineNum,
        const char* const paramResourceDirectoryPath )
    : myInputFileNamePart( paramInputFileNamePart )
    , myInputFileExtensionPart( paramInputFileExtensionPart )
    , myInputSuiteName( paramInputSuiteName )
    , myTestCppFileName( paramTestCppFileName )
    , myTestCppLineNum( paramTestCppLineNum )
    , myResourceDirectoryPath( paramResourceDirectoryPath )
    , myTestTimer()
    , myXdocLoadTimer()
    , myMxDeserializeTimer()
    , myMxReserializeTimer()
    , myTestCompareTimer()
    , myXdocWriteTimer()
    , myIsMxBypassed( false )
    {
        
    }
    
    bool ImportRoundTripTest::runTest( std::stringstream& outMessage )
    {
        std::string inputFilePath = filePath( myInputSuiteName, getFileName() );
        std::string expectedFilePath = getExpectedFileFullPath( myInputSuiteName, getFileName() );
        
        auto inputXDoc = XFactory::makeXDoc();
        auto expectedXDoc = XFactory::makeXDoc();
        auto reserializedXDoc = XFactory::makeXDoc();
        auto resultantMxDoc = makeDocument();
        
        myTestTimer.start();
        
        if( !loadTestFile( outMessage, inputXDoc, inputFilePath ) )
        {
            return false;
        }
        
        myXdocLoadTimer.start();
        
        if( !loadExpectedFile( outMessage, expectedXDoc, expectedFilePath ) )
        {
            return false;
        }
        
        myXdocLoadTimer.stop();
        myMxDeserializeTimer.start();
        
        bool isLoadIntoMxClassesSuccessful = false;
        isLoadIntoMxClassesSuccessful = loadIntoMxClasses( outMessage, inputXDoc, resultantMxDoc );
        
        myMxDeserializeTimer.stop();
        myMxReserializeTimer.start();
        
        if( myIsMxBypassed )
        {
            loadExpectedFile( outMessage, reserializedXDoc, expectedFilePath );
        }
        else
        {
            if( !reserializeToXDoc( outMessage, resultantMxDoc, reserializedXDoc ) )
            {
                return false;
            }
        }
        
        myMxReserializeTimer.stop();
        myTestCompareTimer.start();
        
        std::stringstream expectedSstr;
        std::stringstream actualSstr;
        expectedXDoc->saveStream( expectedSstr );
        reserializedXDoc->saveStream( actualSstr );
        std::string expected = expectedSstr.str();
        std::string actual = actualSstr.str();
        double percentMatch = 0.0;
        auto isCheckEqualSuccessful = checkEqual( outMessage, expected, actual, percentMatch );
        
        
        myTestCompareTimer.stop();
        myXdocWriteTimer.start();
        
        if( !writeErrorFiles( isCheckEqualSuccessful, expected, actual ) )
        {
            return false;
        }

        myXdocWriteTimer.stop();
        auto resultInfo = createInfo( outMessage.str(), isCheckEqualSuccessful, percentMatch );
        myTestTimer.stop();
        
        if( !writeResultToOutputFile( resultInfo ) )
        {
            return false;
        }

        
        return isCheckEqualSuccessful && isLoadIntoMxClassesSuccessful;
    }
    
    std::string ImportRoundTripTest::getTestName() const
    {
        std::stringstream name;
        name << myInputSuiteName << "_" << myInputFileNamePart;
        return name.str();
    }
    
    
    std::string ImportRoundTripTest::getFileName() const
    {
        return myInputFileNamePart + "." + myInputFileExtensionPart;
    }
    
    
    std::ostream& ImportRoundTripTest::streamValues( std::ostream& os ) const
    {
        os << "inputFileNamePart: " << myInputFileNamePart << ", ";
        os << "inputFileExtensionPart: " << myInputFileExtensionPart << ", ";
        os << "inputSuiteName: " << myInputSuiteName << ", ";
        os << "testCppFileName: " << myTestCppFileName << ", ";
        os << "testCppLineNum: " << myTestCppLineNum << ", ";
        os << "resourceDirectoryPath: " << myResourceDirectoryPath;
        return os;
    }
    
    void ImportRoundTripTest::setIsMxBypassed( bool value )
    {
        myIsMxBypassed = value;
    }
    
    
    bool ImportRoundTripTest::loadTestFile( std::ostream& msg, mx::xml::XDocPtr& outXDoc, const std::string& filepath )
    {
        try
        {
            outXDoc->loadFile( filepath );
            auto root = outXDoc->getRoot();
            
            for( auto it = root->attributesBegin(); it != root->attributesEnd(); ++it )
            {
                if( it->getName() == "version" )
                {
                    it->setValue( "3.0" );
                    return true;
                }
            }

            root->appendAttribute( "version" )->setValue( "3.0" );

        }
        catch ( std::exception& e )
        {
            msg << "exception occurred while loading xml file '" << filepath << "', exception message '" << e.what() << "'";
            return false;
        }
        return true;
    }
    
    
    bool ImportRoundTripTest::loadExpectedFile( std::ostream& msg, mx::xml::XDocPtr& outXDoc, const std::string& filepath )
    {
        try
        {
            outXDoc->loadFile( filepath );
        }
        catch ( std::exception& e )
        {
            msg << "exception occurred while loading xml file '" << filepath << "', exception message '" << e.what() << "'";
            return false;
        }
        return true;
    }
    
    
    bool ImportRoundTripTest::loadIntoMxClasses( std::ostream& msg, const mx::xml::XDocPtr& inXdoc, mx::core::DocumentPtr& outMx )
    {
        if( myIsMxBypassed )
        {
            return true;
        }


        std::stringstream xdocMsg;
        bool isSuccess = false;
        try
        {
            isSuccess = outMx->fromXDoc( xdocMsg, *inXdoc );
        }
        catch ( std::exception& e )
        {
            msg << "exception occurred in fromXDoc '" << e.what() << "' ";
            msg << "additional messages from xdoc, if any '" << xdocMsg.str() << "'";
            return false;
        }
        if( !isSuccess )
        {
            msg << xdocMsg.str();
        }
        return isSuccess;
    }
    
    
    bool ImportRoundTripTest::reserializeToXDoc( std::ostream& msg, const mx::core::DocumentPtr& inMx, mx::xml::XDocPtr& outXDoc )
    {
        if( myIsMxBypassed )
        {
            return true;
        }
        
        try
        {
            inMx->toXDoc( *outXDoc );
            
            // because attribute order doesn't matter in xml, we need to sort them alphabetically before and after round trip
            sortAttributes( *outXDoc );
        }
        catch ( std::exception& e )
        {
            msg << "exception occurred in toXDoc '" << e.what() << "'";
            
            const std::string filename = getTestOutputErrorFileFullPath( myInputSuiteName, getFileName() );
            
            std::ofstream errorFile;
            openOutputFile( errorFile, filename );
            inMx->toStream( errorFile );
            errorFile.close();
            
            return false;
        }
        return true;
    }
    
    
    bool ImportRoundTripTest::checkEqual( std::ostream& msg, const std::string& inExpected, const std::string& inActual, double& outPercentMatch )
    {
        outPercentMatch = 0.0;
        std::vector<std::pair<int, int>> failurePositions;
        int charPosition = 1;
        int lineNumber = 1;
        int totalCharCount = 0;
        int failedCharCount = 0;
        int matchingCharCount = 0;
        bool isEqual = true;

        std::string::const_iterator eit = inExpected.cbegin();
        std::string::const_iterator ait = inActual.cbegin();
        std::string::const_iterator eend = inExpected.cend();
        std::string::const_iterator aend = inActual.cend();
        bool isExpectedLongerThanActual = false;
        bool isActualLongerThanExpected = false;
        for( ; ( eit != eend ) && ( ait != aend ); ++eit, ++ait )
        {
            bool eIsEnd = ( eit == eend );
            bool aIsEnd = ( ait == aend );
            
            if( !eIsEnd )
            {
                ++totalCharCount;
            }
            
            if( eIsEnd && !aIsEnd )
            {
                ++failedCharCount;
                isEqual = false;
                
                if( !isActualLongerThanExpected )
                {
                    msg << "actual has more characters than expected ";
                }
                
                isActualLongerThanExpected = true;
                continue;
            }
            
            if( !eIsEnd && aIsEnd )
            {
                ++failedCharCount;
                isEqual = false;
                
                if( !isExpectedLongerThanActual )
                {
                    msg << "actual has fewer characters than expected ";
                }
                
                isExpectedLongerThanActual = true;
                continue;
            }
            
            if( eIsEnd || aIsEnd )
            {
                continue;
            }
            
            char e = *eit;
            char a = *ait;
            
            if( e == '\n' )
            {
                ++lineNumber;
                charPosition = 0;
            }
            else
            {
                ++charPosition;
            }
            
            if( e != a )
            {
                isEqual = false;
                failurePositions.push_back( std::pair<int,int>( lineNumber, charPosition ) );
                if( failurePositions.size() == 1 )
                {
                    msg << getTestName() << " ";
                    msg << "first non matching char found at lineNumber " << lineNumber;
                    msg << ", charPosition " << charPosition << ". expected '" << e << "' ";
                    msg << "but was '" << a << "' ";
                }
                ++failedCharCount;
            }
            else
            {
                ++matchingCharCount;
            }
        }
        
        if( inExpected.size() > 0 )
        {
            outPercentMatch = ( static_cast<double>( matchingCharCount ) / static_cast<double>( inExpected.size() ) );
        }
        
        if( !isEqual || failurePositions.size() > 0 || failedCharCount > 0 )
        {
            msg << "total expected characters " << inExpected.size() << ", total matching characters " << matchingCharCount << ", ";
            msg << "match percent " << outPercentMatch << " ";
        }
        
        return isEqual && failurePositions.size() == 0 && failedCharCount == 0;
    }
    
    
    ImportRoundTripResult ImportRoundTripTest::createInfo( const std::string& msg, bool success, const double percentMatch )
    {
        ImportRoundTripResult result;
        result.testName = getTestName();
        result.testResult = success ? "Passed" : "Failed";
        result.percentMatch = percentMatch;
        result.totalMs = myTestTimer.getMilliseconds();
        result.xdocLoadMs = myXdocLoadTimer.getMilliseconds();
        result.mxLoadMs = myMxDeserializeTimer.getMilliseconds();
        result.mxSaveMs = myMxReserializeTimer.getMilliseconds();
        result.strCompareMs = myTestCompareTimer.getMilliseconds();
        result.xdocSaveMs = myXdocWriteTimer.getMilliseconds();
        result.inputFileName = getFileName();
        result.message = msg;
        return result;
    }
    
    
    bool ImportRoundTripTest::writeResultToOutputFile( const ImportRoundTripResult& info )
    {
        std::ofstream ofile( getTestRecordFilepath(), std::ios::app );
        if( !ofile.is_open() )
        {
            return false;
        }
        ofile.setf( std::ios::fixed, std:: ios::floatfield );
        info.writeRow( ofile );
        ofile.close();
        return true;
    }
    
    
    bool ImportRoundTripTest::writeErrorFiles( bool isSuccess, const std::string& expectedContents, const std::string& actualContents )
    {
        if( isSuccess )
        {
            // no need to write the files
            return true;
        }


        std::string testOutputErrorFilePath = getTestOutputErrorFileFullPath( myInputSuiteName, getFileName() );
        std::string testOutputExpectedFilePath = getTestOutputExpectedFileFullPath( myInputSuiteName, getFileName() );
        
        std::ofstream errorFile;
        openOutputFile( errorFile, testOutputErrorFilePath );
        errorFile << actualContents;
        errorFile.close();
        
        std::ofstream expectedFile;
        openOutputFile( expectedFile, testOutputExpectedFilePath );
        expectedFile << expectedContents;
        expectedFile.close();
        
        return true;
    }

}

#endif
