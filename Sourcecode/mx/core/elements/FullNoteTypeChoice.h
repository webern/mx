// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

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

        MX_FORWARD_DECLARE_ELEMENT( Pitch )
        MX_FORWARD_DECLARE_ELEMENT( Rest )
        MX_FORWARD_DECLARE_ELEMENT( Unpitched )
        MX_FORWARD_DECLARE_ELEMENT( FullNoteTypeChoice )

        inline FullNoteTypeChoicePtr makeFullNoteTypeChoice() { return std::make_shared<FullNoteTypeChoice>(); }

        class FullNoteTypeChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                pitch = 1,
                unpitched = 2,
                rest = 3
            };
            FullNoteTypeChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */
            FullNoteTypeChoice::Choice getChoice() const;
            void setChoice( const FullNoteTypeChoice::Choice value );

            /* _________ Pitch minOccurs = 1, maxOccurs = 1 _________ */
            PitchPtr getPitch() const;
            void setPitch( const PitchPtr& value );

            /* _________ Unpitched minOccurs = 1, maxOccurs = 1 _________ */
            UnpitchedPtr getUnpitched() const;
            void setUnpitched( const UnpitchedPtr& value );

            /* _________ Rest minOccurs = 1, maxOccurs = 1 _________ */
            RestPtr getRest() const;
            void setRest( const RestPtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            MX_MUTEX
            Choice myChoice;
            mutable PitchPtr myPitch;
            mutable UnpitchedPtr myUnpitched;
            mutable RestPtr myRest;
        };
    }
}
