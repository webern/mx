// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/xml/XAttribute.h"

#include <memory>

namespace mx
{
    namespace xml
    {
        class XAttributeIterImpl;
        using XAttributeIterImplUP = std::unique_ptr<XAttributeIterImpl>;
        
        class XAttributeIterImpl
        {
        public:

            virtual ~XAttributeIterImpl() {};
            
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
}
