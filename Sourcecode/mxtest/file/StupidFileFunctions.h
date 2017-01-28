// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <string>

namespace mxtest
{
    constexpr const char* const TEST_OUTPUT_DIRECTORY_NAME = "testOutput";
    constexpr const char* const EXPECTED_FILES_DIRECTORY_NAME = "expected";
    
    std::string getSubdirectoryFullPath( const std::string& subdirectory );
    std::string getTestOutputFullDirectoryPath();
    std::string getExpectedFilesFullDirectoryPath();
    std::string getExpectedFileFullPath( const std::string& filename );
    
    std::string concatFileNameParts(
        const std::string& fileNamePart,
        const std::string& fileExtensionPart );
    
    std::string filePath(
        const std::string& subdirectory,
        const std::string& fileName );
    
    void openInputFile(
        std::ifstream& outFileRef,
        const std::string& inFullpath );
    
    void openOutputFile(
        std::ofstream& outFileRef,
        const std::string& inFullpath );
    
    void openAppendFile(
        std::ofstream& outFileRef,
        const std::string& inFullpath );
    
    void deleteFileThrowError( const std::string fullpath );
    
    bool deleteFileNoThrow( const std::string fullpath );
}
