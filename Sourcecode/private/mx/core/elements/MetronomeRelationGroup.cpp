// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/MetronomeRelationGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/MetronomeNote.h"
#include "mx/core/elements/MetronomeRelation.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MetronomeRelationGroup::MetronomeRelationGroup()
        :myMetronomeRelation( makeMetronomeRelation() )
        ,myMetronomeNote( makeMetronomeNote() )
        {}


        bool MetronomeRelationGroup::hasAttributes() const
        {
            return false;
        }


        std::ostream& MetronomeRelationGroup::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& MetronomeRelationGroup::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool MetronomeRelationGroup::hasContents() const
        {
            return true;
        }


        std::ostream& MetronomeRelationGroup::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            myMetronomeRelation->toStream( os, indentLevel );
            os << std::endl;
            myMetronomeNote->toStream( os, indentLevel );
            return os;
        }


        MetronomeRelationPtr MetronomeRelationGroup::getMetronomeRelation() const
        {
            return myMetronomeRelation;
        }


        void MetronomeRelationGroup::setMetronomeRelation( const MetronomeRelationPtr& value )
        {
            if ( value )
            {
                myMetronomeRelation = value;
            }
        }


        MetronomeNotePtr MetronomeRelationGroup::getMetronomeNote() const
        {
            return myMetronomeNote;
        }


        void MetronomeRelationGroup::setMetronomeNote( const MetronomeNotePtr& value )
        {
            if ( value )
            {
                myMetronomeNote = value;
            }
        }
        
        MX_FROM_XELEMENT_UNUSED( MetronomeRelationGroup );

    }
}
