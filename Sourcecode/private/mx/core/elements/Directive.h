// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/XsString.h"
#include "mx/core/elements/DirectiveAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( DirectiveAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Directive )

        inline DirectivePtr makeDirective() { return std::make_shared<Directive>(); }
        inline DirectivePtr makeDirective( const XsString& value ) { return std::make_shared<Directive>( value ); }
        inline DirectivePtr makeDirective( XsString&& value ) { return std::make_shared<Directive>( std::move( value ) ); }

        class Directive : public ElementInterface
        {
        public:
            Directive();
            Directive( const XsString& value );

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DirectiveAttributesPtr getAttributes() const;
            void setAttributes( const DirectiveAttributesPtr& value );
            XsString getValue() const;
            void setValue( const XsString& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            DirectiveAttributesPtr myAttributes;
            XsString myValue;
        };
    }
}
