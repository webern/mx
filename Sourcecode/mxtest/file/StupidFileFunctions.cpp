// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/file/StupidFileFunctions.h"
#include "mxtest/file/Path.h"
#include "mx/utility/Throw.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

namespace mxtest
{
    
    std::string getSubdirectoryFullPath( const std::string& subdirectory )
    {
        std::stringstream ss;
        ss << RESOURCES_DIRECTORY_PATH << subdirectory << FILE_PATH_SEPARATOR;
        return ss.str();
    }
    
    
	std::string getTestOutputFullDirectoryPath()
    {
        return getSubdirectoryFullPath( TEST_OUTPUT_DIRECTORY_NAME );
    }
    
    
    std::string getExpectedFilesFullDirectoryPath()
    {
        return getSubdirectoryFullPath( EXPECTED_FILES_DIRECTORY_NAME );
    }
    
    
    std::string concatFileNameParts(
        const std::string& fileNamePart,
        const std::string& fileExtensionPart )
    {
        std::stringstream ss;
        ss << fileNamePart << "." << fileExtensionPart;
        return ss.str();
    }
    
    
    std::string getExpectedFileFullPath( const std::string& filename )
    {
        std::stringstream ss;
        ss << getExpectedFilesFullDirectoryPath();
        ss << filename;
        return ss.str();
    }
    
    
    std::string filePath(
        const std::string& subdirectory,
        const std::string& fileName )
    {
        std::stringstream ss;
        ss << getSubdirectoryFullPath( subdirectory );
        ss << fileName;
        return ss.str();
    }
    
    
    void openInputFile(
        std::ifstream& outFileRef,
        const std::string& inFullpath )
    {
        outFileRef = std::ifstream( inFullpath );
        if( !outFileRef.is_open() )
        {
            std::stringstream ss;
            ss << "the file '" << inFullpath << "' could not be opened for reading";
            MX_THROW( ss.str() );
        }
    }
    
    
    void openOutputFile(
        std::ofstream& outFileRef,
        const std::string& inFullpath )
    {
        outFileRef = std::ofstream( inFullpath );
        if( !outFileRef.is_open() )
        {
            std::stringstream ss;
            ss << "the file '" << inFullpath << "' could not be opened for writing";
            MX_THROW( ss.str() );
        }
    }
    
    
    void openAppendFile(
        std::ofstream& outFileRef,
        const std::string& inFullpath )
    {
        outFileRef = std::ofstream( inFullpath, std::ios::app );
        if( !outFileRef.is_open() )
        {
            std::stringstream ss;
            ss << "the file '" << inFullpath << "' could not be opened for appending";
            MX_THROW( ss.str() );
        }
    }
    
    
    void deleteFileThrowError( const std::string fullpath )
    {
        auto result = std::remove( fullpath.c_str() );
        if( result != 0 )
        {
            std::stringstream ss;
            ss << "deletion of the file '" << fullpath << "' failed";
            std::cout << ss.str() << std::endl;
        }
    }

    
    bool deleteFileNoThrow( const std::string fullpath )
    {
        return 0 == std::remove( fullpath.c_str() );
    }
    
}
