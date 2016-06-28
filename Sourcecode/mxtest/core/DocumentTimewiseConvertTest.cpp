#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/DocumentPartwise.h"
#include "mx/core/DocumentTimewise.h"
#include "mxtest/core/DocumentHeaderTest.h"
#include "DocumentTimewiseCreate.h"
#include "DocumentPartwiseCreate.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( DocumentTimewiseConvert, DocumentTimewise )
{
    auto docOriginal = MxTestHelpers::createDocumentPartwise();
    auto docExpected = MxTestHelpers::createDocumentTimewise();
    auto docActual = DocumentTimewise::convert( docOriginal );
    stringstream expected;
    docExpected->toStream( expected );
    stringstream actual;
    docActual->toStream( actual );
    CHECK_EQUAL( expected.str(), actual.str() )
}

#endif