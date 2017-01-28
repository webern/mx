// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( Barre )
        MX_FORWARD_DECLARE_ELEMENT( Fingering )
        MX_FORWARD_DECLARE_ELEMENT( Fret )
        MX_FORWARD_DECLARE_ELEMENT( String )
        MX_FORWARD_DECLARE_ELEMENT( FrameNote )

        inline FrameNotePtr makeFrameNote() { return std::make_shared<FrameNote>(); }

        class FrameNote : public ElementInterface
        {
        public:
            FrameNote();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ String minOccurs = 1, maxOccurs = 1 _________ */
            StringPtr getString() const;
            void setString( const StringPtr& value );

            /* _________ Fret minOccurs = 1, maxOccurs = 1 _________ */
            FretPtr getFret() const;
            void setFret( const FretPtr& value );

            /* _________ Fingering minOccurs = 0, maxOccurs = 1 _________ */
            FingeringPtr getFingering() const;
            void setFingering( const FingeringPtr& value );
            bool getHasFingering() const;
            void setHasFingering( const bool value );

            /* _________ Barre minOccurs = 0, maxOccurs = 1 _________ */
            BarrePtr getBarre() const;
            void setBarre( const BarrePtr& value );
            bool getHasBarre() const;
            void setHasBarre( const bool value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            StringPtr myString;
            FretPtr myFret;
            FingeringPtr myFingering;
            bool myHasFingering;
            BarrePtr myBarre;
            bool myHasBarre;
        };
    }
}
