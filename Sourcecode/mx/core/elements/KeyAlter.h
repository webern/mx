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

        MX_FORWARD_DECLARE_ELEMENT( KeyAlter )

        inline KeyAlterPtr makeKeyAlter() { return std::make_shared<KeyAlter>(); }
		inline KeyAlterPtr makeKeyAlter( const Semitones& value ) { return std::make_shared<KeyAlter>( value ); }
		inline KeyAlterPtr makeKeyAlter( Semitones&& value ) { return std::make_shared<KeyAlter>( std::move( value ) ); }

        class KeyAlter : public ElementInterface
        {
        public:
            KeyAlter();
            KeyAlter( const Semitones& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Semitones getValue() const;
            void setValue( const Semitones& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            Semitones myValue;
        };
    }
}
