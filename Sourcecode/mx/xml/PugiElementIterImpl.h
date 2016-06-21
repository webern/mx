// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

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

            PugiElementIterImpl();

            PugiElementIterImpl(
                const pugi::xml_node_iterator& iter,
                const pugi::xml_node& iterParent,
                const XDocCPtr& parentDoc );

            PugiElementIterImpl( const PugiElementIterImpl& ) = default;
            PugiElementIterImpl( PugiElementIterImpl&& ) = default;
            ~PugiElementIterImpl() = default;

            PugiElementIterImpl& operator=( const PugiElementIterImpl& ) = default;
            PugiElementIterImpl& operator=( PugiElementIterImpl&& ) = default;

            virtual bool getIsPayloadNull() const override;
            virtual bool getIsEndIter() const override;
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
            mutable PugiElement myReturnableElement;
        };
    }
}
