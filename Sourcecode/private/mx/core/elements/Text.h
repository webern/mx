// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"
#include "mx/core/elements/TextAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( TextAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Text )

        inline TextPtr makeText() { return std::make_shared<Text>(); }
		inline TextPtr makeText( const XsString& value ) { return std::make_shared<Text>( value ); }
		inline TextPtr makeText( XsString&& value ) { return std::make_shared<Text>( std::move( value ) ); }

        class Text : public ElementInterface
        {
        public:
            Text();
            Text( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TextAttributesPtr getAttributes() const;
            void setAttributes( const TextAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            XsString myValue;
            TextAttributesPtr myAttributes;
        };
    }
}
