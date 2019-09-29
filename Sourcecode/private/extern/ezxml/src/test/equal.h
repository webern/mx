#pragma once

#include "ezxml/ezxml.h"

namespace ezxmltest
{
    inline std::string
    elementsEqual( const std::string& inParentName, const ezxml::XElement& a, const ezxml::XElement& b );

    inline std::string
    attributesEqual(
            const std::string& inParentName,
            ezxml::XAttributeIterator aitera,
            const ezxml::XAttributeIterator& aenda,
            ezxml::XAttributeIterator aiterb,
            const ezxml::XAttributeIterator& aendb
    );


    inline std::string
    elementsEqual(
            const std::string& inParentName,
            ezxml::XElementIterator eitera,
            const ezxml::XElementIterator& eenda,
            ezxml::XElementIterator eiterb,
            const ezxml::XElementIterator& eendb
    )
    {
        bool sameEndish = ( eitera == eenda ) == ( eiterb == eendb );

        if( !sameEndish )
        {
            return inParentName + ": one had zero children while the other had some children";
        }

        for( ; ( eitera != eenda ) && ( eiterb != eendb ); ++eitera, ++eiterb )
        {
            if( eitera.getIsPayloadNull() || eiterb.getIsPayloadNull() )
            {
                return inParentName + ": cannot proceed due to null xelement";
            }

            const auto result = elementsEqual( inParentName, *eitera, *eiterb );

            if( !result.empty() )
            {
                return result;
            }
        }

        sameEndish = ( eitera == eenda ) == ( eiterb == eendb );

        if( !sameEndish )
        {
            return inParentName + ": had different numbers of children";
        }

        return "";
    }


    inline std::string
    attributesEqual(
            const std::string& inParentName,
            ezxml::XAttributeIterator aitera,
            const ezxml::XAttributeIterator& aenda,
            ezxml::XAttributeIterator aiterb,
            const ezxml::XAttributeIterator& aendb
    )
    {
        bool sameEndish = ( aitera == aenda ) == ( aiterb == aendb );

        if( !sameEndish )
        {
            return inParentName + ": one had zero attributes while the other had some attributes";
        }

        for( ; ( aitera != aenda ) && ( aiterb != aendb ); ++aitera, ++aiterb )
        {
            if( aitera->getName() != aiterb->getName() )
            {
                return inParentName + ": differing attribute names found, " + aitera->getName() + " vs " +
                       aiterb->getName();
            }

            if( aitera->getValue() != aiterb->getValue() )
            {
                return inParentName + ": differing attribute values found, " + aitera->getValue() + " vs " +
                       aiterb->getValue();
            }

        }

        sameEndish = ( aitera == aenda ) == ( aiterb == aendb );

        if( !sameEndish )
        {
            return inParentName + ": had different numbers of attributes";
        }

        return "";
    }


    inline std::string
    elementsEqual( const std::string& inParentName, const ezxml::XElement& a, const ezxml::XElement& b )
    {
        const auto myName = a.getName();

        if( myName != b.getName() )
        {
            return inParentName + "/" + myName + ": element names not equal: " + myName + " vs " + b.getName();
        }

        if( a.getType() != b.getType() )
        {
            return inParentName
                   + "/"
                   + a.getName()
                   + ": element types not equal: "
                   + ezxml::toString( a.getType() ) + " vs "
                   + ezxml::toString( b.getType() );
        }

        auto aitera = a.attributesBegin();
        const auto aenda = a.attributesEnd();
        auto aiterb = b.attributesBegin();
        const auto aendb = b.attributesEnd();

        const auto attributeResult = attributesEqual( a.getName(), aitera, aenda, aiterb, aendb );

        if( !attributeResult.empty() )
        {
            return attributeResult;
        }

        if( a.getType() == ezxml::XElementType::element )
        {
            auto eitera = a.begin();
            const auto eenda = a.end();
            auto eiterb = b.begin();
            const auto eendb = b.end();

            const auto childrenResult = elementsEqual( a.getName(), eitera, eenda, eiterb, eendb );

            if( !childrenResult.empty() )
            {
                return childrenResult;
            }
        }
        else if( a.getType() == ezxml::XElementType::text )
        {
            const auto myVal = a.getValue();

            if( myVal != b.getValue() )
            {
                return inParentName + "/" + myName + ": element values not equal: " + myVal + " vs " + b.getValue();
            }
        }

        return "";
    }


    inline std::string
    equal( const ezxml::XDocPtr& a, const ezxml::XDocPtr& b )
    {
        using namespace ezxml;

        if( a == nullptr || b == nullptr )
        {
            return "cannot proceed due to null xdoc";
        }

        if( a->getXmlVersion() != b->getXmlVersion() )
        {
            return "xml-versions not equal";
        }

        if( a->getEncoding() != b->getEncoding() )
        {
            return "getEncoding not equal";
        }

        if( a->getHasStandaloneAttribute() != b->getHasStandaloneAttribute() )
        {
            return "getHasStandaloneAttribute not equal";
        }

        if( a->getIsStandalone() != b->getIsStandalone() )
        {
            return "getIsStandalone not equal";
        }

        if( a->getHasDoctypeDeclaration() != b->getHasDoctypeDeclaration() )
        {
            return "getHasDoctypeDeclaration not equal";
        }

        if( a->getDoctypeValue() != b->getDoctypeValue() )
        {
            return "getDoctypeValue not equal";
        }

        const auto roota = a->getRoot();
        const auto rootb = b->getRoot();

        if( roota == nullptr || rootb == nullptr )
        {
            return "cannot proceed due to a null root";
        }

        const auto result = elementsEqual( "document", *roota, *rootb );

        if( !result.empty() )
        {
            return result;
        }

        return "";
    }
}