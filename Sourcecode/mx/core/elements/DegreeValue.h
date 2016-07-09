// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Integers.h"
#include "mx/core/elements/DegreeValueAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( DegreeValueAttributes )
        MX_FORWARD_DECLARE_ELEMENT( DegreeValue )

        inline DegreeValuePtr makeDegreeValue() { return std::make_shared<DegreeValue>(); }
		inline DegreeValuePtr makeDegreeValue( const PositiveInteger& value ) { return std::make_shared<DegreeValue>( value ); }
		inline DegreeValuePtr makeDegreeValue( PositiveInteger&& value ) { return std::make_shared<DegreeValue>( std::move( value ) ); }

        class DegreeValue : public ElementInterface
        {
        public:
            DegreeValue();
            DegreeValue( const PositiveInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DegreeValueAttributesPtr getAttributes() const;
            void setAttributes( const DegreeValueAttributesPtr& attributes );
            PositiveInteger getValue() const;
            void setValue( const PositiveInteger& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            PositiveInteger myValue;
            DegreeValueAttributesPtr myAttributes;
        };
    }
}
