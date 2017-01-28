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

        MX_FORWARD_DECLARE_ELEMENT( BeatUnitPer )
        MX_FORWARD_DECLARE_ELEMENT( NoteRelationNote )
        MX_FORWARD_DECLARE_ELEMENT( BeatUnitPerOrNoteRelationNoteChoice )

        inline BeatUnitPerOrNoteRelationNoteChoicePtr makeBeatUnitPerOrNoteRelationNoteChoice() { return std::make_shared<BeatUnitPerOrNoteRelationNoteChoice>(); }

        class BeatUnitPerOrNoteRelationNoteChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                beatUnitPer = 0,
                noteRelationNote = 1
            };
            BeatUnitPerOrNoteRelationNoteChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BeatUnitPerOrNoteRelationNoteChoice::Choice getChoice() const;
            void setChoice( const BeatUnitPerOrNoteRelationNoteChoice::Choice value );
            BeatUnitPerPtr getBeatUnitPer() const;
            void setBeatUnitPer( const BeatUnitPerPtr& value );
            NoteRelationNotePtr getNoteRelationNote() const;
            void setNoteRelationNote( const NoteRelationNotePtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            Choice myChoice;
            BeatUnitPerPtr myBeatUnitPer;
            NoteRelationNotePtr myNoteRelationNote;
        };
    }
}
