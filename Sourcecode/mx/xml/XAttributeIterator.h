// MusicXML Class Library v0.1.1
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
            
            // TODO - how can I eliminate this violation of encapsulation?
            const XAttributeIterImplUP& reveal() const;
            
        private:
            XAttributeIterImplUP myImpl;
        };
    }
}
/*
public:
XElementIterator();
XElementIterator( const XElementIterImpl& impl );
XElementIterator( const XElementIterator& other );
XElementIterator( XElementIterator&& other ) = default;
XElementIterator& operator=( const XElementIterator& other );
XElementIterator& operator=( XElementIterator&& other ) = default;
~XElementIterator() = default;

// STL Iterator Compliance
typedef ptrdiff_t difference_type; // ???
typedef XElement value_type;
typedef XElement* pointer;
typedef XElement& reference;
typedef std::bidirectional_iterator_tag iterator_category;


// Iterator operators
bool operator==(const XElementIterator& rhs) const;
bool operator!=(const XElementIterator& rhs) const;

XElement& operator*() const;
XElement* operator->() const;

const XElementIterator& operator++();
XElementIterator operator++(int);

const XElementIterator& operator--();
XElementIterator operator--(int);

// TODO - how can I eliminate this violation of encapsulation?
const XElementIterImplUP& reveal() const;
*/