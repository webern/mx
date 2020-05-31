// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ElementInterface.h"
#include "mx/core/Enums.h"
#include "mx/core/ForwardDeclare.h"
#include "mx/core/PlaybackSound.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( InstrumentSound )

        inline InstrumentSoundPtr makeInstrumentSound() { return std::make_shared<InstrumentSound>(); }
		inline InstrumentSoundPtr makeInstrumentSound( PlaybackSound value ) { return std::make_shared<InstrumentSound>( value ); }

        class InstrumentSound : public ElementInterface
        {
        public:
            InstrumentSound();
            InstrumentSound( const PlaybackSound& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PlaybackSound getValue() const;
            void setValue( const PlaybackSound& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            PlaybackSound myValue;
        };
    }
}
