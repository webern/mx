// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once


#include "mx/xml/XElementIterator.h"
#include "mx/xml/XElementIterImpl.h"
#include "mx/xml/XDoc.h"
#include "mx/xml/PugiElement.h"
#include "mx/pugixml/pugixml.hpp"

namespace mx
{
    namespace xml
    {
        class PugiElementIterImpl : public XElementIterImpl
        {
        public:
            ~PugiElementIterImpl() = default;

            PugiElementIterImpl();

            PugiElementIterImpl(
                const pugi::xml_node_iterator& iter,
                const pugi::xml_node& iterParent,
                const XDocCPtr& parentDoc );

            PugiElementIterImpl( const PugiElementIterImpl& );
            PugiElementIterImpl( PugiElementIterImpl&& ) = default;

            PugiElementIterImpl& operator=( const PugiElementIterImpl& );
            PugiElementIterImpl& operator=( PugiElementIterImpl&& ) = default;

            virtual bool getIsPayloadNull() const override;
            virtual bool getIsEndIter() const override;
            virtual bool getIsProcessingInstruction() const override;
            virtual bool getSkipProcessingInstructions() const override;
            virtual void setSkipProcessingInstructions( bool inValue ) override;

            virtual XElementIterImplUP clone() const override;
            virtual bool equals( const XElementIterator& other ) const override;
            
            virtual XElement& getRef() const override;
            virtual XElement* getPtr() const override;
            
            virtual const PugiElementIterImpl& increment() override;
            virtual const PugiElementIterImpl& decrement() override;

            // not part of inherited interface
            virtual pugi::xml_node_type getPugiXmlNodeType() const final;
            virtual bool hasTextData() const final;

        public:
            pugi::xml_node_iterator myIter;
            pugi::xml_node myIterParent;
            XDocCWPtr myXDoc;
            bool mySkipProcessingInstructions;
            mutable std::unique_ptr<PugiElement> myReturnableElement;

        private:
            bool getIsBeginIter() const;
        };
    }
}
