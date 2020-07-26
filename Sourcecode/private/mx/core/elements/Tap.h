// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/XsString.h"
#include "mx/core/elements/TapAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( TapAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Tap )

        inline TapPtr makeTap() { return std::make_shared<Tap>(); }
		inline TapPtr makeTap( const XsString& value ) { return std::make_shared<Tap>( value ); }
		inline TapPtr makeTap( XsString&& value ) { return std::make_shared<Tap>( std::move( value ) ); }

        class Tap : public ElementInterface
        {
        public:
            Tap();
            Tap( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TapAttributesPtr getAttributes() const;
            void setAttributes( const TapAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            XsString myValue;
            TapAttributesPtr myAttributes;
        };
    }
}
