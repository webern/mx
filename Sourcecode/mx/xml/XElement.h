// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs


#pragma once

#include <iostream>
#include <memory>

namespace mx
{
    namespace xml
    {
        enum class XElementType
        {
            null,           // The element is in a bad, unusable state and getIsNull will return true
            empty,          // The element has no text and no child elements
            element,        // The element has child elements
            text,           // The element has text data and no child elements
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

            virtual std::string getName() const = 0;
            virtual std::string getValue() const = 0;

            virtual void setName( const std::string& name ) = 0;
            virtual void setValue( const std::string& name ) = 0;

            virtual XDocCPtr getDoc() const = 0;
            virtual XElementPtr getParent() const = 0;

            virtual XElementIterator elementsBegin() const = 0;
            virtual XElementIterator elementsEnd() const = 0;

            virtual XAttributeIterator attributesBegin() const = 0;
            virtual XAttributeIterator attributesEnd() const = 0;

            // TODO prepend element append element

            virtual XAttributePtr appendAttribute( const std::string& name ) = 0;
            virtual XAttributePtr prependAttribute( const std::string& name ) = 0;
        };
    }
}