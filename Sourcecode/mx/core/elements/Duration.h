// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Decimals.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( Duration )

        inline DurationPtr makeDuration() { return std::make_shared<Duration>(); }
		inline DurationPtr makeDuration( const PositiveDivisionsValue& value ) { return std::make_shared<Duration>( value ); }
		inline DurationPtr makeDuration( PositiveDivisionsValue&& value ) { return std::make_shared<Duration>( std::move( value ) ); }

        class Duration : public ElementInterface
        {
        public:
            Duration();
            Duration( const PositiveDivisionsValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PositiveDivisionsValue getValue() const;
            void setValue( const PositiveDivisionsValue& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            PositiveDivisionsValue myValue;
        };
    }
}
