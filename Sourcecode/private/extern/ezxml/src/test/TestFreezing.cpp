#include <sstream>

#include "Catch.h"
#include "ezxml/ezxml.h"
#include "Files.h"
#include "equal.h"

TEST_CASE( "FreezingTest" )
{
    using namespace ezxml;
    using namespace ezxmltest;
    const auto fp = filepath( FREEZING_SMALL );
    auto xdocOriginal = XFactory::makeXDoc();
    xdocOriginal->loadFile( fp );
    auto xdoc = std::move( xdocOriginal );

    const auto& root = xdoc->getRoot();
    auto str = root->getName();
    CHECK( "score-partwise" == str );
    auto aiter = root->attributesBegin();
    auto aend = root->attributesEnd();
    REQUIRE( aiter != aend );
    str = aiter->getName();
    CHECK( "version" == str );
    str = aiter->getValue();
    CHECK( "3.0" == str );
    ++aiter;
    CHECK( aend == aiter );

    auto iter = root->begin();

    // drill down to some random place and check a few values
    ++iter;
    ++iter;
    ++iter;
    ++iter;
    ++iter;
    ++iter;
    ++iter;
    ++iter;
    ++iter;
    ++iter;
    ++iter;
    ++iter;
    ++iter;
    iter = iter->begin();
    iter = iter->begin();
    const auto name = iter->getName();
    CHECK( "print" == name );
    auto printAttributesIter = iter->attributesBegin();
    const auto printAttributesEnd = iter->attributesEnd();
    REQUIRE( printAttributesIter != printAttributesEnd );
    CHECK( "blank-page" == printAttributesIter->getName() );
    CHECK( "4" == printAttributesIter->getValue() );
    ++printAttributesIter;
    REQUIRE( printAttributesIter != printAttributesEnd );
    CHECK( "new-page" == printAttributesIter->getName() );
    CHECK( "yes" == printAttributesIter->getValue() );
    ++printAttributesIter;
    CHECK( printAttributesIter == printAttributesEnd );
    REQUIRE( iter->begin() != iter->end() );
    CHECK( "system-layout" == iter->begin()->getName() );

    // round-trip the file and make sure it didn't change
    std::stringstream saveStream;
    xdoc->saveStream( saveStream );
    auto xmlStr = saveStream.str();
    std::istringstream iss{ xmlStr };
    auto reified = XFactory::makeXDoc();
    reified->loadStream( iss );
    CHECK( equal( xdoc, reified ).empty() );

    // prove that our equality function would actually catch an inequality in the file
    size_t start_pos = 0;
    const std::string replaceable = "Freezing";
    const std::string replaceWith = "Breezing";
    while( ( start_pos = xmlStr.find( replaceable, start_pos ) ) != std::string::npos )
    {
        xmlStr.replace( start_pos, replaceable.length(), replaceWith );
        start_pos += replaceWith.length();
    }
    auto reifiedWrong = XFactory::makeXDoc();
    std::istringstream issWrong{ xmlStr };
    reifiedWrong->loadStream( issWrong );
    CHECK( "score-partwise/movement-title: element values not equal: Freezing Pieces vs Breezing Pieces" ==
           equal( xdoc, reifiedWrong ) );
}