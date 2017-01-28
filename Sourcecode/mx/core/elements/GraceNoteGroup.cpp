// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/GraceNoteGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/FullNoteGroup.h"
#include "mx/core/elements/Grace.h"
#include "mx/core/elements/Tie.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        GraceNoteGroup::GraceNoteGroup()
        :myGrace( makeGrace() )
        ,myFullNoteGroup( makeFullNoteGroup() )
        ,myTieSet()
        {}


        bool GraceNoteGroup::hasAttributes() const
        {
            return false;
        }


        std::ostream& GraceNoteGroup::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& GraceNoteGroup::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool GraceNoteGroup::hasContents() const
        {
            return true;
        }


        std::ostream& GraceNoteGroup::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            myGrace->toStream( os, indentLevel );
            os << std::endl;
            myFullNoteGroup->streamContents( os, indentLevel, isOneLineOnly );
            for ( auto x : myTieSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel );
            }
            isOneLineOnly = false;
            return os;
        }


        GracePtr GraceNoteGroup::getGrace() const
        {
            return myGrace;
        }


        void GraceNoteGroup::setGrace( const GracePtr& value )
        {
            if ( value )
            {
                myGrace = value;
            }
        }


        FullNoteGroupPtr GraceNoteGroup::getFullNoteGroup() const
        {
            return myFullNoteGroup;
        }


        void GraceNoteGroup::setFullNoteGroup( const FullNoteGroupPtr& value )
        {
            if ( value )
            {
                myFullNoteGroup = value;
            }
        }


        const TieSet& GraceNoteGroup::getTieSet() const
        {
            return myTieSet;
        }


        void GraceNoteGroup::addTie( const TiePtr& value )
        {
            if ( value )
            {
                if ( myTieSet.size() < 2 )
                {
                    myTieSet.push_back( value );
                }
            }
        }


        void GraceNoteGroup::removeTie( const TieSetIterConst& value )
        {
            if ( value != myTieSet.cend() )
            {
                myTieSet.erase( value );
            }
        }


        void GraceNoteGroup::clearTieSet()
        {
            myTieSet.clear();
        }


        TiePtr GraceNoteGroup::getTie( const TieSetIterConst& setIterator ) const
        {
            if( setIterator != myTieSet.cend() )
            {
                return *setIterator;
            }
            return TiePtr();
        }
        
        
        MX_FROM_XELEMENT_UNUSED( GraceNoteGroup );

    }
}
