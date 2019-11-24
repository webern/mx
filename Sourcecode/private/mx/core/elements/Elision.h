// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"
#include "mx/core/elements/ElisionAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( ElisionAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Elision )

        inline ElisionPtr makeElision() { return std::make_shared<Elision>(); }
		inline ElisionPtr makeElision( const XsString& value ) { return std::make_shared<Elision>( value ); }
		inline ElisionPtr makeElision( XsString&& value ) { return std::make_shared<Elision>( std::move( value ) ); }

        class Elision : public ElementInterface
        {
        public:
            Elision();
            Elision( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ElisionAttributesPtr getAttributes() const;
            void setAttributes( const ElisionAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            XsString myValue;
            ElisionAttributesPtr myAttributes;
        };
    }
}
