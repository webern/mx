// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Integers.h"
#include "mx/core/elements/FretAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( FretAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Fret )

        inline FretPtr makeFret() { return std::make_shared<Fret>(); }
		inline FretPtr makeFret( const NonNegativeInteger& value ) { return std::make_shared<Fret>( value ); }
		inline FretPtr makeFret( NonNegativeInteger&& value ) { return std::make_shared<Fret>( std::move( value ) ); }

        class Fret : public ElementInterface
        {
        public:
            Fret();
            Fret( const NonNegativeInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            FretAttributesPtr getAttributes() const;
            void setAttributes( const FretAttributesPtr& attributes );
            NonNegativeInteger getValue() const;
            void setValue( const NonNegativeInteger& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            NonNegativeInteger myValue;
            FretAttributesPtr myAttributes;
        };
    }
}
