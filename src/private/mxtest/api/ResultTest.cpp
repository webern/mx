// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/Result.h"

using namespace mx::api;

// A fresh Location knows nothing: every position is -1. Errors built without
// a position in mind read as "unknown", never as a plausible wrong place.
TEST(locationDefaultsToUnknown, Result)
{
    const Location location{};
    CHECK_EQUAL(-1, location.partIndex);
    CHECK_EQUAL(-1, location.measureIndex);
    CHECK_EQUAL(-1, location.staffIndex);
    CHECK_EQUAL(-1, location.voiceIndex);
    CHECK_EQUAL(-1, location.tickTimePosition);
    CHECK_EQUAL(-1, location.byteOffset);
    CHECK(location.xmlPath.empty());
}

T_END

// formatError is the one way errors are written to logs, so its output is
// pinned: an error rendered today renders the same next year. Unknown
// positions are left out rather than printed as part=-1.
TEST(formatErrorRendersCodeMessageAndPosition, Result)
{
    ApiError error{};
    error.code = ResultCode::tooManyElements;
    error.location.partIndex = 0;
    error.location.measureIndex = 2;
    error.location.tickTimePosition = 480;
    error.message = "at most 8 beam occurrences";
    CHECK_EQUAL(std::string{"mx: tooManyElements at part=0 measure=2 tick=480: "
                            "at most 8 beam occurrences"},
                formatError(error));
}

T_END

TEST(formatErrorRendersXmlPath, Result)
{
    ApiError error{};
    error.code = ResultCode::unknownElement;
    error.location.xmlPath = "/score-partwise/part[1]/measure[3]";
    error.message = "nope";
    CHECK_EQUAL(std::string{"mx: unknownElement at /score-partwise/part[1]/measure[3]: nope"}, formatError(error));
}

T_END

TEST(formatErrorRendersByteOffset, Result)
{
    ApiError error{};
    error.code = ResultCode::xmlSyntaxError;
    error.location.byteOffset = 12;
    error.message = "Error parsing document declaration";
    CHECK_EQUAL(std::string{"mx: xmlSyntaxError at offset=12: Error parsing document declaration"}, formatError(error));
}

T_END

// Nothing is known and nothing was said: no empty "at", no dangling colon.
TEST(formatErrorRendersBareCode, Result)
{
    ApiError error{};
    error.code = ResultCode::ioError;
    CHECK_EQUAL(std::string{"mx: ioError"}, formatError(error));
}

T_END

#endif
