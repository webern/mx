// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/LyricFontAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( LyricFontAttributes )
        MX_FORWARD_DECLARE_ELEMENT( LyricFont )

        inline LyricFontPtr makeLyricFont() { return std::make_shared<LyricFont>(); }

        class LyricFont : public ElementInterface
        {
        public:
            LyricFont();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            LyricFontAttributesPtr getAttributes() const;
            void setAttributes( const LyricFontAttributesPtr& attributes );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            LyricFontAttributesPtr myAttributes;
        };
    }
}
