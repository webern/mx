// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPORT_TESTS

#include "mxtest/file/Path.h"
#include "mxtest/file/StupidFileFunctions.h"
#include "mxtest/file/MxFileRepository.h"
#include "mx/utility/Parse.h"
#include "ezxml/XFactory.h"
#include "ezxml/XDoc.h"
#include "ezxml/XAttributeIterator.h"

#include <map>
#include <cstdio>

namespace mxtest
{
    
    void generateExpectedFile( const std::string& subdir, const std::string& fileName );
    
    ::ezxml::XDocPtr readInputFile( const std::string& subdir, const std::string& fileName );
    
    void setXmlDeclaration( ::ezxml::XDocPtr& xdoc );
    
    bool getIsScorePartwise( const ::ezxml::XDocPtr& xdoc );
    
    void setDoctype( ::ezxml::XDocPtr& xdoc, bool isScorePartwise );
    
    void setRootMusicXmlVersion( ::ezxml::XDocPtr& xdoc, bool isScorePartwsise );
    
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
