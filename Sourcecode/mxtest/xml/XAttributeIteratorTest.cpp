// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_XML_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/xml/FakeXml.h"
#include "mx/xml/XFactory.h"
#include "mx/xml/XDoc.h"
#include "mx/xml/XAttribute.h"
#include "mx/xml/XAttributeIterator.h"
#include "mx/xml/XElementIterator.h"

#include <algorithm>

using namespace std;
using namespace mx::xml;

namespace
{
    inline mx::xml::XDocCPtr doc()
    {
        auto xdoc = XFactory::makeXDoc();
        std::istringstream is( MxTest::fakeXml );
        xdoc->loadStream( is );
        return xdoc;
    }
    
    inline XElementPtr somethingWithAttributes( const mx::xml::XDocCPtr& xdoc )
    {
        auto root = xdoc->getRoot();
        auto firstIter = root->begin();
        ++firstIter;
        ++firstIter;
        auto nextIter = firstIter->begin();
        return nextIter->clone();
    }
}


TEST( basicIteration, XAttributeIterator )
{
    auto xdoc = doc();
    auto node = somethingWithAttributes( xdoc );
    auto iter = node->attributesBegin();
    CHECK( iter != node->attributesEnd() );
    CHECK_EQUAL( "attr-a", iter->getName() );
    CHECK_EQUAL( "A", iter->getValue() );
    ++iter;
    CHECK( iter != node->attributesEnd() );
    CHECK_EQUAL( "attr-b", iter->getName() );
    CHECK_EQUAL( "B", iter->getValue() );
    ++iter;
    CHECK( iter != node->attributesEnd() );
    CHECK_EQUAL( "attr-c", iter->getName() );
    CHECK_EQUAL( "C", iter->getValue() );
    ++iter;
    CHECK( iter == node->attributesEnd() );
    
}
T_END


TEST( suffixIncrement, XAttributeIterator )
{
    auto xdoc = doc();
    auto node = somethingWithAttributes( xdoc );
    auto iter = node->attributesBegin();
    CHECK( iter != node->attributesEnd() );
    CHECK_EQUAL( "attr-a", ( iter++ )->getName() );
    CHECK( iter != node->attributesEnd() );
    CHECK_EQUAL( "attr-b", ( iter++ )->getName() );
    CHECK( iter != node->attributesEnd() );
    CHECK_EQUAL( "attr-c", ( iter++ )->getName() );
    CHECK( iter == node->attributesEnd() );
}
T_END


TEST( suffixDecrement, XAttributeIterator )
{
    auto xdoc = doc();
    auto node = somethingWithAttributes( xdoc );
    XAttributeIterator iter = node->attributesEnd();
    CHECK( iter == node->attributesEnd() );
    iter--;
    CHECK( iter != node->attributesEnd() );
    CHECK( iter != node->attributesBegin() );
    CHECK_EQUAL( "attr-c", ( iter-- )->getName() );
    CHECK( iter != node->attributesEnd() );
    CHECK( iter != node->attributesBegin() );
    CHECK_EQUAL( "attr-b", ( iter-- )->getName() );
    CHECK( iter != node->attributesEnd() );
    CHECK( iter == node->attributesBegin() );
    CHECK_EQUAL( "attr-a", ( iter-- )->getName() );
    
    // iter is now "before" root->begin()
    // which behaves like an "end" iter, should throw
    CHECK_RAISES( iter->getName() );
}
T_END


TEST( prefixIncrement, XAttributeIterator )
{
    auto xdoc = doc();
    auto node = somethingWithAttributes( xdoc );
    XAttributeIterator iter = node->attributesBegin();
    CHECK_EQUAL( "attr-a", iter->getName() );
    CHECK_EQUAL( "attr-b", ( ++iter )->getName() );
    CHECK_EQUAL( "attr-c", ( ++iter )->getName() );
    ++iter;
    CHECK( iter == node->attributesEnd() )
}
T_END


TEST( prefixDecrement, XAttributeIterator )
{
    auto xdoc = doc();
    auto node = somethingWithAttributes( xdoc );
    XAttributeIterator iter = node->attributesEnd();
    CHECK( iter == node->attributesEnd() );
    CHECK( iter != node->attributesBegin() );
    CHECK_EQUAL( "attr-c", ( --iter )->getName() );
    CHECK( iter != node->attributesEnd() );
    CHECK( iter != node->attributesBegin() );
    CHECK_EQUAL( "attr-b", ( --iter )->getName() );
    CHECK( iter != node->attributesEnd() );
    CHECK( iter != node->attributesBegin() );
    CHECK_EQUAL( "attr-a", ( --iter )->getName() );
    CHECK( iter != node->attributesEnd() );
    CHECK( iter == node->attributesBegin() );
    --iter;
    
    // iter is now "before" root->begin()
    // which behaves like an "end" iter, should throw
    CHECK_RAISES( iter->getName() );
}
T_END


TEST( stlAlgorithm, XAttributeIterator )
{
    // First demonstrate the impossible lambda syntax
    std::vector<int> ints{ 5, 7, 99, 1, -1, 36, 17, 9 };
    
    auto it = std::find_if(
                           ints.begin(),
                           ints.end(),
                           [] ( const int& i ) { return i < 0; } );
    
    CHECK_EQUAL( -1, *it );
    
    // Now try it with the custom iterator
    auto xdoc = doc();
    auto node = somethingWithAttributes( xdoc );
    
    auto iter = std::find_if(
                             node->attributesBegin(),
                             node->attributesEnd(),
                             [] ( const XAttribute& n ) { return n.getName() == "attr-b"; } );
    
    CHECK_EQUAL( "attr-b", iter->getName() )
}
T_END


TEST( operatorStar, XAttributeIterator )
{
    auto xdoc = doc();
    auto node = somethingWithAttributes( xdoc );
    XAttributeIterator iter = node->attributesBegin();
    auto& attribute = *iter;
    CHECK_EQUAL( "attr-a", attribute.getName() );
    attribute.setName( "attr-XYZ" );
    CHECK_EQUAL( "attr-XYZ", attribute.getName() );
    
    // obtain the node again to make sure it changed in the xdoc
    auto reobtainednode = somethingWithAttributes( xdoc );
    auto reobtainedIter = reobtainednode->attributesBegin();
    CHECK_EQUAL( "attr-XYZ", reobtainedIter->getName() );
}
T_END


TEST( copyConstructor, XAttributeIterator )
{
    auto xdoc = doc();
    auto node = somethingWithAttributes( xdoc );
    XAttributeIterator iter = node->attributesBegin();
    XAttributeIterator copy{ iter };
    
    CHECK_EQUAL( "attr-a", iter->getName() )
    CHECK_EQUAL( "attr-a", copy->getName() )
    
    copy->setName( "XYZ" );
    
    CHECK_EQUAL( "XYZ", iter->getName() )
    CHECK_EQUAL( "XYZ", copy->getName() )
}
T_END


TEST( moveConstructor, XAttributeIterator )
{
    auto xdoc = doc();
    auto node = somethingWithAttributes( xdoc );
    XAttributeIterator movedIter{ node->attributesBegin() }; // maybe it calls move ctor...

    CHECK_EQUAL( "attr-a", movedIter->getName() )
    movedIter->setName( "XYZ" );
    CHECK_EQUAL( "XYZ", movedIter->getName() )

    // obtain the node again to make sure it changed in the xdoc
    auto reobtainednode = somethingWithAttributes( xdoc );
    auto reobtainedIter = reobtainednode->attributesBegin();
    CHECK_EQUAL( "XYZ", reobtainedIter->getName() );
}
T_END


TEST( assignmentOperator, XAttributeIterator )
{
    auto xdoc = doc();
    auto node = somethingWithAttributes( xdoc );
    XAttributeIterator iter = node->attributesBegin();
    XAttributeIterator assigned;
    assigned = iter;
    
    CHECK_EQUAL( "attr-a", iter->getName() )
    CHECK_EQUAL( "attr-a", assigned->getName() )
    
    assigned->setName( "XYZ" );
    
    CHECK_EQUAL( "XYZ", iter->getName() )
    CHECK_EQUAL( "XYZ", assigned->getName() )
}
T_END


TEST( moveAssignment, XAttributeIterator )
{
    auto xdoc = doc();
    auto node = somethingWithAttributes( xdoc );
    XAttributeIterator movedIter;
    movedIter = node->attributesBegin(); // maybe it calls move assignment...

    CHECK_EQUAL( "attr-a", movedIter->getName() )
        movedIter->setName( "XYZ" );
    CHECK_EQUAL( "XYZ", movedIter->getName() )

    // obtain the node again to make sure it changed in the xdoc
    auto reobtainednode = somethingWithAttributes( xdoc );
    auto reobtainedIter = reobtainednode->attributesBegin();
    CHECK_EQUAL( "XYZ", reobtainedIter->getName() );
}
T_END


TEST( nullThrows, XAttributeIterator )
{
    XAttributeIterator nullIter;
    CHECK_RAISES( nullIter->getName() )
    CHECK_RAISES( (*nullIter).getName() )
    
}
T_END


TEST( doesNotCrash, XAttributeIterator )
{
    XAttributeIterator nullIter;
    XAttributeIterator anotherIter;
    CHECK_NOT_RAISES( anotherIter = nullIter );
    CHECK_NOT_RAISES( anotherIter = XAttributeIterator{} );
}
T_END


namespace
{
    XElementPtr nodeWithDocOutOfScope()
    {
        auto xdoc = doc();
        auto node = xdoc->getRoot();
        return node;
    }
}


TEST( avoidsUndefinedBehavior, XAttributeIterator )
{
    auto root = nodeWithDocOutOfScope();
    CHECK_RAISES( auto iter = root->attributesBegin() );
}
T_END

#endif
