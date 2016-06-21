// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/xml/PugiElement.h"
#include "mx/xml/PugiElementIterImpl.h"
#include "mx/xml/XAttributeIterImpl.h"
#include "mx/xml/PugiAttributeIterImpl.h"
#include "mx/xml/XDoc.h"
#include "mx/xml/XThrow.h"

#define MX_CHECK_NULL_NODE if( getIsNull() ) { MX_THROW_XNULL; }
#define MX_CHECK_NODE_ELEMENT if ( myNode.type() != pugi::node_element ) { MX_THROW( "bad internal state, node should be an element" ); }

namespace mx
{
    namespace xml
    {
        PugiElement::PugiElement()
        : myNode()
        , myXDoc( XDocCPtr{ nullptr} )
        {

        }


        PugiElement::PugiElement(
            const pugi::xml_node& node,
            const XDocCPtr& xdoc )
        : myNode( node )
        , myXDoc( xdoc )
        {
            MX_CHECK_NODE_ELEMENT;
        }


        XElementPtr PugiElement::clone() const
        {
            return XElementPtr( new PugiElement{ myNode, myXDoc.lock() } );
        }


        XElementType PugiElement::getType() const
        {
            if( getIsNull() )
            {
                return XElementType::null;
            }

            if( std::string{ myNode.text().as_string() }.length() > 0 )
            {
                return XElementType::text;
            }

            if( elementsBegin() == elementsEnd() )
            {
                return XElementType::empty;
            }

            return XElementType::element;
        }


        bool PugiElement::getIsNull() const
        {
            auto ptr = myXDoc.lock();
            if( !ptr )
            {
                return true;
            }
            else if( myNode.type() != pugi::node_element )
            {
                return true;
            }
            return false;
        }


        std::string PugiElement::getName() const
        {
            if( getIsNull() )
            {
                return "";
            }
            return std::string{ myNode.name() };
        }


        std::string PugiElement::getValue() const
        {
            if( getIsNull() )
            {
                return std::string{};
            }
            return std::string{ myNode.text().as_string() };
        }


        void PugiElement::setName( const std::string& name )
        {
            if( getIsNull() )
            {
                return;
            }
            myNode.set_name( name.c_str() );
        }


        void PugiElement::setValue( const std::string& value )
        {
            if( getIsNull() )
            {
                return;
            }
            XElementType xetype = getType();
            
            if( xetype == XElementType::element )
            {
                MX_THROW( "the object cannot hold both elements and text" );
            }
            else if( xetype == XElementType::empty )
            {
                auto newnode = myNode.prepend_child( pugi::node_pcdata );
                newnode.set_value( value.c_str() );
            }
            else if( xetype == XElementType::text )
            {
                auto it = myNode.begin();
                it->set_value( value.c_str() );
            }
        }


        XDocCPtr PugiElement::getDoc() const
        {
            return myXDoc.lock();
        }


        XElementPtr PugiElement::getParent() const
        {
            MX_CHECK_NULL_NODE;
            MX_CHECK_NODE_ELEMENT;
            return XElementPtr{ new PugiElement{ myNode.parent(), myXDoc.lock() } };
        }


        XElementIterator PugiElement::elementsBegin() const
        {
            MX_CHECK_NULL_NODE;
            MX_CHECK_NODE_ELEMENT;
            return XElementIterator( PugiElementIterImpl{ myNode.begin(), myNode, myXDoc.lock() } );
        }


        XElementIterator PugiElement::elementsEnd() const
        {
            MX_CHECK_NULL_NODE;
            MX_CHECK_NODE_ELEMENT;
            return XElementIterator( PugiElementIterImpl{ myNode.end(), myNode, myXDoc.lock() } );
        }


        XAttributeIterator PugiElement::attributesBegin() const
        {
            MX_CHECK_NULL_NODE;
            MX_CHECK_NODE_ELEMENT;
            return XAttributeIterator( PugiAttributeIterImpl{ myNode.attributes_begin(), myNode, myXDoc.lock() } );
        }


        XAttributeIterator PugiElement::attributesEnd() const
        {
            MX_CHECK_NULL_NODE;
            MX_CHECK_NODE_ELEMENT;
            return XAttributeIterator( PugiAttributeIterImpl{ myNode.attributes_end(), myNode, myXDoc.lock() } );
        }


        XAttributePtr PugiElement::appendAttribute( const std::string & name )
        {
            MX_CHECK_NULL_NODE;
            MX_CHECK_NODE_ELEMENT;
            if( getType() == XElementType::text )
            {
                MX_THROW( "the object cannot hold both elements and text" );
            }
            return XAttributePtr{ new PugiAttribute{ myNode.append_attribute( name.c_str() ), myNode, myXDoc.lock() } };
        }


        XAttributePtr PugiElement::prependAttribute( const std::string & name )
        {
            MX_CHECK_NULL_NODE;
            MX_CHECK_NODE_ELEMENT;
            if( getType() == XElementType::text )
            {
                MX_THROW( "the object cannot hold both elements and text" );
            }
            return XAttributePtr{ new PugiAttribute{ myNode.prepend_attribute( name.c_str() ), myNode, myXDoc.lock() } };
        }
    }
}