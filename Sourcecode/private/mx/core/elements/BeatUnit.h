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

        MX_FORWARD_DECLARE_ELEMENT( BeatUnit )

        inline BeatUnitPtr makeBeatUnit() { return std::make_shared<BeatUnit>(); }
		inline BeatUnitPtr makeBeatUnit( const NoteTypeValue& value ) { return std::make_shared<BeatUnit>( value ); }
		inline BeatUnitPtr makeBeatUnit( NoteTypeValue&& value ) { return std::make_shared<BeatUnit>( std::move( value ) ); }

        class BeatUnit : public ElementInterface
        {
        public:
            BeatUnit();
            BeatUnit( const NoteTypeValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NoteTypeValue getValue() const;
            void setValue( const NoteTypeValue& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            NoteTypeValue myValue;
        };
    }
}
