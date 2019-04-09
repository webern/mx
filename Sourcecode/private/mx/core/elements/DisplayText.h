// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"
#include "mx/core/elements/DisplayTextAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( DisplayTextAttributes )
        MX_FORWARD_DECLARE_ELEMENT( DisplayText )

        inline DisplayTextPtr makeDisplayText() { return std::make_shared<DisplayText>(); }
		inline DisplayTextPtr makeDisplayText( const XsString& value ) { return std::make_shared<DisplayText>( value ); }
		inline DisplayTextPtr makeDisplayText( XsString&& value ) { return std::make_shared<DisplayText>( std::move( value ) ); }

        class DisplayText : public ElementInterface
        {
        public:
            DisplayText();
            DisplayText( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DisplayTextAttributesPtr getAttributes() const;
            void setAttributes( const DisplayTextAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            XsString myValue;
            DisplayTextAttributesPtr myAttributes;
        };
    }
}
