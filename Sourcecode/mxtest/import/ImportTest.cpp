// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPORT_TESTS

#include "mxtest/control/Path.h"
#include "cpul/cpulTestHarness.h"
#include "ExpectedFiles.h"
#include "mxtest/import/ImportTestImpl.h"

#include <cstdio>
#include <fstream>
#include <list>

using namespace mx::xml;
using namespace mxtest;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MXTEST_DELETE_EXPECTED_FILES
#ifdef MXTEST_DELETE_EXPECTED_FILES

    TEST( DeleteExpected, Import )
    {
        mxtest::deleteExpectedFiles();
    }
    T_END

#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MXTEST_GENERATE_EXPECTED_FILES
#ifdef MXTEST_GENERATE_EXPECTED_FILES

    TEST( GenerateExpected, Import )
    {
        mxtest::generateExpectedFiles();
    }
    T_END

#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MXTEST_DELETE_TEST_OUTPUT_FILES
#ifdef MXTEST_DELETE_TEST_OUTPUT_FILES

    TEST( DeleteTestOutputFiles, Import )
    {
        mxtest::deleteTestOutputFiles();
    }
    T_END

#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MXTEST_CREATE_CSV_FILE
#ifdef MXTEST_CREATE_CSV_FILE

    TEST( CreateCsvFile, Import )
    {
        std::string csvFilePath = mxtest::getTestRecordFilepath();
        deleteFileNoThrow( csvFilePath );
        std::ofstream ofile;
        openAppendFile( ofile, csvFilePath );
        ofile << mxtest::ImportRoundTripResult::getHeaderRow() << std::endl;
        ofile.close();
    }
    T_END

#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////

class ImportTests
{
public:
    ImportTests()
    {
        int maxFileSizeBytes = MX_COMPILE_MAX_FILE_SIZE_BYTES; // 0 means infinity
        TestFiles t;
        auto files = t.getTestFiles( maxFileSizeBytes );
        for( const auto& file : files )
        {
            tests.push_back( std::make_shared<ImportTestCpul>( file ) );
        }
    }
    std::list<std::shared_ptr<ImportTestCpul>> tests;
} importTestsInstance;

#endif
