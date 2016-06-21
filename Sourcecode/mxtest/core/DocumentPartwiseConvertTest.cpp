#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/DocumentTimewise.h"
#include "mx/core/DocumentPartwise.h"
#include "mxtest/core/DocumentHeaderTest.h"
#include "DocumentPartwiseCreate.h"
#include "DocumentTimewiseCreate.h"

using namespace mx::core;
using namespace std;
using namespace MxTestHelpers;

TEST( DocumentPartwiseConvert, DocumentPartwise )
{
    auto docOriginal = MxTestHelpers::createDocumentTimewise();
    auto docExpected = MxTestHelpers::createDocumentPartwise();
    auto docActual = DocumentPartwise::convert( docOriginal );
    stringstream expected;
    docExpected->toStream( expected );
    stringstream actual;
    docActual->toStream( actual );
    CHECK_EQUAL( expected.str(), actual.str() )
}

#endif
