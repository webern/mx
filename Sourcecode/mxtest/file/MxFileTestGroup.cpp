// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/file/MxFileTestGroup.h"
#include "mxtest/file/MxFileRepository.h"

namespace mxtest
{
    MxFileTestGroup::MxFileTestGroup(
	    int maxFileSizeBytes,
	    std::string testName,
	    std::string testCppFileName,
	    int testCppFileLineNumber )
    : myTests{}
    {
        auto files = MxFileRepository::getTestFiles( maxFileSizeBytes );
        for( auto& file : files )
        {
            std::shared_ptr<MxFileTest> testPtr{ new ImportTestCpul{ file, testName, testCppFileName, testCppFileLineNumber } };
            myTests.push_back( testPtr );
        }
    }
    
    
    MxFileTestGroup::MxFileTestGroup(
			int maxFileSizeBytes,
			std::vector<std::string> fileList,
			MxFileTestListType listType,
			std::string testName,
			std::string testCppFileName,
			int testCppFileLineNumber )
    : myTests{}
    {
        throw std::runtime_error{ "not implemented" };
    }
}
