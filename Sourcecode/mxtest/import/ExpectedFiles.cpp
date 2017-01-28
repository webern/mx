// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPORT_TESTS

#include "mxtest/file/StupidFileFunctions.h"
#include "mxtest/file/MxFileRepository.h"
#include "mxtest/import/ExpectedFiles.h"
#include "mx/utility/Throw.h"
#include "mx/core/Document.h"
#include "mxtest/import/SortAttributes.h"
#include "mxtest/import/ChangeValues.h"
#include "mxtest/import/DecimalFields.h"

#include <iostream>
#include <fstream>
#include <thread>
#include <future>
#include <list>

using namespace mx::xml;


namespace mxtest
{
    void generateExpectedFile( const std::string& subdir, const std::string& fileName )
    {
        auto xdoc = readInputFile( subdir, fileName );
        setXmlDeclaration( xdoc );
        auto isScorePartwise = getIsScorePartwise( xdoc );
        setDoctype( xdoc, isScorePartwise );
        setRootMusicXmlVersion( xdoc, isScorePartwise );
        
        // ly01e_Pitches_ParenthesizedAccidentals incorrectly uses "double-flat" which should be "flat-flat" instead
        convertValues( *xdoc, "accidental", "double-flat", "flat-flat" );
        
        // lysuite_ly22b_Staff_Notestyles.xml incorrectly has <slash> elements that are missing required attribute
        // 'use-stems' and child element 'slash-type'
        addChildIfNone( *xdoc, "slash", "slash-type", "eighth" );
        
        // ly32b_Atriculations_Texts.xml cas a 'color' attribute on the 'words' element
        // I have examined the xsd thoughouly and I believe this is illegal
        removeAttribute( *xdoc, "words", "color" );
        
        // ly32a_Notations.xml the 'fret' element should have a value
        convertValues( *xdoc, "fret", "", "1" );
        
        // ly32a_Notations.xml the 'string' element should have a value
        convertValues( *xdoc, "string", "", "1" );
        
        // lysuite_ly74a_FiguredBass.xml incorrectly has <figured-bass> elements with zero
        // 'figure' and child elements. one or more 'figure' elements are required
        addChildIfNone( *xdoc, "figured-bass", "figure", "" );
        
        // lysuite_ly41g_PartNoId.xml the part-id attribute is required
        addAttributeIfNone( *xdoc, "part", "id", "ID" );
        
        // several of the musescore test files say <sound dynamics="140.00">
        // my code returns <sound dynamics="140"> which I find acceptable
        stripZerosFromDecimalFields( *xdoc );
        //changeAttributeValue( *xdoc, "sound", "dynamics", "140.00", "140" );
        
        // same as above, mx doesn't write the .00 but musescore does
        //convertValues( *xdoc, "top-system-distance", "170.00", "170" );
        //convertValues( *xdoc, "left-margin", "21.00", "21" );
        //convertValues( *xdoc, "right-margin", "-0.00", "-0" );
        //convertValues( *xdoc, "top-system-distance", "70.00", "70" );
        //convertValues( *xdoc, "staff-distance", "65.00", "65" );
        
        
        //convertValues( *xdoc, "top-system-distance", "275.00", "275" );
        //convertValues( *xdoc, "system-distance", "150.00", "150" );
        //convertValues( *xdoc, "system-distance", "100.00", "100" );

        //changeAttributeValue( *xdoc, "note", "default-y", "-30.00", "-30" );
        //changeAttributeValue( *xdoc, "note", "default-y", "-20.00", "-20" );
        //changeAttributeValue( *xdoc, "note", "default-y", "-40.00", "-40" );
        //changeAttributeValue( *xdoc, "note", "default-y", "-35.00", "-35" );

        //changeAttributeValue( *xdoc, "note", "default-x", "71.00", "71" );
        //changeAttributeValue( *xdoc, "note", "default-x", "211.40", "211.4" );
        //changeAttributeValue( *xdoc, "note", "default-x", "284.50", "284.5" );
        //changeAttributeValue( *xdoc, "note", "default-x", "87.30", "87.3" );

        // ly75a_AccordionRegistrations has incorrect and missing values in the accordion-middle element
        convertValues( *xdoc, "accordion-middle", "test", "0" );
        convertValues( *xdoc, "accordion-middle", "", "0" );
        convertValues( *xdoc, "accordion-middle", "5", "3" );

        // musuite_test_harmony.xml a 'kind' element which has a 'text' attribute consisting of a space character
        // the 'text' attribute is of type XsToken which does not accept spaces
        changeAttributeValue( *xdoc, "kind", "text", " ", "" );

        // musuite_test_harmony.xml has empty 'kind' elements which is incorrect, these will be defaulted to 'major'
        // it also has values that are not allowed in the xsd
        // it also has missing degree-alter tags which are required when 'degree' is used
        convertValues( *xdoc, "kind", "", "none" );
        convertValues( *xdoc, "kind", "minor-major", "none" );
        convertValues( *xdoc, "kind", "dominant-seventh", "dominant" );
        convertValues( *xdoc, "kind", "italian", "Italian" );
        convertValues( *xdoc, "kind", "french", "French" );
        convertValues( *xdoc, "kind", "german", "German" );
        convertValues( *xdoc, "kind", "neapolitan", "Neapolitan" );
        convertValues( *xdoc, "kind", "maj69", "none" );
        convertValues( *xdoc, "kind", "augmented-ninth", "none" );
        convertValues( *xdoc, "kind", "altered", "none" );
        InsertChildAfterIfMissingParams insertDegreeAlterParams;
        insertDegreeAlterParams.elementToInsert = "degree-alter";
        insertDegreeAlterParams.insertAfterElement = "degree-value";
        insertDegreeAlterParams.parentElement = "degree";
        insertDegreeAlterParams.value = "0";
        insertChildAfterIfMissing( insertDegreeAlterParams, *xdoc->getRoot() );
        
        // msuite more values that need the *.00 stripped
        //convertValues( *xdoc, "left-margin", "0.00", "0" );
        //convertValues( *xdoc, "right-margin", "0.00", "0" );
        //changeAttributeValue( *xdoc, "note", "default-y", "-25.00", "-25" );
        //changeAttributeValue( *xdoc, "note", "default-y", "-15.00", "-15" );
        //changeAttributeValue( *xdoc, "note", "default-y", "-5.00", "-5" );
        
        // remove the fake elements from musuite_testInvalid.xml
        removeElement( *xdoc->getRoot(), "invalid_element" );
        removeElement( *xdoc->getRoot(), "another_invalid_element" );
        
        // KEEP AS LAST - attribute order doesn't matter in xml so must be sorted for comparison
        sortAttributes( *xdoc );
        
        std::string expectedFileName = getExpectedFileName( subdir, fileName );
        std::string expectedFilePath = getExpectedFileFullPath( expectedFileName );
        std::ofstream ofile;
        openOutputFile( ofile, expectedFilePath );
        xdoc->saveStream( ofile );
        ofile.close();
    }
    
    
    mx::xml::XDocPtr readInputFile( const std::string& subdir, const std::string& fileName )
    {
        std::ifstream inputFile;
        std::string fullPath = filePath( subdir, fileName );
        openInputFile( inputFile, fullPath );
        auto xdoc = mx::xml::XFactory::makeXDoc();
        xdoc->loadStream( inputFile );
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
            xdoc->setDoctypeValue( mx::core::DOCTYPE_VALUE_SCORE_PARTWISE );
        }
        else
        {
            xdoc->setDoctypeValue( mx::core::DOCTYPE_VALUE_SCORE_TIMEWISE );
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
        std::cout << "deleting expected files" << std::flush;
        auto testFiles = MxFileRepository::getTestFiles( 0 );
        for( auto it = testFiles.cbegin(); it != testFiles.cend(); ++it )
        {
            std::string fullpath = getExpectedFileFullPath( it->subdirectory, it->fileName );
            bool isDeleted = deleteFileNoThrow( fullpath );
            if( isDeleted )
            {
                std::cout << "." << std::flush;
                //std::cout << "deleting expected file - " << fullpath << it->fileName << std::endl;
            }
        }
        std::cout << "done" << std::endl;
    }
    
    
    void generateExpectedFiles()
    {
        std::cout << "creating expected files" << std::flush;
        const int maxConcurrency = 50;
        std::list<std::future<void>> q;
        auto testFiles = MxFileRepository::getTestFiles( MX_COMPILE_MAX_FILE_SIZE_BYTES );
        for( auto it = testFiles.cbegin(); it != testFiles.cend(); ++it )
        {
            std::cout << "." << std::flush;
            // std::cout << "creating expected file - " << it->subdirectory << " - " << it->fileName << std::endl;
            auto fut = std::async( std::launch::async, generateExpectedFile, it->subdirectory.c_str(), it->fileName.c_str() );
            
            while( q.size() >= maxConcurrency )
            {
                for( auto qIter = q.begin(); qIter != q.end(); ++qIter )
                {
                    if( qIter->wait_for( std::chrono::milliseconds(0) ) == std::future_status::ready )
                    {
                        qIter->wait();
                        q.erase( qIter );
                        break;
                    }
                }
                std::this_thread::sleep_for( std::chrono::milliseconds(10) );
            }
            q.push_front( std::move( fut ) );
        }
        
        while( q.size() > 0 )
        {
            for( auto qIter = q.begin(); qIter != q.end(); ++qIter )
            {
                if( qIter->wait_for( std::chrono::milliseconds(0) ) == std::future_status::ready )
                {
                    qIter->wait();
                    q.erase( qIter );
                    break;
                }
                std::this_thread::sleep_for( std::chrono::milliseconds(10) );
            }
        }
        std::cout << "done" << std::endl;
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
        auto testFiles = MxFileRepository::getTestFiles( 0 );
        for( auto it = testFiles.cbegin(); it != testFiles.cend(); ++it )
        {
            std::string ex = getTestOutputExpectedFileFullPath( it->subdirectory, it->fileName );
            std::string er = getTestOutputErrorFileFullPath( it->subdirectory, it->fileName );
            deleteFileNoThrow( ex );
            deleteFileNoThrow( er );
        }
    }
    
}

#endif
