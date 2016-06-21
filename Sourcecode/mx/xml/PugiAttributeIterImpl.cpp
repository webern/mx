
#include "mx/xml/PugiAttributeIterImpl.h"
#include "mx/xml/XAttribute.h"
#include "mx/xml/XDoc.h"
#include "mx/xml/XThrow.h"

#define MX_CHECK_NODE_STATE if( myParentElement.type() != pugi::node_element ) { MX_THROW_XNULL; }

namespace mx
{
    namespace xml
    {
        PugiAttributeIterImpl::PugiAttributeIterImpl()
        : myIter()
        , myParentElement()
        , myXDoc()
        , myReturnableAttribute()
        {
            
        }


        PugiAttributeIterImpl::PugiAttributeIterImpl(
            const pugi::xml_attribute_iterator& iter,
            const pugi::xml_node& iterParentElement,
            const XDocCPtr& parentXDoc )
        : myIter( iter )
        , myParentElement( iterParentElement )
        , myXDoc( parentXDoc )
        , myReturnableAttribute()
        {
            MX_CHECK_NODE_STATE;
            if( myParentElement.type() != pugi::node_element )
            {
                MX_THROW( "improperly constructed, parent should be of element type" );
            }
        }


        bool PugiAttributeIterImpl::getIsPayloadNull() const
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


        bool PugiAttributeIterImpl::getIsEndIter() const
        {
            if ( myIter == myParentElement.attributes_end() )
            {
                return true;
            }

            return false;
        }


        XAttributeIterImplUP PugiAttributeIterImpl::clone() const
        {
            return XAttributeIterImplUP{ new PugiAttributeIterImpl{ *this } };
        }
        
        
        bool PugiAttributeIterImpl::equals( const XAttributeIterator& other ) const
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
        
        
        XAttribute& PugiAttributeIterImpl::getRef() const
        {
            if( getIsPayloadNull() )
            {
                MX_THROW_XNULL;
            }

            if ( getIsEndIter() )
            {
                MX_THROW( "XAttributeIterator attempted to dereference and 'end' iterator" );
            }

            myReturnableAttribute = PugiAttribute{ *myIter, myParentElement, myXDoc.lock() };
            return myReturnableAttribute;
        }
        
        
        XAttribute* PugiAttributeIterImpl::getPtr() const
        {
            if( getIsPayloadNull() )
            {
                MX_THROW_XNULL;
            }

            if ( getIsEndIter() )
            {
                MX_THROW( "XAttributeIterator attempted to dereference and 'end' iterator" );
            }

            myReturnableAttribute = PugiAttribute{ *myIter, myParentElement, myXDoc.lock() };
            return &myReturnableAttribute;
        }
        
        const PugiAttributeIterImpl& PugiAttributeIterImpl::increment()
        {
            ++myIter;
            return *this;
        }
        
        
        const PugiAttributeIterImpl& PugiAttributeIterImpl::decrement()
        {
            --myIter;
            return *this;
        }
    }
}
