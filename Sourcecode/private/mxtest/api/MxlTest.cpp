// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mxtest/file/MxFileRepository.h"
#include <sstream>

namespace mxtest
{
    static constexpr const char* const EXPECTED_MXL_ERROR_MESSAGE =
        "it looks like you are trying to parse a compressed musicxml file, which is currently unsupported.";
}


TEST( Mxl, TemporaryNoCrashTest )
{
    const auto filepath = mxtest::MxFileRepository::getFullPath( "Dichterliebe01.mxl" );
    auto& docMgr = mx::api::DocumentManager::getInstance();
    std::string actualMessage;
    const std::string expectedMessage = mxtest::EXPECTED_MXL_ERROR_MESSAGE;
    bool isExceptionThrown = false;

    try
    {
        docMgr.createFromFile( filepath );
    }
    catch ( std::exception& e )
    {
        isExceptionThrown = true;
        actualMessage = e.what();
    }

    if( !isExceptionThrown )
    {
        FAIL( "an exception was expected during docMgr.createFromFile( filepath ); but none was thrown" );
    }

    const auto isFound = actualMessage.find( expectedMessage ) != std::string::npos;

    if( !isFound )
    {
        std::stringstream failure;
        failure << "expected to find the string '" << expectedMessage << "' in the error message '"
        << actualMessage << "' but did not.";
        FAIL( failure.str() );
    }
}
