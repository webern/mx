// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"
#include "mx/core/elements/PerMinuteAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( PerMinuteAttributes )
        MX_FORWARD_DECLARE_ELEMENT( PerMinute )

        inline PerMinutePtr makePerMinute() { return std::make_shared<PerMinute>(); }
		inline PerMinutePtr makePerMinute( const XsString& value ) { return std::make_shared<PerMinute>( value ); }
		inline PerMinutePtr makePerMinute( XsString&& value ) { return std::make_shared<PerMinute>( std::move( value ) ); }

        class PerMinute : public ElementInterface
        {
        public:
            PerMinute();
            PerMinute( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PerMinuteAttributesPtr getAttributes() const;
            void setAttributes( const PerMinuteAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            XsString myValue;
            PerMinuteAttributesPtr myAttributes;
        };
    }
}
