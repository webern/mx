// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"
#include "mx/core/elements/OtherAppearanceAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( OtherAppearanceAttributes )
        MX_FORWARD_DECLARE_ELEMENT( OtherAppearance )

        inline OtherAppearancePtr makeOtherAppearance() { return std::make_shared<OtherAppearance>(); }
		inline OtherAppearancePtr makeOtherAppearance( const XsString& value ) { return std::make_shared<OtherAppearance>( value ); }
		inline OtherAppearancePtr makeOtherAppearance( XsString&& value ) { return std::make_shared<OtherAppearance>( std::move( value ) ); }

        class OtherAppearance : public ElementInterface
        {
        public:
            OtherAppearance();
            OtherAppearance( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OtherAppearanceAttributesPtr getAttributes() const;
            void setAttributes( const OtherAppearanceAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            XsString myValue;
            OtherAppearanceAttributesPtr myAttributes;
        };
    }
}
