// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/BeatUnitPerOrNoteRelationNoteChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/BeatUnitPer.h"
#include "mx/core/elements/NoteRelationNote.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BeatUnitPerOrNoteRelationNoteChoice::BeatUnitPerOrNoteRelationNoteChoice()
        :myChoice( Choice::beatUnitPer )
        ,myBeatUnitPer( makeBeatUnitPer() )
        ,myNoteRelationNote( makeNoteRelationNote() )
        {}


        bool BeatUnitPerOrNoteRelationNoteChoice::hasAttributes() const
        {
            return false;
        }


        std::ostream& BeatUnitPerOrNoteRelationNoteChoice::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& BeatUnitPerOrNoteRelationNoteChoice::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool BeatUnitPerOrNoteRelationNoteChoice::hasContents() const
        {
            return true;
        }


        std::ostream& BeatUnitPerOrNoteRelationNoteChoice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            switch ( myChoice )
            {
                case Choice::beatUnitPer:
                {
                    myBeatUnitPer->streamContents( os, indentLevel, isOneLineOnly );
                }
                    break;
                case Choice::noteRelationNote:
                {
                    myNoteRelationNote->streamContents( os, indentLevel, isOneLineOnly );
                }
                    break;
                default:
                    break;
            }
            isOneLineOnly = false;
            return os;
        }


        BeatUnitPerOrNoteRelationNoteChoice::Choice BeatUnitPerOrNoteRelationNoteChoice::getChoice() const
        {
            return myChoice;
        }


        void BeatUnitPerOrNoteRelationNoteChoice::setChoice( const BeatUnitPerOrNoteRelationNoteChoice::Choice value )
        {
            myChoice = value;
        }


        BeatUnitPerPtr BeatUnitPerOrNoteRelationNoteChoice::getBeatUnitPer() const
        {
            return myBeatUnitPer;
        }


        void BeatUnitPerOrNoteRelationNoteChoice::setBeatUnitPer( const BeatUnitPerPtr& value )
        {
            if ( value )
            {
                myBeatUnitPer = value;
            }
        }


        NoteRelationNotePtr BeatUnitPerOrNoteRelationNoteChoice::getNoteRelationNote() const
        {
            return myNoteRelationNote;
        }


        void BeatUnitPerOrNoteRelationNoteChoice::setNoteRelationNote( const NoteRelationNotePtr& value )
        {
            myNoteRelationNote = value;
        }


        bool BeatUnitPerOrNoteRelationNoteChoice::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_CHOICE_IF( beatUnitPer, "beat-unit-per", BeatUnitPer );
            MX_CHOICE_IF( noteRelationNote, "note-relation-note", NoteRelationNote );
            MX_BAD_ELEMENT_FAILURE( BeatUnitPerOrNoteRelationNoteChoice );
        }

    }
}
