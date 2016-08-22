// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "HelloWorldTest.h"
#include "HelloWorldExpected.h"
#include "cpul/cpulTestHarness.h"

using namespace mx::core;

TEST( Test, HelloWorld )
{
    std::string expected = helloWorldExpected();
    std::string actual = MxTest::helloMusicXml();
    CHECK_EQUAL( expected, actual );
}

#endif
