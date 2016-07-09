// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_XML_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/xml/XFactory.h"
#include "mx/xml/XDoc.h"
#include "mx/xml/XAttribute.h"
#include "mx/xml/XAttributeIterator.h"
#include "mx/xml/XElementIterator.h"
#include "mxtest/xml/FakeXml.h"

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


    inline XElementPtr somethingWithAttributes( const  mx::xml::XDocCPtr& xdoc )
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
