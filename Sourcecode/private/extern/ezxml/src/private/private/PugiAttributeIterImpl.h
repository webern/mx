// Copyright (c) Matthew James Briggs

#pragma once

#include "ezxml/XAttributeIterator.h"
#include "ezxml/XAttributeIterImpl.h"
#include "private/PugiAttribute.h"
#include <pugixml.hpp>

namespace ezxml
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
