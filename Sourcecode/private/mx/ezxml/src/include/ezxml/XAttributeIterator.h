// Copyright (c) Matthew James Briggs

#pragma once

#include "ezxml/XAttribute.h"
#include "ezxml/XAttributeIterImpl.h"

#include <iostream>
#include <memory>
#include <cstddef>

namespace ezxml
{
    class XAttributeIterator final
    {

    public:
        XAttributeIterator();
        explicit XAttributeIterator( const XAttributeIterImpl& impl );
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
        bool operator==( const XAttributeIterator& rhs ) const;
        bool operator!=( const XAttributeIterator& rhs ) const;

        XAttribute& operator*() const;
        XAttribute* operator->() const;

        const XAttributeIterator& operator++();
        XAttributeIterator operator++( int );

        const XAttributeIterator& operator--();
        XAttributeIterator operator--( int );

        // Reveal the private data member, violates
        // encapsulation but could not find a better way
        const XAttributeIterImplUP& reveal() const;

    private:
        XAttributeIterImplUP myImpl;
    };
}
