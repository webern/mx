// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_XML_TESTS

#include "cpul/cpulTestHarness.h"
#include "ezxml/XFactory.h"
#include "ezxml/XDoc.h"
#include "ezxml/XElement.h"
#include "ezxml/XElementIterator.h"
#include "ezxml/XAttributeIterator.h"
#include "mxtest/xml/FakeXml.h"

using namespace std;
using namespace ezxml;



namespace
{
    inline ::ezxml::XDocCPtr doc()
    {
        auto xdoc = XFactory::makeXDoc();
        std::istringstream is( mxtest::fakeXml );
        xdoc->loadStream( is );
        return xdoc;
    }
}


TEST( useIteratorBasic, XElement )
{
    auto xdoc = doc();
    auto root = xdoc->getRoot();
    int i = 0;
    auto endIter = root->end();
    for( auto it = root->begin(); it != endIter; ++it, ++i )
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


TEST( getTypeElement, XElement )
{
    auto xdoc = doc();
    auto node = xdoc->getRoot();
    CHECK( !node->getIsNull() );
    CHECK_EQUAL( "root-node", node->getName() );
    CHECK_EQUAL( "", node->getValue() );
    CHECK( XElementType::element == node->getType() );
}
T_END


TEST( getTypeText, XElement )
{
    auto xdoc = doc();
    auto node = xdoc->getRoot();
    auto iter = node->begin();
    CHECK( !iter.getIsPayloadNull() );
    CHECK( !iter->getIsNull() );
    CHECK_EQUAL( "abc", iter->getName() );
    auto weird = iter->begin();
    CHECK( weird == iter->end() );
    CHECK_EQUAL( "abcdefg", iter->getValue() );
    CHECK( XElementType::text == iter->getType() );
}
T_END


TEST( getTypeEmpty, XElement )
{
    auto xdoc = doc();
    auto node = xdoc->getRoot();
    auto iter = node->begin();
    ++iter;
    CHECK( !iter.getIsPayloadNull() );
    CHECK( !iter->getIsNull() );
    CHECK_EQUAL( "empty-element", iter->getName() );
    CHECK( XElementType::empty == iter->getType() );
}
T_END


TEST( getValueEmptyElementEmptyString, XElement )
{
    auto xdoc = doc();
    auto node = xdoc->getRoot();
    auto iter = node->begin();
    ++iter;
    CHECK( !iter.getIsPayloadNull() );
    CHECK( !iter->getIsNull() );
    CHECK_EQUAL( "", iter->getValue() );
    CHECK( XElementType::empty == iter->getType() );
}
T_END


TEST( getValueEmptyElementTypeEmptyString, XElement )
{
    auto xdoc = doc();
    auto node = xdoc->getRoot();
    auto iter = node->begin();
    ++iter;
    ++iter;
    CHECK( !iter.getIsPayloadNull() );
    CHECK( !iter->getIsNull() );
    CHECK_EQUAL( "", iter->getValue() );
    CHECK( XElementType::element == iter->getType() );
}
T_END


TEST( getValue, XElement )
{
    auto xdoc = doc();
    auto node = xdoc->getRoot();
    auto iter = node->begin();
    CHECK( !iter.getIsPayloadNull() );
    CHECK( !iter->getIsNull() );
    CHECK_EQUAL( "abcdefg", iter->getValue() );
    CHECK( XElementType::text == iter->getType() );
}
T_END


TEST( setName, XElement )
{
    auto xdoc = doc();
    auto node1 = xdoc->getRoot();
    auto iter1 = node1->begin();
    CHECK_EQUAL( "abc", iter1->getName() );

    // function under test
    iter1->setName( "blah" );
    CHECK_EQUAL( "blah", iter1->getName() );

    auto node2 = xdoc->getRoot();
    auto iter2 = node1->begin();
    CHECK_EQUAL( "blah", iter1->getName() );
}
T_END


TEST( setValueChangeExisting, XElement )
{
    auto xdoc = doc();
    auto node1 = xdoc->getRoot();
    auto iter1 = node1->begin();
    CHECK_EQUAL( "abcdefg", iter1->getValue() );

    // function under test
    iter1->setValue( "blah" );
    CHECK_EQUAL( "blah", iter1->getValue() );

    auto node2 = xdoc->getRoot();
    auto iter2 = node1->begin();
    CHECK_EQUAL( "blah", iter1->getValue() );
}
T_END


TEST( setValueAddToEmpty, XElement )
{
    auto xdoc = doc();
    auto node1 = xdoc->getRoot();
    auto iter1 = node1->begin();
    ++iter1;
    CHECK_EQUAL( "", iter1->getValue() );
    CHECK( XElementType::empty == iter1->getType() );

    // function under test
    iter1->setValue( "blah" );
    CHECK_EQUAL( "blah", iter1->getValue() );
    CHECK( XElementType::text == iter1->getType() );

    auto node2 = xdoc->getRoot();
    auto iter2 = node1->begin();
    ++iter2;
    CHECK_EQUAL( "blah", iter1->getValue() );
    CHECK( XElementType::text == iter1->getType() );
}
T_END


TEST( getDoc, XElement )
{
    auto xdoc = doc();
    auto node = xdoc->getRoot();
    auto gotdoc = node->getDoc();
    CHECK_EQUAL( reinterpret_cast<size_t>( xdoc.get() ), reinterpret_cast<size_t>( gotdoc.get() ) )
}
T_END


TEST( getParent, XElement )
{
    auto xdoc = doc();
    auto node = xdoc->getRoot();
    auto iter = node->begin();
    auto parent = iter->getParent();
    CHECK_EQUAL( "root-node", node->getName() );
    CHECK_EQUAL( "root-node", parent->getName() )
}
T_END


TEST( attributesBeginEnd, XElement )
{
    auto xdoc = doc();
    auto root = xdoc->getRoot();
    CHECK( root->attributesBegin() == root->attributesEnd() );
    auto elIter = root->begin();
    ++elIter;
    CHECK( elIter->attributesBegin() != elIter->attributesEnd() );
}
T_END


TEST( prependAttributeFromEmpty, XElement )
{
    auto xdoc = doc();
    auto root = xdoc->getRoot();
    CHECK( root->attributesBegin() == root->attributesEnd() );
    root->prependAttribute( "prepended" )->setValue( "val-A" );
    CHECK( root->attributesBegin() != root->attributesEnd() );
    auto atIter = root->attributesBegin();
    CHECK_EQUAL( "prepended", atIter->getName() );
    CHECK_EQUAL( "val-A", atIter->getValue() );

    // start over make sure its there
    auto root2 = xdoc->getRoot();
    auto atIter2 = root2->attributesBegin();
    CHECK_EQUAL( "prepended", atIter2->getName() );
    CHECK_EQUAL( "val-A", atIter2->getValue() );
}
T_END


TEST( appendAttributeFromEmpty, XElement )
{
    auto xdoc = doc();
    auto root = xdoc->getRoot();
    CHECK( root->attributesBegin() == root->attributesEnd() );
    root->appendAttribute( "appended" )->setValue( "val-B" );
    CHECK( root->attributesBegin() != root->attributesEnd() );
    auto atIter = root->attributesBegin();
    CHECK_EQUAL( "appended", atIter->getName() );
    CHECK_EQUAL( "val-B", atIter->getValue() );

    // start over make sure its there
    auto root2 = xdoc->getRoot();
    auto atIter2 = root2->attributesBegin();
    CHECK_EQUAL( "appended", atIter2->getName() );
    CHECK_EQUAL( "val-B", atIter2->getValue() );
}
T_END


TEST( prependAttributeWithExisting, XElement )
{
    auto xdoc = doc();
    auto root = xdoc->getRoot();
    auto elIter = root->begin();
    ++elIter;
    ++elIter;
    auto& node = *elIter;
    auto atIter = node.attributesBegin();
    CHECK_EQUAL( "color", atIter->getName() );
    CHECK_EQUAL( "blue", atIter->getValue() );
    node.prependAttribute( "pre" )->setValue( "pended" );
    atIter = node.attributesBegin();
    CHECK_EQUAL( "pre", atIter->getName() );
    CHECK_EQUAL( "pended", atIter->getValue() );
    ++atIter;
    CHECK_EQUAL( "color", atIter->getName() );
    CHECK_EQUAL( "blue", atIter->getValue() );
}
T_END


TEST( appendAttributeWithExisting, XElement )
{
    auto xdoc = doc();
    auto root = xdoc->getRoot();
    auto elIter = root->begin();
    ++elIter;
    ++elIter;
    auto& node = *elIter;
    auto atIter = node.attributesBegin();
    CHECK_EQUAL( "color", atIter->getName() );
    CHECK_EQUAL( "blue", atIter->getValue() );
    node.appendAttribute( "ap" )->setValue( "pended" );
    atIter = node.attributesBegin();
    CHECK_EQUAL( "color", atIter->getName() );
    CHECK_EQUAL( "blue", atIter->getValue() );
    ++atIter;
    CHECK_EQUAL( "ap", atIter->getName() );
    CHECK_EQUAL( "pended", atIter->getValue() );
}
T_END


TEST( insertSiblingAfter, XElement )
{
    auto xdoc = XFactory::makeXDoc();
    auto root = xdoc->getRoot();
    root->appendChild( "one" );
    root->appendChild( "three" );
    root->appendChild( "four" );
    root->begin()->insertSiblingAfter( "two" );
    auto it = root->begin();
    CHECK_EQUAL( "one", it->getName() );
    ++it;
    CHECK_EQUAL( "two", it->getName() );
    ++it;
    CHECK_EQUAL( "three", it->getName() );
    ++it;
    CHECK_EQUAL( "four", it->getName() );
}
T_END


TEST( removeChild, XElement )
{
    auto xdoc = XFactory::makeXDoc();
    auto root = xdoc->getRoot();
    root->appendChild( "one" );
    root->appendChild( "two" );
    root->appendChild( "three" );
    root->appendChild( "four" );
    root->removeChild( "two" );
    auto it = root->begin();
    CHECK_EQUAL( "one", it->getName() );
    ++it;
    CHECK_EQUAL( "three", it->getName() );
    ++it;
    CHECK_EQUAL( "four", it->getName() );
}
T_END


#endif
