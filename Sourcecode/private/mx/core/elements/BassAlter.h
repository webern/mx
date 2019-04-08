// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Decimals.h"
#include "mx/core/elements/BassAlterAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( BassAlterAttributes )
        MX_FORWARD_DECLARE_ELEMENT( BassAlter )

        inline BassAlterPtr makeBassAlter() { return std::make_shared<BassAlter>(); }
		inline BassAlterPtr makeBassAlter( const Semitones& value ) { return std::make_shared<BassAlter>( value ); }
		inline BassAlterPtr makeBassAlter( Semitones&& value ) { return std::make_shared<BassAlter>( std::move( value ) ); }

        class BassAlter : public ElementInterface
        {
        public:
            BassAlter();
            BassAlter( const Semitones& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BassAlterAttributesPtr getAttributes() const;
            void setAttributes( const BassAlterAttributesPtr& attributes );
            Semitones getValue() const;
            void setValue( const Semitones& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            Semitones myValue;
            BassAlterAttributesPtr myAttributes;
        };
    }
}
