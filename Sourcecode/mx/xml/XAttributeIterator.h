// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/xml/XAttribute.h"
#include "mx/xml/XAttributeIterImpl.h"

#include <iostream>
#include <memory>

namespace mx
{
    namespace xml
    {
        class XAttributeIterator final
        {

        public:
            XAttributeIterator();
            XAttributeIterator( const XAttributeIterImpl& impl );
            XAttributeIterator( const XAttributeIterator& other );
            XAttributeIterator( XAttributeIterator&& other ) = default;
            XAttributeIterator& operator=( const XAttributeIterator& other );
            XAttributeIterator& operator=( XAttributeIterator&& other ) = default;
            ~XAttributeIterator() = default;
            
            // STL Iterator Compliance
            typedef ptrdiff_t difference_type; // ???
            typedef XAttribute value_type;
            typedef XAttribute* pointer;
            typedef XAttribute& reference;
            typedef std::bidirectional_iterator_tag iterator_category;
            
            
            // Iterator operators
            bool operator==(const XAttributeIterator& rhs) const;
            bool operator!=(const XAttributeIterator& rhs) const;
            
            XAttribute& operator*() const;
            XAttribute* operator->() const;
            
            const XAttributeIterator& operator++();
            XAttributeIterator operator++(int);
            
            const XAttributeIterator& operator--();
            XAttributeIterator operator--(int);
            
            // Reveal the private data member, violates
            // encapsulation but could not find a better way
            const XAttributeIterImplUP& reveal() const;
            
        private:
            XAttributeIterImplUP myImpl;
        };
    }
}
