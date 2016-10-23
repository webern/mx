// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mxtest/file/MxFile.h"

#include <string>
#include <vector>
#include <map>
#include <set>

namespace mxtest
{
    // set to zero for infitinity
    constexpr const int MX_COMPILE_MAX_FILE_SIZE_KILOBYTES = 10;
    constexpr const int MX_COMPILE_MAX_FILE_SIZE_BYTES = 1024 * MX_COMPILE_MAX_FILE_SIZE_KILOBYTES;
    
    using StringPair = std::pair<std::string, std::string>;
    using StringMap = std::map<std::string, std::string>;
    
    class MxFileRepository
    {
    public:
        static const std::string getFullPath( const std::string& fileName );
        static const std::string& getSubdirectory( const std::string& fileName );
        static MxFiles getTestFiles( int maxFileSizeBytes );
        static const MxFiles& getTestFiles();
        static const MxFile& getTestFile( const std::string& fileName );
        
    private:
        MxFileRepository();
        static const MxFileRepository& getInstance();
        void initializeExpectedLoadFailures();
        void initializeTestFiles();
        void initializeNameSubdirectoryMap();

    private:
        const std::string myPath;
        const std::string mySeparator;
        std::set<std::string> myExpectedLoadFailures;
        StringMap myNameSubdirectoryMap;
        MxFiles myTestFiles;
    };
}
