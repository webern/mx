// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"
#include "mx/core/elements/PrefixAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( PrefixAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Prefix )

        inline PrefixPtr makePrefix() { return std::make_shared<Prefix>(); }
		inline PrefixPtr makePrefix( const XsString& value ) { return std::make_shared<Prefix>( value ); }
		inline PrefixPtr makePrefix( XsString&& value ) { return std::make_shared<Prefix>( std::move( value ) ); }

        class Prefix : public ElementInterface
        {
        public:
            Prefix();
            Prefix( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PrefixAttributesPtr getAttributes() const;
            void setAttributes( const PrefixAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            XsString myValue;
            PrefixAttributesPtr myAttributes;
        };
    }
}
