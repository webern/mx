// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "cpul/cpulTestHarness.h"
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "mx/api/PageData.h"

using namespace std;
using namespace mx::api;

// Make sure the equality macros (i.e. operator==) works correctly with std::optional
TEST( EqualityOfOptional, PitchData )
{
    PageData a;
    PageData b;
    a.pageLayoutData = PageLayoutData{};
    CHECK( a.pageLayoutData );
    CHECK( !b.pageLayoutData );
    CHECK( a != b );
    PageData c;
    c.pageLayoutData = PageLayoutData{};
    c.pageLayoutData->pageWidth = 1;
    PageData d;
    d.pageLayoutData = PageLayoutData{};
    d.pageLayoutData->pageWidth = 2;
    CHECK( c != d );
    d.pageLayoutData->pageWidth = 1;
    CHECK( c == d );
}
T_END;

#endif
