// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"
#include "mx/core/elements/OtherNotationAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( OtherNotationAttributes )
        MX_FORWARD_DECLARE_ELEMENT( OtherNotation )

        inline OtherNotationPtr makeOtherNotation() { return std::make_shared<OtherNotation>(); }
		inline OtherNotationPtr makeOtherNotation( const XsString& value ) { return std::make_shared<OtherNotation>( value ); }
		inline OtherNotationPtr makeOtherNotation( XsString&& value ) { return std::make_shared<OtherNotation>( std::move( value ) ); }

        class OtherNotation : public ElementInterface
        {
        public:
            OtherNotation();
            OtherNotation( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OtherNotationAttributesPtr getAttributes() const;
            void setAttributes( const OtherNotationAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            XsString myValue;
            OtherNotationAttributesPtr myAttributes;
        };
    }
}
