// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Enums.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( Pitched )

        inline PitchedPtr makePitched() { return std::make_shared<Pitched>(); }
		inline PitchedPtr makePitched( const PitchedEnum& value ) { return std::make_shared<Pitched>( value ); }
		inline PitchedPtr makePitched( PitchedEnum&& value ) { return std::make_shared<Pitched>( std::move( value ) ); }

        class Pitched : public ElementInterface
        {
        public:
            Pitched();
            Pitched( const PitchedEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PitchedEnum getValue() const;
            void setValue( const PitchedEnum& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            PitchedEnum myValue;
        };
    }
}
