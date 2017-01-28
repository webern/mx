// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License


#pragma once

#include "mx/xml/XElementIterator.h"

#include <memory>

namespace mx
{
    namespace xml
    {
        class XElementIterImpl;
        using XElementIterImplUP = std::unique_ptr<XElementIterImpl>;
        
        class XElementIterImpl
        {
        public:

            virtual ~XElementIterImpl() {};

            virtual bool getIsPayloadNull() const = 0;
            virtual bool getIsEndIter() const = 0;

            virtual XElementIterImplUP clone() const = 0;
            virtual bool equals( const XElementIterator& other ) const = 0;
            
            virtual XElement& getRef() const = 0;
            virtual XElement* getPtr() const = 0;
            
            virtual const XElementIterImpl& increment() = 0;
            virtual const XElementIterImpl& decrement() = 0;
        };
    }
}
