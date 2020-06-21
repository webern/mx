// Copyright (c) Matthew James Briggs

#include "private/PugiAttributeIterImpl.h"
#include "ezxml/XDoc.h"
#include "private/XThrow.h"

#define EZXML_CHECK_NODE_STATE if( myParentElement.type() != pugi::node_element ) { EZXML_THROW_XNULL; }

namespace ezxml
{
    PugiAttributeIterImpl::PugiAttributeIterImpl()
            : myIter(), myParentElement(), myXDoc(), myReturnableAttribute()
    {

    }


    PugiAttributeIterImpl::PugiAttributeIterImpl(
            const pugi::xml_attribute_iterator& iter,
            const pugi::xml_node& iterParentElement,
            const XDocCPtr& parentXDoc
    )
            : myIter( iter ), myParentElement( iterParentElement ), myXDoc( parentXDoc ), myReturnableAttribute()
    {
        EZXML_CHECK_NODE_STATE;
        if( myParentElement.type() != pugi::node_element )
        {
            EZXML_THROW( "improperly constructed, parent should be of element type" );
        }
    }


    bool
    PugiAttributeIterImpl::getIsPayloadNull() const
    {
        auto ptr = myXDoc.lock();

        if( !ptr )
        {
            return true;
        }
        else if( myParentElement.type() != pugi::node_element )
        {
            return true;
        }

        return false;
    }


    bool
    PugiAttributeIterImpl::getIsEndIter() const
    {
        if( myIter == myParentElement.attributes_end() )
        {
            return true;
        }

        return false;
    }


    XAttributeIterImplUP
    PugiAttributeIterImpl::clone() const
    {
        return XAttributeIterImplUP{ new PugiAttributeIterImpl{ *this } };
    }


    bool
    PugiAttributeIterImpl::equals( const XAttributeIterator& other ) const
    {
        const auto& otherImpl = other.reveal();
        if( !otherImpl )
        {
            return false;
        }
        auto pugi = dynamic_cast<PugiAttributeIterImpl*>( otherImpl.get() );
        if( !pugi )
        {
            throw std::runtime_error( "PugiAttributeIterImpl::equals - a downcast failed" );
        }
        return this->myIter == pugi->myIter;
    }


    XAttribute&
    PugiAttributeIterImpl::getRef() const
    {
        if( getIsPayloadNull() )
        {
            EZXML_THROW_XNULL;
        }

        if( getIsEndIter() )
        {
            EZXML_THROW( "XAttributeIterator attempted to dereference an 'end' iterator" );
        }

        myReturnableAttribute = PugiAttribute{ *myIter, myParentElement, myXDoc.lock() };
        return myReturnableAttribute;
    }


    XAttribute*
    PugiAttributeIterImpl::getPtr() const
    {
        if( getIsPayloadNull() )
        {
            EZXML_THROW_XNULL;
        }

        if( getIsEndIter() )
        {
            EZXML_THROW( "XAttributeIterator attempted to dereference an 'end' iterator" );
        }

        myReturnableAttribute = PugiAttribute{ *myIter, myParentElement, myXDoc.lock() };
        return &myReturnableAttribute;
    }


    const PugiAttributeIterImpl&
    PugiAttributeIterImpl::increment()
    {
        ++myIter;
        return *this;
    }


    const PugiAttributeIterImpl&
    PugiAttributeIterImpl::decrement()
    {
        --myIter;
        return *this;
    }
}
