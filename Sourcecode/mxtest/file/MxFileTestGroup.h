// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mxtest/file/MxFileTest.h"
#include "mxtest/import/ImportTestImpl.h"

#include <vector>
#include <list>
#include <memory>

namespace mxtest
{

	enum class MxFileTestListType
	{
		includedFiles,
		excludedFiles
	};

	// the purpose of this class is to create a collection of MxFileTest instances
	// and to keep them alive for the lifetime of the test run.  pointers to these
	// tests will be inserted into the cpul test registry and run during main()
	template<typename MX_FILE_TEST_DERIVITAVE_TYPE>
	class MxFileTestGroup
	{
	public:

		// will automatically include all files from TestFiles.h which are less
		// than or equal to maxFileSizeBytes in size.  maxFileSizeBytes 0 is infinity
		MxFileTestGroup(
			int maxFileSizeBytes,
			std::string testName,
			std::string testCppFileName,
			int testCppFileLineNumber )
	    {
	        auto files = MxFileRepository::getTestFiles( maxFileSizeBytes );
	        for( auto& file : files )
	        {
	            std::shared_ptr<MxFileTest> testPtr{ new MX_FILE_TEST_DERIVITAVE_TYPE{ file, testName, testCppFileName, testCppFileLineNumber } };
	            myTests.push_back( testPtr );
	        }
	    }

		// if you use MxFileTestListType::includedFiles then only those files which
		// are in fileList and which are less than or equal to maxFileSizeBytes
		// will be included.  only those files which are found in TestFiles.h may be
		// added to the fileList. if you use MxFileTestListType::excludedFiles then
		// all files which are found in TestFiles.h and which are less than or equal
		// to maxFileSizeBytes and which are *not* found in fileList will be included.
		// maxFileSizeBytes == 0 means infinity
		MxFileTestGroup(
			int maxFileSizeBytes,
			std::vector<std::string> fileList,
			MxFileTestListType listType,
			std::string testName,
			std::string testCppFileName,
			int testCppFileLineNumber )
		{
        	throw std::runtime_error{ "not implemented" };
    	}

	private:
		std::list<std::shared_ptr<MxFileTest>> myTests;
	};
}
