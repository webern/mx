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

        MX_FORWARD_DECLARE_ELEMENT( Chord )
        MX_FORWARD_DECLARE_ELEMENT( FullNoteTypeChoice )
        MX_FORWARD_DECLARE_ELEMENT( FullNoteGroup )

        inline FullNoteGroupPtr makeFullNoteGroup() { return std::make_shared<FullNoteGroup>(); }

        class FullNoteGroup : public ElementInterface
        {
        public:
            FullNoteGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Chord minOccurs = 0, maxOccurs = 1 _________ */
            ChordPtr getChord() const;
            void setChord( const ChordPtr& value );
            bool getHasChord() const;
            void setHasChord( const bool value );

            /* _________ FullNoteTypeChoice minOccurs = 1, maxOccurs = 1 _________ */
            FullNoteTypeChoicePtr getFullNoteTypeChoice() const;
            void setFullNoteTypeChoice( const FullNoteTypeChoicePtr& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            MX_MUTEX
            ChordPtr myChord;
            bool myHasChord;
            mutable FullNoteTypeChoicePtr myFullNoteTypeChoice;
        };
    }
}
