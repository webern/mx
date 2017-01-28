// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/BookmarkAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( BookmarkAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Bookmark )

        inline BookmarkPtr makeBookmark() { return std::make_shared<Bookmark>(); }

        class Bookmark : public ElementInterface
        {
        public:
            Bookmark();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BookmarkAttributesPtr getAttributes() const;
            void setAttributes( const BookmarkAttributesPtr& attributes );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            BookmarkAttributesPtr myAttributes;
        };
    }
}
