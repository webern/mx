// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPORT_TESTS

#include "mxtest/control/Path.h"
#include "mxtest/control/File.h"
#include "mxtest/import/TestFiles.h"
#include "mx/utility/Parse.h"
#include "mx/xml/XFactory.h"
#include "mx/xml/XDoc.h"
#include "mx/xml/XAttributeIterator.h"

#include <map>
#include <cstdio>

namespace MxTest
{
    
    void generateExpectedFile( const std::string& subdir, const std::string& fileName );
    
    mx::xml::XDocPtr readInputFile( const std::string& subdir, const std::string& fileName );
    
    void setXmlDeclaration( mx::xml::XDocPtr& xdoc );
    
    bool getIsScorePartwise( const mx::xml::XDocPtr& xdoc );
    
    void setDoctype( mx::xml::XDocPtr& xdoc, bool isScorePartwise );
    
    void setRootMusicXmlVersion( mx::xml::XDocPtr& xdoc, bool isScorePartwsise );
    
    std::string getExpectedFileName( const std::string& subdir, const std::string& fileName );
    
    std::string getExpectedFileFullPath( const std::string& subdir, const std::string& fileName );

    void deleteExpectedFiles();
    
    void generateExpectedFiles();
    
    std::string gettestOutputErrorFileName( const std::string& subdir, const std::string& fileName );
    
    std::string getTestOutputErrorFileFullPath( const std::string& subdir, const std::string& fileName );
    
    std::string getTestOutputExpectedFileName( const std::string& subdir, const std::string& fileName );

    std::string getTestOutputExpectedFileFullPath( const std::string& subdir, const std::string& fileName );

    void deleteTestOutputFiles();
}

#endif
