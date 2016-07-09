// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/MusicDataChoice.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MusicDataGroup::MusicDataGroup()
        :myMusicDataChoiceSet()
        {}


        bool MusicDataGroup::hasAttributes() const
        {
            return false;
        }


        std::ostream& MusicDataGroup::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& MusicDataGroup::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool MusicDataGroup::hasContents() const
        {
            return myMusicDataChoiceSet.size() > 0;
        }


        std::ostream& MusicDataGroup::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            for ( auto it = myMusicDataChoiceSet.cbegin();
                 it != myMusicDataChoiceSet.cend(); ++it )
            {
                if ( it != myMusicDataChoiceSet.cbegin() )
                {
                    os << std::endl;
                }
                (*it)->streamContents( os, indentLevel, isOneLineOnly );
            }
            isOneLineOnly = false;
            return os;
        }


        const MusicDataChoiceSet& MusicDataGroup::getMusicDataChoiceSet() const
        {
            return myMusicDataChoiceSet;
        }


        void MusicDataGroup::addMusicDataChoice( const MusicDataChoicePtr& value )
        {
            if ( value )
            {
                myMusicDataChoiceSet.push_back( value );
            }
        }


        void MusicDataGroup::removeMusicDataChoice( const MusicDataChoiceSetIterConst& value )
        {
            if ( value != myMusicDataChoiceSet.cend() )
            {
                myMusicDataChoiceSet.erase( value );
            }
        }


        void MusicDataGroup::clearMusicDataChoiceSet()
        {
            myMusicDataChoiceSet.clear();
        }


        MusicDataChoicePtr MusicDataGroup::getMusicDataChoice( const MusicDataChoiceSetIterConst& setIterator ) const
        {
            if( setIterator != myMusicDataChoiceSet.cend() )
            {
                return *setIterator;
            }
            return MusicDataChoicePtr();
        }
        
        
        MX_FROM_XELEMENT_UNUSED( MusicDataGroup );

    }
}
