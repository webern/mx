// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/NoteChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/CueNoteGroup.h"
#include "mx/core/elements/GraceNoteGroup.h"
#include "mx/core/elements/NormalNoteGroup.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        NoteChoice::NoteChoice()
        :myChoice( Choice::normal )
        ,myGraceNoteGroup( makeGraceNoteGroup() )
        ,myCueNoteGroup( makeCueNoteGroup() )
        ,myNormalNoteGroup( makeNormalNoteGroup() )
        {}


        bool NoteChoice::hasAttributes() const
        {
            return false;
        }


        std::ostream& NoteChoice::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& NoteChoice::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool NoteChoice::hasContents() const
        {
            return true;
        }


        std::ostream& NoteChoice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            switch ( myChoice )
            {
                case Choice::grace:
                    myGraceNoteGroup->streamContents( os, indentLevel, isOneLineOnly );
                    break;
                case Choice::cue:
                    myCueNoteGroup->streamContents( os, indentLevel, isOneLineOnly );
                    break;
                case Choice::normal:
                    myNormalNoteGroup->streamContents( os, indentLevel, isOneLineOnly );
                    break;
                default:
                    break;
            }
            isOneLineOnly = false;
            return os;
        }


        NoteChoice::Choice NoteChoice::getChoice() const
        {
            return myChoice;
        }


        void NoteChoice::setChoice( const NoteChoice::Choice value )
        {
            myChoice = value;
        }


        GraceNoteGroupPtr NoteChoice::getGraceNoteGroup() const
        {
            return myGraceNoteGroup;
        }


        void NoteChoice::setGraceNoteGroup( const GraceNoteGroupPtr& value )
        {
            if ( value )
            {
                myGraceNoteGroup = value;
            }
        }


        CueNoteGroupPtr NoteChoice::getCueNoteGroup() const
        {
            return myCueNoteGroup;
        }


        void NoteChoice::setCueNoteGroup( const CueNoteGroupPtr& value )
        {
            if ( value )
            {
                myCueNoteGroup = value;
            }
        }


        NormalNoteGroupPtr NoteChoice::getNormalNoteGroup() const
        {
            return myNormalNoteGroup;
        }


        void NoteChoice::setNormalNoteGroup( const NormalNoteGroupPtr& value )
        {
            if ( value )
            {
                myNormalNoteGroup = value;
            }
        }

        MX_FROM_XELEMENT_UNUSED( NoteChoice );
    }
}
