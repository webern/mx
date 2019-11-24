// Copyright (c) Matthew James Briggs

#pragma once

#include "ezxml/XElement.h"
#include "ezxml/XElementIterImpl.h"

#include <iostream>
#include <memory>
#include <cstddef>

namespace ezxml
{
    class XElementIterator final
    {

    public:
        XElementIterator();
        explicit XElementIterator( const XElementIterImpl& impl );
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
        bool operator==( const XElementIterator& rhs ) const;
        bool operator!=( const XElementIterator& rhs ) const;

        XElement& operator*() const;
        XElement* operator->() const;

        const XElementIterator& operator++();
        const XElementIterator operator++( int );

        const XElementIterator& operator--();
        const XElementIterator operator--( int );

        bool getIsPayloadNull() const;

        bool getSkipProcessingInstructions() const;
        void setSkipProcessingInstructions( bool inValue );

        // Reveal the private data member, violates
        // encapsulation but could not find a better way
        const XElementIterImplUP& reveal() const;

    private:
        XElementIterImplUP myImpl;
    };
}
