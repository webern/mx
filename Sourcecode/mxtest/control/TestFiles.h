// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>

namespace mxtest
{
    // set to zero for infitinity
    constexpr const int MX_COMPILE_MAX_FILE_SIZE_MEGABYTES = 5;
    constexpr const int MX_COMPILE_MAX_FILE_SIZE_BYTES = 1024 * MX_COMPILE_MAX_FILE_SIZE_MEGABYTES;
    
    using StringPair = std::pair<std::string, std::string>;
    using StringMap = std::map<std::string, std::string>;

    struct TestFile
    {
        std::string fileName;
        std::string subdirectory;
        std::string path;
        bool isLoadFailureExpected;
        int sizeBytes;
        inline std::string getFileNamePart() const
        {
            return fileName.substr( 0, static_cast<size_t>( static_cast<int>( fileName.size() ) - 4 ) );
        }
    };
    
    class TestFiles
    {
    public:
        TestFiles();
        
        const StringMap& getTestFileMap() const;
        const std::string getFullPath( const std::string& fileName ) const;
        const std::string& getSubdirectory( const std::string& fileName ) const;
        std::vector<TestFile> getTestFiles( int maxFileSizeBytes = 0 ) const;
        
    private:
        const std::string myPath;
        const std::string mySeparator;
        const StringMap myTestFiles;
        const std::set<std::string> myExpectedLoadFailures;
        
    };
}
