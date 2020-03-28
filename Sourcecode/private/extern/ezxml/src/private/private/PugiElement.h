#pragma once

#include "ezxml/XElement.h"
#include <pugixml.hpp>
#include "ezxml/XElementIterator.h"

#include <memory>

namespace ezxml
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
                const XDocCPtr& xdoc
        );

        // copy
        PugiElement( const PugiElement& other ) = default;
        PugiElement& operator=( const PugiElement& other ) = default;

        // move
        PugiElement( PugiElement&& other ) noexcept = default;
        PugiElement& operator=( PugiElement&& other ) noexcept = default;

        virtual XElementPtr clone() const override;

        virtual XElementType getType() const override;
        virtual bool getIsNull() const override;

        virtual bool getIsProcessingInstruction() const override;

        virtual std::string getName() const override;
        virtual std::string getValue() const override;

        virtual void setName( const std::string& name ) override;
        virtual void setValue( const std::string& name ) override;

        virtual XDocCPtr getDoc() const override;
        virtual XElementPtr getParent() const override;
        virtual XElementPtr getNextSibling() const override;

        virtual XElementIterator begin() const override;
        virtual XElementIterator beginWithProcessingInstructions() const override;
        virtual XElementIterator end() const override;

        virtual XAttributeIterator attributesBegin() const override;
        virtual XAttributeIterator attributesEnd() const override;

        virtual XElementPtr appendChild( const std::string& name ) override;
        virtual XElementPtr prependChild( const std::string& name ) override;

        virtual XElementPtr insertSiblingAfter( const std::string& newElementName ) override;
        virtual bool removeChild( const std::string& elementName ) override;

        virtual XAttributePtr appendAttribute( const std::string& name ) override;
        virtual XAttributePtr prependAttribute( const std::string& name ) override;
        virtual void removeAttribute( const XAttributeIterator& iter ) override;

    private:
        pugi::xml_node myNode;
        XDocCWPtr myXDoc;
        pugi::xml_node_type myNodeType;
        XElementIterator myEndIter;

    private:
        void update();
    };
}

