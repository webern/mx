// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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

        MX_FORWARD_DECLARE_ELEMENT( MeasureDistance )

        inline MeasureDistancePtr makeMeasureDistance() { return std::make_shared<MeasureDistance>(); }
		inline MeasureDistancePtr makeMeasureDistance( const TenthsValue& value ) { return std::make_shared<MeasureDistance>( value ); }
		inline MeasureDistancePtr makeMeasureDistance( TenthsValue&& value ) { return std::make_shared<MeasureDistance>( std::move( value ) ); }

        class MeasureDistance : public ElementInterface
        {
        public:
            MeasureDistance();
            MeasureDistance( const TenthsValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TenthsValue getValue() const;
            void setValue( const TenthsValue& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            TenthsValue myValue;
        };
    }
}
