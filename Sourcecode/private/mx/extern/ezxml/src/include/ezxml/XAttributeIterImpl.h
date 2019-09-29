// Copyright (c) Matthew James Briggs

#pragma once

#include "ezxml/XAttribute.h"

#include <memory>

namespace ezxml
{
    class XAttributeIterImpl;
    using XAttributeIterImplUP = std::unique_ptr<XAttributeIterImpl>;

    class XAttributeIterImpl
    {
    public:

        virtual ~XAttributeIterImpl() = default;

        virtual bool getIsPayloadNull() const = 0;
        virtual bool getIsEndIter() const = 0;

        virtual XAttributeIterImplUP clone() const = 0;
        virtual bool equals( const XAttributeIterator& other ) const = 0;

        virtual XAttribute& getRef() const = 0;
        virtual XAttribute* getPtr() const = 0;

        virtual const XAttributeIterImpl& increment() = 0;
        virtual const XAttributeIterImpl& decrement() = 0;
    };
}
