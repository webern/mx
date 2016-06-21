// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/xml/XElement.h"
#include "mx/pugixml/pugixml.hpp"

#include <memory>


namespace mx
{
    namespace xml
    {
        class PugiElement;
        using PugiElementPtr = std::shared_ptr<PugiElement>;

        class XDoc;
        using XDocCPtr = std::shared_ptr<const XDoc>;
        using XDocCWPtr = std::weak_ptr<const XDoc>;

        class PugiElement : public XElement
        {
        public:
            PugiElement();
            
            PugiElement(
                const pugi::xml_node& node,
                const XDocCPtr& xdoc );

            ~PugiElement() = default;

            // copy
            PugiElement( const PugiElement& other ) = default;
            PugiElement& operator=( const PugiElement& other ) = default;

            // move
            PugiElement( PugiElement&& other ) noexcept = default;
            PugiElement& operator=( PugiElement&& other ) noexcept = default;

            virtual XElementPtr clone() const override;

            virtual XElementType getType() const override;
            virtual bool getIsNull() const override;

            virtual std::string getName() const override;
            virtual std::string getValue() const override;

            virtual void setName( const std::string& name ) override;
            virtual void setValue( const std::string& name ) override;

            virtual XDocCPtr getDoc() const override;
            virtual XElementPtr getParent() const override;

            virtual XElementIterator elementsBegin() const override;
            virtual XElementIterator elementsEnd() const override;

            virtual XAttributeIterator attributesBegin() const override;
            virtual XAttributeIterator attributesEnd() const override;

            virtual XAttributePtr appendAttribute( const std::string& name ) override;
            virtual XAttributePtr prependAttribute( const std::string& name ) override;

        private:
            pugi::xml_node myNode;
            XDocCWPtr myXDoc;
        };
    }
}
