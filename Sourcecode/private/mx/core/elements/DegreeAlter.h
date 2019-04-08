// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Decimals.h"
#include "mx/core/elements/DegreeAlterAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( DegreeAlterAttributes )
        MX_FORWARD_DECLARE_ELEMENT( DegreeAlter )

        inline DegreeAlterPtr makeDegreeAlter() { return std::make_shared<DegreeAlter>(); }
		inline DegreeAlterPtr makeDegreeAlter( const Semitones& value ) { return std::make_shared<DegreeAlter>( value ); }
		inline DegreeAlterPtr makeDegreeAlter( Semitones&& value ) { return std::make_shared<DegreeAlter>( std::move( value ) ); }

        class DegreeAlter : public ElementInterface
        {
        public:
            DegreeAlter();
            DegreeAlter( const Semitones& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            DegreeAlterAttributesPtr getAttributes() const;
            void setAttributes( const DegreeAlterAttributesPtr& attributes );
            Semitones getValue() const;
            void setValue( const Semitones& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            Semitones myValue;
            DegreeAlterAttributesPtr myAttributes;
        };
    }
}
