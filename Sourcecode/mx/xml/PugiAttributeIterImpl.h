// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once


#include "mx/xml/XAttributeIterator.h"
#include "mx/xml/XAttributeIterImpl.h"
#include "mx/xml/PugiAttribute.h"
#include "mx/pugixml/pugixml.hpp"

namespace mx
{
    namespace xml
    {
        class XDoc;
        using XDocCPtr = std::shared_ptr<const XDoc>;
        using XDocCWPtr = std::weak_ptr<const XDoc>;

        class PugiAttributeIterImpl : public XAttributeIterImpl
        {
        public:
            
            PugiAttributeIterImpl();

            PugiAttributeIterImpl(
                const pugi::xml_attribute_iterator& iter,
                const pugi::xml_node& iterParentElement,
                const XDocCPtr& parentXDoc );
            
            virtual ~PugiAttributeIterImpl() = default;
            
            virtual bool getIsPayloadNull() const override;
            virtual bool getIsEndIter() const override;

            virtual XAttributeIterImplUP clone() const override;
            virtual bool equals( const XAttributeIterator& other ) const override;
            
            virtual XAttribute& getRef() const override;
            virtual XAttribute* getPtr() const override;
            
            virtual const PugiAttributeIterImpl& increment() override;
            virtual const PugiAttributeIterImpl& decrement() override;
            
        public:
            pugi::xml_attribute_iterator myIter;
            pugi::xml_node myParentElement;
            XDocCWPtr myXDoc;
            mutable PugiAttribute myReturnableAttribute;
        };
    }
}
//PugiAttributeIterImpl{ myParentElement.attributes_begin(), myParentElement, myXDoc.lock() }
