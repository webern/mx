#include "cpul/cpulTestHarness.h"
#include "mx/xml/XFactory.h"
#include "mx/xml/XDoc.h"
#include "mx/xml/XElement.h"
#include "mx/xml/XElementIterator.h"
#include "mxtest/xml/FakeXml.h"



#include <algorithm>

using namespace std;
using namespace mx::xml;

namespace
{
    inline mx::xml::XDocCPtr doc()
    {
        auto xdoc = XFactory::makeXDoc();
        std::istringstream is( MxTest::fakeXml );
        xdoc->parse( is );
        return xdoc;
    }
}


TEST( beginEndForLoop, XElementIterator )
{
    auto xdoc = doc();
    auto root = xdoc->getRoot();
    int i = 0;
    for( auto it = root->elementsBegin(); it != root->elementsEnd(); ++it, ++i )
    {
        switch( i )
        {
            case 0:
                CHECK_EQUAL( "abc", it->getName() );
                break;
            case 1:
                CHECK_EQUAL( "empty-element", it->getName() );
                break;
            case 2:
                CHECK_EQUAL( "nested-stuff", it->getName() );
                break;
            default:
                CHECK_FAIL( "should not reach here" );
                break;
        }
    }
}
T_END


TEST( suffixIncrement, XElementIterator )
{
    auto xdoc = doc();
    auto root = xdoc->getRoot();
    XElementIterator iter = root->elementsBegin();
    CHECK( iter != root->elementsEnd() );
    CHECK_EQUAL( "abc", ( iter++ )->getName() );
    CHECK( iter != root->elementsEnd() );
    CHECK_EQUAL( "empty-element", ( iter++ )->getName() );
    CHECK( iter != root->elementsEnd() );
    CHECK_EQUAL( "nested-stuff", ( iter++ )->getName() );
    CHECK( iter == root->elementsEnd() );
}
T_END


TEST( suffixDecrement, XElementIterator )
{
    auto xdoc = doc();
    auto root = xdoc->getRoot();
    XElementIterator iter = root->elementsEnd();
    CHECK( iter == root->elementsEnd() );
    iter--;
    CHECK( iter != root->elementsEnd() );
    CHECK( iter != root->elementsBegin() );
    CHECK_EQUAL( "nested-stuff", ( iter-- )->getName() );
    CHECK( iter != root->elementsEnd() );
    CHECK( iter != root->elementsBegin() );
    CHECK_EQUAL( "empty-element", ( iter-- )->getName() );
    CHECK( iter != root->elementsEnd() );
    CHECK( iter == root->elementsBegin() );
    CHECK_EQUAL( "abc", ( iter-- )->getName() );
    
    // iter is now "before" root->elementsBegin()
    // which behaves like an "end" iter, should throw
    CHECK_RAISES( iter->getName() );
}
T_END


TEST( prefixIncrement, XElementIterator )
{
    auto xdoc = doc();
    auto root = xdoc->getRoot();
    XElementIterator iter = root->elementsBegin();
    CHECK_EQUAL( "abc", iter->getName() );
    CHECK_EQUAL( "empty-element", ( ++iter )->getName() );
    CHECK_EQUAL( "nested-stuff", ( ++iter )->getName() );
    ++iter;
    CHECK( iter == root->elementsEnd() )
}


TEST( prefixDecrement, XElementIterator )
{
    auto xdoc = doc();
    auto root = xdoc->getRoot();
    XElementIterator iter = root->elementsEnd();
    CHECK( iter == root->elementsEnd() );
    CHECK( iter != root->elementsBegin() );
    CHECK_EQUAL( "nested-stuff", ( --iter )->getName() );
    CHECK( iter != root->elementsEnd() );
    CHECK( iter != root->elementsBegin() );
    CHECK_EQUAL( "empty-element", ( --iter )->getName() );
    CHECK( iter != root->elementsEnd() );
    CHECK( iter != root->elementsBegin() );
    CHECK_EQUAL( "abc", ( --iter )->getName() );
    CHECK( iter != root->elementsEnd() );
    CHECK( iter == root->elementsBegin() );
    --iter;
    
    // iter is now "before" root->elementsBegin()
    // which behaves like an "end" iter, should throw
    CHECK_RAISES( iter->getName() );
}
T_END


TEST( stlAlgorithm, XElementIterator )
{
    // First demonstrate the impossible lambda syntax
    std::vector<int> ints{ 5, 7, 99, 1, -1, 36, 17, 9 };

    auto it = std::find_if(
        ints.begin(),
        ints.end(),
        [] ( const int& i ) { return i < 0; } );
    
    CHECK_EQUAL( -1, (*it) );

    // Now try it with the custom iterator
    auto xdoc = doc();
    auto root = xdoc->getRoot();
    
    auto iter = std::find_if(
        root->elementsBegin(),
        root->elementsEnd(),
        [] ( const XElement& n ) { return n.getName() == "nested-stuff"; } );
    
    CHECK_EQUAL( "nested-stuff", iter->getName() )
}
T_END


TEST( operatorStar, XElementIterator )
{
    auto xdoc = doc();
    auto root = xdoc->getRoot();
    XElementIterator iter = root->elementsBegin();
    auto& node = *iter;
    CHECK_EQUAL( "abc", node.getName() )
    node.setName( "def" );
    CHECK_EQUAL( "def", node.getName() )
    
    // obtain the node again to make sure it changed in the xdoc
    auto reobtainedRoot = xdoc->getRoot();
    auto reobtainedIter = reobtainedRoot->elementsBegin();
    CHECK_EQUAL( "def", reobtainedIter->getName() );
}
T_END


TEST( copyConstructor, XElementIterator )
{
    auto xdoc = doc();
    auto root = xdoc->getRoot();
    XElementIterator iter = root->elementsBegin();
    XElementIterator copy{ iter };

    CHECK_EQUAL( "abc", iter->getName() )
    CHECK_EQUAL( "abc", copy->getName() )
    
    copy->setName( "def" );

    CHECK_EQUAL( "def", iter->getName() )
    CHECK_EQUAL( "def", copy->getName() )
}
T_END


TEST( moveConstructor, XElementIterator )
{
    auto xdoc = doc();
    auto root = xdoc->getRoot();
    XElementIterator moved{ root->elementsBegin() }; // maybe it calls move ctor...
    
    auto& node = *moved;
    CHECK_EQUAL( "abc", node.getName() )
    node.setName( "def" );
    CHECK_EQUAL( "def", node.getName() )
    
    // obtain the node again to make sure it changed in the xdoc
    auto reobtainedRoot = xdoc->getRoot();
    auto reobtainedIter = reobtainedRoot->elementsBegin();
    CHECK_EQUAL( "def", reobtainedIter->getName() );
}
T_END


TEST( assignmentOperator, XElementIterator )
{
    auto xdoc = doc();
    auto root = xdoc->getRoot();
    XElementIterator iter = root->elementsBegin();
    XElementIterator assigned;
    assigned = iter;
    
    CHECK_EQUAL( "abc", iter->getName() )
    CHECK_EQUAL( "abc", assigned->getName() )
    
    assigned->setName( "def" );
    
    CHECK_EQUAL( "def", iter->getName() )
    CHECK_EQUAL( "def", assigned->getName() )
}


TEST( moveAssignment, XElementIterator )
{
    auto xdoc = doc();
    auto root = xdoc->getRoot();
    XElementIterator moved;
    moved = root->elementsBegin(); // maybe it calls move assignment...
    
    auto& node = *moved;
    CHECK_EQUAL( "abc", node.getName() )
    node.setName( "def" );
    CHECK_EQUAL( "def", node.getName() )
    
    // obtain the node again to make sure it changed in the xdoc
    auto reobtainedRoot = xdoc->getRoot();
    auto reobtainedIter = reobtainedRoot->elementsBegin();
    CHECK_EQUAL( "def", reobtainedIter->getName() );
}
T_END


TEST( nullThrows, XElementIterator )
{
    XElementIterator nullIter;
    CHECK_RAISES( nullIter->getName() )
    CHECK_RAISES( (*nullIter).getName() )
    
}


TEST( doesNotCrash, XElementIterator )
{
    XElementIterator nullIter;
    XElementIterator anotherIter;
    CHECK_NOT_RAISES( anotherIter = nullIter );
    CHECK_NOT_RAISES( anotherIter = XElementIterator{} );
}

