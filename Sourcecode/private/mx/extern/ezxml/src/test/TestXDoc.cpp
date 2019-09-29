#include <sstream>

#include "Catch.h"
#include "ezxml/ezxml.h"

static constexpr const char* const xml = R"(
<whatever abc="def" ghi="jkl">
    <bloop>blerp</bloop>
    <bloop>bleep</bloop>
</whatever>
)";

TEST_CASE( "basic_xml", "1" )
{
    std::istringstream iss{ xml };
    auto xdoc = ezxml::XFactory::makeXDoc();
    xdoc->loadStream( iss );
    auto root = xdoc->getRoot();
    CHECK( "whatever" == root->getName() );
    auto aiter = root->attributesBegin();
    const auto aend = root->attributesEnd();
    REQUIRE( aiter != aend );
    CHECK( "abc" == aiter->getName() );
    CHECK( "def" == aiter->getValue() );
    ++aiter;
    REQUIRE( aiter != aend );
    CHECK( "ghi" == aiter->getName() );
    CHECK( "jkl" == aiter->getValue() );
    ++aiter;
    REQUIRE( aiter == aend );

    auto chiter = root->begin();
    const auto chend = root->end();
    REQUIRE( chiter != chend );
    REQUIRE( ezxml::XElementType::text == chiter->getType() );
    CHECK( "bloop" == chiter->getName() );
    CHECK( "blerp" == chiter->getValue() );

    ++chiter;
    REQUIRE( chiter != chend );
    REQUIRE( ezxml::XElementType::text == chiter->getType() );
    CHECK( "bloop" == chiter->getName() );
    CHECK( "bleep" == chiter->getValue() );

    ++chiter;
    REQUIRE( chiter == chend );
}