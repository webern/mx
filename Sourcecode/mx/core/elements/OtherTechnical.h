// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"
#include "mx/core/elements/OtherTechnicalAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( OtherTechnicalAttributes )
        MX_FORWARD_DECLARE_ELEMENT( OtherTechnical )

        inline OtherTechnicalPtr makeOtherTechnical() { return std::make_shared<OtherTechnical>(); }
		inline OtherTechnicalPtr makeOtherTechnical( const XsString& value ) { return std::make_shared<OtherTechnical>( value ); }
		inline OtherTechnicalPtr makeOtherTechnical( XsString&& value ) { return std::make_shared<OtherTechnical>( std::move( value ) ); }

        class OtherTechnical : public ElementInterface
        {
        public:
            OtherTechnical();
            OtherTechnical( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OtherTechnicalAttributesPtr getAttributes() const;
            void setAttributes( const OtherTechnicalAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            XsString myValue;
            OtherTechnicalAttributesPtr myAttributes;
        };
    }
}
