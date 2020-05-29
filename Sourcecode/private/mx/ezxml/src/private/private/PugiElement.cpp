// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "private/PugiElement.h"
#include "private/PugiElementIterImpl.h"
#include "ezxml/XAttributeIterImpl.h"
#include "private/PugiAttributeIterImpl.h"
#include "private/XThrow.h"

#define EZXML_CHECK_NULL_NODE if( getIsNull() ) { EZXML_THROW_XNULL; }
#define EZXML_CHECK_NODE_ELEMENT if ( myNodeType != pugi::node_element && myNodeType != pugi::node_pi ) { EZXML_THROW( "bad internal state, node should be an element" ); }

namespace ezxml
{

    PugiElement::PugiElement()
            : myNode(), myXDoc( XDocCPtr{ nullptr } ), myNodeType{ pugi::xml_node_type::node_null }, myEndIter{}
    {
        update();
    }


    PugiElement::PugiElement(
            const pugi::xml_node& node,
            const XDocCPtr& xdoc
    )
            : myNode( node ), myXDoc( xdoc ), myNodeType{ pugi::xml_node_type::node_null }, myEndIter{}
    {
        update();
        const bool isElement = myNodeType == pugi::node_element;
        const bool isProcessingInstruction = myNodeType == pugi::node_pi;

        if( ( !isElement ) && ( !isProcessingInstruction ) )
        {
            EZXML_THROW( "bad internal state, node should be an element" );
        }
    }


    XElementPtr
    PugiElement::clone() const
    {
        return XElementPtr( new PugiElement{ myNode, myXDoc.lock() } );
    }


    XElementType
    PugiElement::getType() const
    {
        if( getIsNull() )
        {
            return XElementType::null;
        }

        if( std::string{ myNode.text().as_string() }.length() > 0 )
        {
            return XElementType::text;
        }

        if( begin() == end() )
        {
            return XElementType::empty;
        }

        return XElementType::element;
    }


    bool
    PugiElement::getIsNull() const
    {
        auto ptr = myXDoc.lock();

        if( !ptr )
        {
            return true;
        }
        else if( myNodeType != pugi::node_element )
        {
            if( myNodeType != pugi::node_pi )
            {
                return true;
            }
        }

        return false;
    }


    bool
    PugiElement::getIsProcessingInstruction() const
    {
        if( myNodeType == pugi::node_pi )
        {
            return true;
        }

        return false;
    }


    std::string
    PugiElement::getName() const
    {
        if( getIsNull() )
        {
            return "";
        }
        return std::string{ myNode.name() };
    }


    std::string
    PugiElement::getValue() const
    {
        if( getIsNull() )
        {
            return std::string{};
        }

        if( getIsProcessingInstruction() )
        {
            return std::string{ myNode.value() };
        }
        else
        {
            return std::string{ myNode.text().as_string() };
        }
    }


    void
    PugiElement::setName( const std::string& name )
    {
        if( getIsNull() )
        {
            return;
        }
        myNode.set_name( name.c_str() );
        update();
    }


    void
    PugiElement::setValue( const std::string& value )
    {
        if( getIsNull() )
        {
            return;
        }
        XElementType xetype = getType();

        if( xetype == XElementType::element )
        {
            EZXML_THROW( "the object cannot hold both elements and text" );
        }
        else if( xetype == XElementType::empty )
        {
            auto newnode = myNode.prepend_child( pugi::node_pcdata );
            newnode.set_value( value.c_str() );
            update();
            return;
        }
        else if( xetype == XElementType::text )
        {
            auto it = myNode.begin();
            it->set_value( value.c_str() );
            update();
            return;
        }
    }


    XDocCPtr
    PugiElement::getDoc() const
    {
        return myXDoc.lock();
    }


    XElementPtr
    PugiElement::getParent() const
    {
        EZXML_CHECK_NULL_NODE;
        EZXML_CHECK_NODE_ELEMENT;
        return XElementPtr{ new PugiElement{ myNode.parent(), myXDoc.lock() } };
    }


    XElementPtr
    PugiElement::getNextSibling() const
    {
        EZXML_CHECK_NULL_NODE;
        EZXML_CHECK_NODE_ELEMENT;
        const auto nextSibling = myNode.next_sibling();

        if( nextSibling.type() == pugi::node_null )
        {
            return XElementPtr{};
        }

        if( nextSibling.type() == pugi::node_element )
        {
            return XElementPtr{ new PugiElement{ nextSibling, myXDoc.lock() } };
        }

        if( nextSibling.type() == pugi::node_pi )
        {
            return XElementPtr{ new PugiElement{ nextSibling, myXDoc.lock() } };
        }

        return XElementPtr{};
    }


    XElementIterator
    PugiElement::begin() const
    {
        auto iter = beginWithProcessingInstructions();
        iter.setSkipProcessingInstructions( true );

        if( iter.getSkipProcessingInstructions() &&
            iter != end() &&
            iter->getIsProcessingInstruction() )
        {
            ++iter;
        }

        return iter;
    }


    XElementIterator
    PugiElement::beginWithProcessingInstructions() const
    {
        EZXML_CHECK_NULL_NODE;
        EZXML_CHECK_NODE_ELEMENT;
        const auto beginIter = myNode.begin();

        if( beginIter == myNode.end() )
        {
            return this->end();
        }

        const auto type = beginIter->type();

        if( type == pugi::node_element ||
            type == pugi::node_pi )
        {
            auto result = XElementIterator( PugiElementIterImpl{ myNode.begin(), myNode, myXDoc.lock() } );
            result.setSkipProcessingInstructions( false );
            return result;
        }

        return this->end();
    }


    XElementIterator
    PugiElement::end() const
    {
        EZXML_CHECK_NULL_NODE;
        EZXML_CHECK_NODE_ELEMENT;
        return myEndIter;
    }


    XAttributeIterator
    PugiElement::attributesBegin() const
    {
        EZXML_CHECK_NULL_NODE;
        EZXML_CHECK_NODE_ELEMENT;
        return XAttributeIterator( PugiAttributeIterImpl{ myNode.attributes_begin(), myNode, myXDoc.lock() } );
    }


    XAttributeIterator
    PugiElement::attributesEnd() const
    {
        EZXML_CHECK_NULL_NODE;
        EZXML_CHECK_NODE_ELEMENT;
        return XAttributeIterator( PugiAttributeIterImpl{ myNode.attributes_end(), myNode, myXDoc.lock() } );
    }


    XElementPtr
    PugiElement::appendChild( const std::string& name )
    {
        EZXML_CHECK_NULL_NODE;
        EZXML_CHECK_NODE_ELEMENT;
        const auto result = XElementPtr{ new PugiElement{ myNode.append_child( name.c_str() ), myXDoc.lock() } };
        update();
        return result;
    }


    XElementPtr
    PugiElement::prependChild( const std::string& name )
    {
        EZXML_CHECK_NULL_NODE;
        EZXML_CHECK_NODE_ELEMENT;
        const auto result = XElementPtr{ new PugiElement{ myNode.prepend_child( name.c_str() ), myXDoc.lock() } };
        update();
        return result;
    }


    XElementPtr
    PugiElement::insertSiblingAfter( const std::string& newElementName )
    {
        EZXML_CHECK_NULL_NODE;
        EZXML_CHECK_NODE_ELEMENT;
        auto newNode = myNode.parent().insert_child_after( newElementName.c_str(), myNode );
        const auto result = XElementPtr{ new PugiElement{ newNode, myXDoc.lock() } };
        update();
        return result;
    }


    bool
    PugiElement::removeChild( const std::string& elementName )
    {
        const auto result = myNode.remove_child( elementName.c_str() );
        update();
        return result;
    }


    XAttributePtr
    PugiElement::appendAttribute( const std::string& name )
    {
        EZXML_CHECK_NULL_NODE;
        EZXML_CHECK_NODE_ELEMENT;
        const auto result = XAttributePtr{
                new PugiAttribute{ myNode.append_attribute( name.c_str() ), myNode, myXDoc.lock() } };
        update();
        return result;
    }


    XAttributePtr
    PugiElement::prependAttribute( const std::string& name )
    {
        EZXML_CHECK_NULL_NODE;
        EZXML_CHECK_NODE_ELEMENT;
        const auto result = XAttributePtr{
                new PugiAttribute{ myNode.prepend_attribute( name.c_str() ), myNode, myXDoc.lock() } };
        update();
        return result;
    }


    void
    PugiElement::removeAttribute( const XAttributeIterator& iter )
    {
        auto it = myNode.attributes_begin();
        auto e = myNode.attributes_end();
        for( ; it != e; ++it )
        {
            if( iter->getName() == it->name() )
            {
                myNode.remove_attribute( *it );
                update();
                return;
            }
        }
    }


    void
    PugiElement::update()
    {
        myNodeType = myNode.type();
        myEndIter = XElementIterator( PugiElementIterImpl{ myNode.end(), myNode, myXDoc.lock() } );
    }
}

