// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPORT_TESTS

#include "mxtest/file/MxFileTestGroup.h"
#include "mxtest/file/Path.h"
#include "cpul/cpulTestHarness.h"
#include "ExpectedFiles.h"
#include "mxtest/import/ImportTestImpl.h"

#include <cstdio>
#include <fstream>
#include <list>

using namespace ezxml;
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

MxFileTestGroup<ImportTestCpul> instance
{
    MX_COMPILE_MAX_FILE_SIZE_BYTES, // maxFileSizeBytes
    "XML Import Tests",
    __FILE__,
    __LINE__
};

#endif
