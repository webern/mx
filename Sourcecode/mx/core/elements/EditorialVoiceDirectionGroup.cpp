// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/EditorialVoiceDirectionGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/Level.h"
#include "mx/core/elements/Voice.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        EditorialVoiceDirectionGroup::EditorialVoiceDirectionGroup()
        :myFootnote( makeFootnote() )
        ,myHasFootnote( false )
        ,myLevel( makeLevel() )
        ,myHasLevel( false )
        ,myVoice( makeVoice() )
        ,myHasVoice( false )
        {}


        bool EditorialVoiceDirectionGroup::hasAttributes() const
        {
            return false;
        }


        std::ostream& EditorialVoiceDirectionGroup::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& EditorialVoiceDirectionGroup::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool EditorialVoiceDirectionGroup::hasContents() const
        {
            return myHasFootnote || myHasLevel || myHasVoice;
        }


        std::ostream& EditorialVoiceDirectionGroup::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            bool firstItem = true;
            isOneLineOnly = true;
            if ( myHasFootnote )
            {
                myFootnote->toStream( os, indentLevel );
                firstItem = false;
            }
            if ( myHasLevel )
            {
                if( ! firstItem ) os << std::endl;
                myLevel->toStream( os, indentLevel );
                firstItem = false;
            }
            if ( myHasVoice )
            {
                if( ! firstItem ) os << std::endl;
                myVoice->toStream( os, indentLevel );
                firstItem = false;
            }
            isOneLineOnly = ! hasContents();
            return os;
        }


        FootnotePtr EditorialVoiceDirectionGroup::getFootnote() const
        {
            return myFootnote;
        }


        void EditorialVoiceDirectionGroup::setFootnote( const FootnotePtr& value )
        {
            if ( value )
            {
                myFootnote = value;
            }
        }


        bool EditorialVoiceDirectionGroup::getHasFootnote() const
        {
            return myHasFootnote;
        }


        void EditorialVoiceDirectionGroup::setHasFootnote( const bool value )
        {
            myHasFootnote = value;
        }


        LevelPtr EditorialVoiceDirectionGroup::getLevel() const
        {
            return myLevel;
        }


        void EditorialVoiceDirectionGroup::setLevel( const LevelPtr& value )
        {
            if ( value )
            {
                myLevel = value;
            }
        }


        bool EditorialVoiceDirectionGroup::getHasLevel() const
        {
            return myHasLevel;
        }


        void EditorialVoiceDirectionGroup::setHasLevel( const bool value )
        {
            myHasLevel = value;
        }


        VoicePtr EditorialVoiceDirectionGroup::getVoice() const
        {
            return myVoice;
        }


        void EditorialVoiceDirectionGroup::setVoice( const VoicePtr& value )
        {
            if ( value )
            {
                myVoice = value;
            }
        }


        bool EditorialVoiceDirectionGroup::getHasVoice() const
        {
            return myHasVoice;
        }


        void EditorialVoiceDirectionGroup::setHasVoice( const bool value )
        {
            myHasVoice = value;
        }
        
        
        MX_FROM_XELEMENT_UNUSED( EditorialVoiceDirectionGroup );

    }
}
