// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/XsString.h"
#include "mx/core/elements/SlideAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( SlideAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Slide )

        inline SlidePtr makeSlide() { return std::make_shared<Slide>(); }
		inline SlidePtr makeSlide( const XsString& value ) { return std::make_shared<Slide>( value ); }
		inline SlidePtr makeSlide( XsString&& value ) { return std::make_shared<Slide>( std::move( value ) ); }

        class Slide : public ElementInterface
        {
        public:
            Slide();
            Slide( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            SlideAttributesPtr getAttributes() const;
            void setAttributes( const SlideAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            XsString myValue;
            SlideAttributesPtr myAttributes;
        };
    }
}
