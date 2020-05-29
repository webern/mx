#pragma once

#include <iostream>
#include <memory>
#include <cstddef>
#include <cstdint>

namespace ezxml
{
    enum class XElementType
    {
        null,           // The element is in a bad, unusable state and getIsNull will return true
        empty,          // The element has no text and no child elements
        element,        // The element has child elements
        text,           // The element has text data
    };

    class XDoc;
    using XDocCPtr = std::shared_ptr<const XDoc>;

    class XElement;
    using XElementPtr = std::shared_ptr<XElement>;

    class XAttribute;
    using XAttributePtr = std::shared_ptr<XAttribute>;

    class XElementIterator;
    using XElementIteratorPtr = std::shared_ptr<XElementIterator>;

    class XAttributeIterator;
    using XAttributeIteratorPtr = std::shared_ptr<XAttributeIterator>;

    class XElement
    {
    public:
        virtual ~XElement() = default;
        virtual XElementPtr clone() const = 0;

        virtual XElementType getType() const = 0;
        virtual bool getIsNull() const = 0;

        virtual bool getIsProcessingInstruction() const = 0;

        virtual std::string getName() const = 0;
        virtual std::string getValue() const = 0;

        virtual void setName( const std::string& name ) = 0;
        virtual void setValue( const std::string& name ) = 0;

        // get the XDoc from which this
        // entire XML tree descends
        virtual XDocCPtr getDoc() const = 0;

        // get the element that contains this element
        // returns an XElement with type == null if this
        // element is the root of the entire XML tree
        virtual XElementPtr getParent() const = 0;

        // return the next element after this one, can
        // return nullptr if there are no more siblings
        virtual XElementPtr getNextSibling() const = 0;

        // STL compliant iterators to the elements
        // which are children of this element. If
        // this element has text or has no children
        // then begin() == end()
        virtual XElementIterator begin() const = 0;
        virtual XElementIterator end() const = 0;

        // if you want iterations to include processing
        // instructions then use this function.
        virtual XElementIterator beginWithProcessingInstructions() const = 0;

        // STL compliant iterators to the attributes
        // of this element. If this element has no
        // attributes then begin() == end()
        virtual XAttributeIterator attributesBegin() const = 0;
        virtual XAttributeIterator attributesEnd() const = 0;

        // add an element as the last child of this element.
        // throws if this element's type == text
        virtual XElementPtr appendChild( const std::string& name ) = 0;

        // add an element as the first child of this element.
        // throws if this element's type == text
        virtual XElementPtr prependChild( const std::string& name ) = 0;

        // removes the first occurence of a child element with the given name
        virtual bool removeChild( const std::string& elementName ) = 0;

        // adds an element after this element, at this element's
        // same level in the tree. throws if this element is the
        // root of the entire XML tree
        virtual XElementPtr insertSiblingAfter( const std::string& newElementName ) = 0;

        virtual XAttributePtr appendAttribute( const std::string& name ) = 0;
        virtual XAttributePtr prependAttribute( const std::string& name ) = 0;
        virtual void removeAttribute( const XAttributeIterator& iter ) = 0;
    };



    // utils for the XElementType enum


    constexpr const char* const xElementNullString = "XELEMENT_NULL";
    constexpr const char* const xElementEmptyString = "XELEMENT_EMPTY";
    constexpr const char* const xElementElementString = "XELEMENT_ELEMENT";
    constexpr const char* const xElementTextString = "XELEMENT_TEXT";
    constexpr const char* const xElementUnknownString = "XELEMENT_UNKNOWN";


    inline std::string
    toString( XElementType t )
    {
        switch( t )
        {
            case XElementType::null:return xElementNullString;
            case XElementType::empty:return xElementEmptyString;
            case XElementType::element:return xElementElementString;
            case XElementType::text:return xElementTextString;
            default: break;
        }

        return xElementUnknownString;
    }


    inline XElementType
    xElementTypeFromString( const std::string& inString )
    {
        if( inString == xElementNullString )
        {
            return XElementType::null;
        }
        else if( inString == xElementEmptyString )
        {
            return XElementType::empty;
        }
        else if( inString == xElementElementString )
        {
            return XElementType::element;
        }

        return XElementType::null;
    }
}
