// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/PositiveIntegerOrEmpty.h"
#include "mx/core/elements/MeasureRepeatAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( MeasureRepeatAttributes )
        MX_FORWARD_DECLARE_ELEMENT( MeasureRepeat )

        inline MeasureRepeatPtr makeMeasureRepeat() { return std::make_shared<MeasureRepeat>(); }
		inline MeasureRepeatPtr makeMeasureRepeat( const PositiveIntegerOrEmpty& value ) { return std::make_shared<MeasureRepeat>( value ); }
		inline MeasureRepeatPtr makeMeasureRepeat( PositiveIntegerOrEmpty&& value ) { return std::make_shared<MeasureRepeat>( std::move( value ) ); }

        class MeasureRepeat : public ElementInterface
        {
        public:
            MeasureRepeat();
            MeasureRepeat( const PositiveIntegerOrEmpty& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MeasureRepeatAttributesPtr getAttributes() const;
            void setAttributes( const MeasureRepeatAttributesPtr& attributes );
            PositiveIntegerOrEmpty getValue() const;
            void setValue( const PositiveIntegerOrEmpty& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            PositiveIntegerOrEmpty myValue;
            MeasureRepeatAttributesPtr myAttributes;
        };
    }
}
