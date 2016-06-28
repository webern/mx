
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPORT_TESTS

#include "mxtest/control/File.h"
#include "mxtest/import/ExpectedFiles.h"
#include "mx/utility/Throw.h"
#include "mx/core/Document.h"

#include <fstream>

using namespace mx::xml;


namespace MxTest
{
    void generateExpectedFile( const std::string& subdir, const std::string& fileName )
    {
        auto xdoc = readInputFile( subdir, fileName );
        setXmlDeclaration( xdoc );
        auto isScorePartwise = getIsScorePartwise( xdoc );
        setDoctype( xdoc, isScorePartwise );
        setRootMusicXmlVersion( xdoc, isScorePartwise );
        std::string expectedFileName = getExpectedFileName( subdir, fileName );
        std::string expectedFilePath = getExpectedFileFullPath( expectedFileName );
        std::ofstream ofile;
        openOutputFile( ofile, expectedFilePath );
        xdoc->write( ofile );
        ofile.close();
    }
    
    mx::xml::XDocPtr readInputFile( const std::string& subdir, const std::string& fileName )
    {
        std::ifstream inputFile;
        std::string fullPath = filePath( subdir, fileName );
        openInputFile( inputFile, fullPath );
        auto xdoc = mx::xml::XFactory::makeXDoc();
        xdoc->parse( inputFile );
        inputFile.close();
        return xdoc;
    }
    
    
    void setXmlDeclaration( mx::xml::XDocPtr& xdoc )
    {
        xdoc->setIsStandalone( false );
        xdoc->setXmlVersion( mx::xml::XmlVersion::onePointZero );
        auto encoding = xdoc->getEncoding();
        switch( encoding )
        {
            case mx::xml::Encoding::utfEight:
                xdoc->setEncoding( mx::xml::Encoding::utfEight );
                break;
            case mx::xml::Encoding::utfSixteen:
                xdoc->setEncoding( mx::xml::Encoding::utfSixteen );
                break;
            case mx::xml::Encoding::unknown:
                xdoc->setEncoding( mx::xml::DEFAULT_ENCODING );
                break;
            default:
                xdoc->setEncoding( mx::xml::DEFAULT_ENCODING );
                break;
        }
    }
    
    
    bool getIsScorePartwise( const mx::xml::XDocPtr& xdoc )
    {
        std::string doctype = xdoc->getDoctypeValue();
        bool isScorePartwise = true;
        if( mx::utility::containsCaseInsensitive( "score-partwise", doctype ) )
        {
            isScorePartwise = true;
        }
        else if( mx::utility::containsCaseInsensitive( "score-timewise", doctype ) )
        {
            isScorePartwise = false;
        }
        else
        {
            std::stringstream ss;
            ss << "the doctype was unrecognized '" << doctype << "'";
            MX_THROW( ss.str() )
        }
        return isScorePartwise;
    }
    
    
    void setDoctype( mx::xml::XDocPtr& xdoc, bool isScorePartwise )
    {
        if( isScorePartwise )
        {
            xdoc->setDoctypeValue( mx::core::DOCTYPE_SCORE_PARTWISE );
        }
        else
        {
            xdoc->setDoctypeValue( mx::core::DOCTYPE_SCORE_TIMEWISE );
        }
    }
    
    
    void setRootMusicXmlVersion( mx::xml::XDocPtr& xdoc, bool isScorePartwise )
    {
        auto root = xdoc->getRoot();
        if ( root->getName() == "score-partwise"  || root->getName() == "score-timewise" )
        {
            if( root->getName() == "score-partwise" && !isScorePartwise )
            {
                MX_THROW( "the root element 'score-partwise' does not match the doctype declaration" );
            }
            
            if( root->getName() == "score-timewise" && isScorePartwise )
            {
                MX_THROW( "the root element 'score-timewise' does not match the doctype declaration" );
            }
            
            auto it = root->attributesBegin();
            auto end = root->attributesEnd();
            if( it == end )
            {
                auto attr = root->appendAttribute( "version" );
                attr->setValue( "3.0" );
            }
            else
            {
                if( it->getName() != "version" )
                {
                    MX_THROW( "attribute of root element is wrong, should be 'version'" );
                }
                it->setValue( "3.0" );
            }
        }
        else
        {
            MX_THROW( "bad music xml file" );
        }
    }
    
    
    std::string getExpectedFileName( const std::string& subdir, const std::string& fileName )
    {
        std::stringstream ss;
        ss << subdir << "_" << fileName << ".expected.xml";
        return ss.str();
    }
    
    
    
    std::string getExpectedFileFullPath( const std::string& subdir, const std::string& fileName )
    {
        return filePath( "expected", getExpectedFileName( subdir, fileName ) );
    }
    
    
    void deleteExpectedFiles()
    {
        for( auto it = TestFiles.cbegin(); it != TestFiles.cend(); ++it )
        {
            std::string fullpath = getExpectedFileFullPath( it->first, it->second );
            deleteFileNoThrow( fullpath );
        }
    }
    
    
    void generateExpectedFiles()
    {
        for( auto it = TestFiles.cbegin(); it != TestFiles.cend(); ++it )
        {
            generateExpectedFile( it->first.c_str(), it->second.c_str() );
        }
        
    }
    

    std::string gettestOutputErrorFileName( const std::string& subdir, const std::string& fileName )
    {
        std::stringstream ss;
        ss << subdir << "_" << fileName << ".error.xml";
        return ss.str();
    }
    
    
    std::string getTestOutputErrorFileFullPath( const std::string& subdir, const std::string& fileName )
    {
        return filePath( TEST_OUTPUT_DIRECTORY_NAME, gettestOutputErrorFileName( subdir, fileName ) );
    }
    
    
    std::string getTestOutputExpectedFileFullPath( const std::string& subdir, const std::string& fileName )
    {
        return filePath( TEST_OUTPUT_DIRECTORY_NAME, getExpectedFileName( subdir, fileName ) );
    }
    
    
    void deleteTestOutputFiles()
    {
        for( auto it = TestFiles.cbegin(); it != TestFiles.cend(); ++it )
        {
            std::string ex = getTestOutputExpectedFileFullPath( it->first, it->second );
            std::string er = getTestOutputErrorFileFullPath( it->first, it->second );
            deleteFileNoThrow( ex );
            deleteFileNoThrow( er );
        }
    }
    
}

#endif