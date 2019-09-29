// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_XML_TESTS

#include "cpul/cpulTestHarness.h"
#include "ezxml/XFactory.h"
#include "ezxml/XDoc.h"
#include "ezxml/XAttribute.h"
#include "ezxml/XAttributeIterator.h"
#include "ezxml/XElementIterator.h"
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


    inline XElementPtr somethingWithAttributes( const  ::ezxml::XDocCPtr& xdoc )
    {
        auto root = xdoc->getRoot();
        auto firstIter = root->begin();
        ++firstIter;
        ++firstIter;
        auto nextIter = firstIter->begin();
        return nextIter->clone();
    }
}


TEST( getNameValue, XAttribute )
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


TEST( setName, XAttribute )
{
    auto xdoc = doc();
    auto node = somethingWithAttributes( xdoc );
    auto iter = node->attributesBegin();
    CHECK( iter != node->attributesEnd() );
    CHECK_EQUAL( "attr-a", iter->getName() );
    CHECK_EQUAL( "A", iter->getValue() );
    ++iter;
    CHECK( iter != node->attributesEnd() );

    iter->setName( "hello-attr" );

    CHECK_EQUAL( "hello-attr", iter->getName() );
    CHECK_EQUAL( "B", iter->getValue() );
    ++iter;
    CHECK( iter != node->attributesEnd() );
    CHECK_EQUAL( "attr-c", iter->getName() );
    CHECK_EQUAL( "C", iter->getValue() );
    ++iter;
    CHECK( iter == node->attributesEnd() );

    node = somethingWithAttributes( xdoc );
    iter = node->attributesBegin();
    CHECK( iter != node->attributesEnd() );
    CHECK_EQUAL( "attr-a", iter->getName() );
    CHECK_EQUAL( "A", iter->getValue() );
    ++iter;
    CHECK( iter != node->attributesEnd() );
    CHECK_EQUAL( "hello-attr", iter->getName() );
    CHECK_EQUAL( "B", iter->getValue() );
    ++iter;
    CHECK( iter != node->attributesEnd() );
    CHECK_EQUAL( "attr-c", iter->getName() );
    CHECK_EQUAL( "C", iter->getValue() );
    ++iter;
    CHECK( iter == node->attributesEnd() );
}
T_END


TEST( setValue, XAttribute )
{
    auto xdoc = doc();
    auto node = somethingWithAttributes( xdoc );
    auto iter = node->attributesBegin();
    CHECK( iter != node->attributesEnd() );
    CHECK_EQUAL( "attr-a", iter->getName() );
    CHECK_EQUAL( "A", iter->getValue() );
    ++iter;
    CHECK( iter != node->attributesEnd() );
    
    iter->setValue( "hello-attr" );
    
    CHECK_EQUAL( "attr-b", iter->getName() );
    CHECK_EQUAL( "hello-attr", iter->getValue() );
    ++iter;
    CHECK( iter != node->attributesEnd() );
    CHECK_EQUAL( "attr-c", iter->getName() );
    CHECK_EQUAL( "C", iter->getValue() );
    ++iter;
    CHECK( iter == node->attributesEnd() );

    node = somethingWithAttributes( xdoc );
    iter = node->attributesBegin();
    CHECK( iter != node->attributesEnd() );
    CHECK_EQUAL( "attr-a", iter->getName() );
    CHECK_EQUAL( "A", iter->getValue() );
    ++iter;
    CHECK( iter != node->attributesEnd() );
    CHECK_EQUAL( "attr-b", iter->getName() );
    CHECK_EQUAL( "hello-attr", iter->getValue() );
    ++iter;
    CHECK( iter != node->attributesEnd() );
    CHECK_EQUAL( "attr-c", iter->getName() );
    CHECK_EQUAL( "C", iter->getValue() );
    ++iter;
    CHECK( iter == node->attributesEnd() );
}
T_END

#endif
