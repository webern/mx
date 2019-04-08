// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/NoteRelationNote.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/MetronomeNote.h"
#include "mx/core/elements/MetronomeRelationGroup.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        NoteRelationNote::NoteRelationNote()
        :myMetronomeNoteSet()
        ,myMetronomeRelationGroup( makeMetronomeRelationGroup() )
        ,myHasMetronomeRelationGroup( false )
        {
            myMetronomeNoteSet.push_back( makeMetronomeNote() );
        }


        bool NoteRelationNote::hasAttributes() const
        {
            return false;
        }


        std::ostream& NoteRelationNote::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& NoteRelationNote::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool NoteRelationNote::hasContents() const
        {
            return true;
        }


        std::ostream& NoteRelationNote::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            for ( auto it = myMetronomeNoteSet.cbegin();
                 it != myMetronomeNoteSet.cend();
                 ++it )
            {
                if ( it != myMetronomeNoteSet.cbegin() )
                {
                    os << std::endl;
                }
                (*it)->toStream( os, indentLevel );
            }
            if ( myHasMetronomeRelationGroup )
            {
                os << std::endl;
                myMetronomeRelationGroup->streamContents( os, indentLevel, isOneLineOnly );
            }
            isOneLineOnly = false;
            return os;
        }


        const MetronomeNoteSet& NoteRelationNote::getMetronomeNoteSet() const
        {
            return myMetronomeNoteSet;
        }


        void NoteRelationNote::addMetronomeNote( const MetronomeNotePtr& value )
        {
            if ( value )
            {
                myMetronomeNoteSet.push_back( value );
            }
        }


        void NoteRelationNote::removeMetronomeNote( const MetronomeNoteSetIterConst& setIterator )
        {
            if ( setIterator != myMetronomeNoteSet.cend() )
            {
                if ( myMetronomeNoteSet.size() > 1 )
                {
                    myMetronomeNoteSet.erase( setIterator );
                }
            }
        }


        void NoteRelationNote::clearMetronomeNoteSet()
        {
            myMetronomeNoteSet.clear();
            myMetronomeNoteSet.push_back( makeMetronomeNote() );
        }


        MetronomeRelationGroupPtr NoteRelationNote::getMetronomeRelationGroup() const
        {
            return myMetronomeRelationGroup;
        }


        void NoteRelationNote::setMetronomeRelationGroup( const MetronomeRelationGroupPtr& value )
        {
            if ( value )
            {
                myMetronomeRelationGroup = value;
            }
        }


        bool NoteRelationNote::getHasMetronomeRelationGroup() const
        {
            return myHasMetronomeRelationGroup;
        }


        void NoteRelationNote::setHasMetronomeRelationGroup( const bool value )
        {
            myHasMetronomeRelationGroup = value;
        }


        bool NoteRelationNote::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                importElementSet( message, it, endIter, isSuccess, "metronome-note", myMetronomeNoteSet );
                importGroup( message, it, endIter, isSuccess, myMetronomeRelationGroup, myHasMetronomeRelationGroup );
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
