// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Enums.h"
#include "mx/core/elements/MeasureNumberingAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( MeasureNumberingAttributes )
        MX_FORWARD_DECLARE_ELEMENT( MeasureNumbering )

        inline MeasureNumberingPtr makeMeasureNumbering() { return std::make_shared<MeasureNumbering>(); }
		inline MeasureNumberingPtr makeMeasureNumbering( const MeasureNumberingValue& value ) { return std::make_shared<MeasureNumbering>( value ); }
		inline MeasureNumberingPtr makeMeasureNumbering( MeasureNumberingValue&& value ) { return std::make_shared<MeasureNumbering>( std::move( value ) ); }

        class MeasureNumbering : public ElementInterface
        {
        public:
            MeasureNumbering();
            MeasureNumbering( const MeasureNumberingValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MeasureNumberingAttributesPtr getAttributes() const;
            void setAttributes( const MeasureNumberingAttributesPtr& attributes );
            MeasureNumberingValue getValue() const;
            void setValue( const MeasureNumberingValue& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            MeasureNumberingValue myValue;
            MeasureNumberingAttributesPtr myAttributes;
        };
    }
}
