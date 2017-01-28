// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Enums.h"
#include "mx/core/elements/DegreeTypeAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( DegreeTypeAttributes )
        MX_FORWARD_DECLARE_ELEMENT( DegreeType )

        inline DegreeTypePtr makeDegreeType() { return std::make_shared<DegreeType>(); }
		inline DegreeTypePtr makeDegreeType( const DegreeTypeValue& value ) { return std::make_shared<DegreeType>( value ); }
		inline DegreeTypePtr makeDegreeType( DegreeTypeValue&& value ) { return std::make_shared<DegreeType>( std::move( value ) ); }

        class DegreeType : public ElementInterface
        {
        public:
            DegreeType();
            DegreeType( const DegreeTypeValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DegreeTypeAttributesPtr getAttributes() const;
            void setAttributes( const DegreeTypeAttributesPtr& attributes );
            DegreeTypeValue getValue() const;
            void setValue( const DegreeTypeValue& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            DegreeTypeValue myValue;
            DegreeTypeAttributesPtr myAttributes;
        };
    }
}
