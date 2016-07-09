// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Date.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( EncodingDate )

        inline EncodingDatePtr makeEncodingDate() { return std::make_shared<EncodingDate>(); }
		inline EncodingDatePtr makeEncodingDate( const Date& value ) { return std::make_shared<EncodingDate>( value ); }
		inline EncodingDatePtr makeEncodingDate( Date&& value ) { return std::make_shared<EncodingDate>( std::move( value ) ); }

        class EncodingDate : public ElementInterface
        {
        public:
            EncodingDate();
            EncodingDate( const Date& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Date getValue() const;
            void setValue( const Date& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            Date myValue;
        };
    }
}
