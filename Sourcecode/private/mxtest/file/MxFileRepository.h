// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mxtest/control/CompileControl.h"
#include "mxtest/file/MxFile.h"
#include "mx/api/ScoreData.h"

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <set>

namespace mxtest
{
    using StringPair = std::pair<std::string, std::string>;
    using StringMap = std::map<std::string, std::string>;
    
    class MxFileRepository
    {
    public:
        static const std::string getFullPath( const std::string& fileName );
        static const std::string getNameWithoutExtension( const std::string& fileName );
        static const std::string& getSubdirectory( const std::string& fileName );
        static MxFiles getTestFiles( int maxFileSizeBytes );
        static const MxFiles& getTestFiles();
        static const MxFile& getTestFile( const std::string& fileName );
        static mx::api::ScoreData loadFile( const std::string& fileName );
        
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
