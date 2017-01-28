// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "HelloWorldTest.h"
#include "HelloWorldExpected.h"
#include "cpul/cpulTestHarness.h"

using namespace mx::core;

TEST( Test, HelloWorld )
{
    std::string expected = helloWorldExpected();
    std::string actual = mxtest::helloMusicXml();
    CHECK_EQUAL( expected, actual );
}

#endif
