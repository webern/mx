// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/xml/PugiElementIterImpl.h"
#include "mx/xml/XElement.h"
#include "mx/xml/XThrow.h"

namespace mx
{
    namespace xml
    {
        PugiElementIterImpl::PugiElementIterImpl()
        : myIter()
        , myIterParent()
        , myXDoc()
        {
            
        }
        
        
        PugiElementIterImpl::PugiElementIterImpl(
            const pugi::xml_node_iterator& iter,
            const pugi::xml_node& iterParent,
            const XDocCPtr& parentDoc )
        : myIter( iter )
        , myIterParent( iterParent )
        , myXDoc( parentDoc )
        {

        }


        XElementIterImplUP PugiElementIterImpl::clone() const
        {
            return XElementIterImplUP{ new PugiElementIterImpl{ *this } };
        }


        bool PugiElementIterImpl::getIsPayloadNull() const
        {
            auto ptr = myXDoc.lock();
            
            if( !ptr )
            {
                return true;
            }
            else if( myIterParent.type() != pugi::node_element )
            {
                return true;
            }

            return false;
        }


        bool PugiElementIterImpl::getIsEndIter() const
        {
            if ( myIter == myIterParent.end() )
            {
                return true;
            }

            return false;
        }


        bool PugiElementIterImpl::equals( const XElementIterator& other ) const
        {

            auto& otherXImplPtr = other.reveal();
            
            if( !otherXImplPtr )
            {
                return false;
            }

            auto otherPtr = dynamic_cast<PugiElementIterImpl*>( otherXImplPtr.get() );
            

            return myIter == otherPtr->myIter;
        }


        XElement& PugiElementIterImpl::getRef() const
        {
            if( getIsPayloadNull() )
            {
                MX_THROW_XNULL;
            }

            if ( getIsEndIter() )
            {
                MX_THROW( "XElementIterator attempted to dereference and 'end' iterator" );
            }

            myReturnableElement = PugiElement{ *myIter, myXDoc.lock() };
            return myReturnableElement;
        }


        XElement* PugiElementIterImpl::getPtr() const
        {
            if( getIsPayloadNull() )
            {
                MX_THROW_XNULL;
            }

            if ( getIsEndIter() )
            {
                MX_THROW( "XElementIterator attempted to dereference and 'end' iterator" );
            }

            myReturnableElement = PugiElement{ *myIter, myXDoc.lock() };
            return &myReturnableElement;
        }


        const PugiElementIterImpl& PugiElementIterImpl::increment()
        {
            ++myIter;
            return *this;
        }


        const PugiElementIterImpl& PugiElementIterImpl::decrement()
        {
            --myIter;
            return *this;
        }


        pugi::xml_node_type PugiElementIterImpl::getPugiXmlNodeType() const
        {
            if( getIsPayloadNull() )
            {
                return pugi::node_null;
            }
            return myIter->type();
        }


        bool PugiElementIterImpl::hasTextData() const
        {
            if( getIsPayloadNull() )
            {
                return false;
            }
            return std::string{ myIter->child_value() }.size() > 0;
        }

    }
}
