// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/NormalNoteGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Duration.h"
#include "mx/core/elements/FullNoteGroup.h"
#include "mx/core/elements/Tie.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        NormalNoteGroup::NormalNoteGroup()
        :myFullNoteGroup( makeFullNoteGroup() )
        ,myDuration( makeDuration() )
        ,myTieSet()
        {}


        bool NormalNoteGroup::hasAttributes() const
        {
            return false;
        }


        std::ostream& NormalNoteGroup::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& NormalNoteGroup::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool NormalNoteGroup::hasContents() const
        {
            return true;
        }


        std::ostream& NormalNoteGroup::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            myFullNoteGroup->streamContents( os, indentLevel, isOneLineOnly );
            os << std::endl;
            myDuration->toStream( os, indentLevel );
            for ( auto x : myTieSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel );
            }
            return os;
        }


        FullNoteGroupPtr NormalNoteGroup::getFullNoteGroup() const
        {
            return myFullNoteGroup;
        }


        void NormalNoteGroup::setFullNoteGroup( const FullNoteGroupPtr& value )
        {
            if ( value )
            {
                myFullNoteGroup = value;
            }
        }


        DurationPtr NormalNoteGroup::getDuration() const
        {
            return myDuration;
        }


        void NormalNoteGroup::setDuration( const DurationPtr& value )
        {
            if ( value )
            {
                myDuration = value;
            }
        }


        const TieSet& NormalNoteGroup::getTieSet() const
        {
            return myTieSet;
        }


        void NormalNoteGroup::addTie( const TiePtr& value )
        {
            if ( value )
            {
                if ( myTieSet.size() < 2 )
                {
                    myTieSet.push_back( value );
                }
            }
        }


        void NormalNoteGroup::removeTie( const TieSetIterConst& value )
        {
            if ( value != myTieSet.cend() )
            {
                myTieSet.erase( value );
            }
        }


        void NormalNoteGroup::clearTieSet()
        {
            myTieSet.clear();
        }


        TiePtr NormalNoteGroup::getTie( const TieSetIterConst& setIterator ) const
        {
            if( setIterator != myTieSet.cend() )
            {
                return *setIterator;
            }
            return TiePtr();
        }

        MX_FROM_XELEMENT_UNUSED( NormalNoteGroup );
    }
}
